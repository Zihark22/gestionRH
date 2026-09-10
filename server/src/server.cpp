#include "../includes/server.hpp"

Server::Server(int argc, char* argv[]) {
    this->config_file_path = "config/config.ini"; // Valeur par défaut
    this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Erreur création socket" << std::endl;
        exit(EXIT_FAILURE);
    }
    this->opt = 1;
    load_config(config_file_path);
    this->apiServer = ApiServer(db_path);

    // Sauvegarde du chemin et des arguments
    m_executablePath = argv[0];
    for (int i = 0; i < argc; ++i) {
        m_args.push_back(argv[i]);
    }

    // Server s'abonne à l'API (fonction de Callback)
    apiServer.set_request_handler([this](const std::string& req, const std::string& body) -> std::string {
        return this->handle_action(req, body);
    });
}
Server::~Server(){
    if (server_fd >= 0) {
        ::close(server_fd);
        server_fd = -1;
    }

} 
std::string Server::handle_action(const std::string &req, const std::string &body) {
    std::string msg="OK";

    if(req=="getconfig")
        return config_to_json();
    else if(req=="modifyconfig") { 
        modify_config_from_json(body);
        
        this->requestRestart();  // Déclencher le redémarrage asynchrone

        return "Configuration modifiée avec succès. Redémarrage du serveur pour appliquer les changements...";
    }
    else
        return "Invalid request";
}

void Server::requestRestart() {
    // On lance le redémarrage dans un thread séparé !
    // Cela permet au thread appelant de finir sa fonction,
    // de flush la socket HTTP et de renvoyer le 200 OK au client.
    std::thread([this]() {
        this->executeRestart();
    }).detach();
}
void Server::executeRestart() {
    // 1. Laisser 200 à 500 ms pour que le paquet TCP HTTP "200 OK" sorte sur le réseau
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "[SERVER] Redémarrage du processus en cours..." << std::endl;

    // 2. Reconstruire le tableau char* compatible POSIX attendu par execv
    std::vector<char*> rawArgs;
    for (auto& arg : m_args) {
        rawArgs.push_back(arg.data());
    }
    rawArgs.push_back(nullptr); // execv DOIT se terminer par un pointeur null

    // 3. Appel système remplaçant le processus actuel
    execv(m_executablePath.c_str(), rawArgs.data());

    // Si on arrive ici, l'appel a échoué
    perror("[SERVER] Échec critique de execv");
}

std::string Server::config_to_json(){
    std::string rep_json="[{";
    rep_json += "\"host\":\""+host+"\",";
    rep_json += "\"port\":" + std::to_string(port) + ",";
    rep_json += "\"db_path\":\""+db_path+"\"";
    return rep_json+"}]" ;
}

int Server::modify_config_from_json(const std::string &json){
    // ex : json = [{"host":"127.0.0.1","port":8080,"db_path":"../data/employees.db"}]
    std::cout << "modifying ...." << std::endl;
    
    std::string s = IniParser::trim(json);

    if (s.size() < 2 || s.front() != '[' || s.back() != ']')
        return 1;

    std::string inner = IniParser::trim(s.substr(1, s.size() - 2));

    if( inner.empty() || inner.front() != '{' || inner.back() != '}' )
        return 1;

    std::string obj = IniParser::trim(inner.substr(1, inner.size() - 2));

    std::string new_host = IniParser::getField(obj, "host");
    int new_port = stoi(IniParser::getField(obj, "port"));
    std::string new_db_path = IniParser::getField(obj, "db_path");

    // remplacer config dans fichier (relancer pour activer nouvelle config)
    std::fstream fichier;
    fichier.open(config_file_path.c_str(), std::ios::in | std::ios::out);
    if (fichier.is_open()) {
        std::cout << "Fichier ouvert pour modification dont chemin: " << config_file_path << std::endl;
        std::string ligne;
        std::string contenu;
        fichier.seekg(0);
        while (getline(fichier, ligne)) {
            if (ligne.find("host =") != std::string::npos) {
                contenu += "host = " + new_host + "\n";
            } 
            else if (ligne.find("port =") != std::string::npos) {
                contenu += "port = " + std::to_string(new_port) + "\n";
            }
            else if (ligne.find("db_path =") != std::string::npos) {
                contenu += "db_path = " + new_db_path + "\n";
            }            
            else {
                contenu += ligne + "\n";
            }
        }
        fichier.clear();
        fichier.seekp(0);
        fichier.write(contenu.c_str(), contenu.length());
        fichier.close();
    } else {
        std::cerr << "Impossible d'ouvrir le fichier en écriture." << std::endl;
    }

    return 0;
} 
void Server::load_config(const std::string& file_path) {
    auto config = IniParser::parse(file_path);

    for (const auto& section : config) {
        if (section.nom == "Server") {
            if (auto var = section.getVariable("port")) {
                try {
                    this->port = stoi(var->valeur);
                } catch (...) {
                    std::cerr << "[loadConfig] Invalid port value: " << var->valeur << std::endl;
                }
            }
            if (auto var = section.getVariable("host")) {
                this->host = var->valeur;
            }
        }
        else if (section.nom == "Database") {
            if (auto var = section.getVariable("db_path")) {
                this->db_path = var->valeur;
            }
        }
        else if (section.nom == "Logging") {
            if (auto var = section.getVariable("log_file")) {
                this->log_file = var->valeur;
            }
        }
    }

    std::cout << "[Server] Config loaded: host=" << (host.empty()?"(none)":host) << " port=" << port << " db_path=" << (db_path.empty()?"(none)":db_path) << " log_file=" << (log_file.empty()?"(none)":log_file) << std::endl;
}
void Server::start() {
    // Réutilisation du port pour éviter l'erreur "Address already in use"
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) { // server_fd est le socket du serveur
        std::cerr << "Erreur bind" << std::endl;
        close(server_fd);
        return;
    }

    if (listen(server_fd, 10) < 0) {
        std::cerr << "Erreur listen" << std::endl;
        close(server_fd);
        return;
    }


    std::cout << "\n============= Serveur démarré sur http://" << host << ":" << port << " ==============" << std::endl;

    std::thread monThread([this]() {
        std::cout << "[Thread API] Démarrage du serveur API..." << std::endl;
        apiServer.start(server_fd);
    });

    // Le thread principal continue son travail en parallèle
    std::cout << "[Thread Server] Attente d'une demande du thread API..." << std::endl;

    // Attente bloquante : le main attend que 'monThread' ait fini avant de continuer
    monThread.join();
}