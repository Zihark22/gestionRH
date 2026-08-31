#include "../includes/server.hpp"

Server::Server() {
    this->config_file_path = "config/config.ini"; // Valeur par défaut
    this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Erreur création socket" << std::endl;
        exit(EXIT_FAILURE);
    }
    this->opt = 1;
    load_config(config_file_path);
    this->apiServer = ApiServer(db_path);

    // Server s'abonne à l'API (fonction de Callback)
    apiServer.set_request_handler([this](const std::string& req) -> std::string {
        return this->handle_action(req);
    });
}
Server::~Server(){
    if (server_fd >= 0) {
        ::close(server_fd);
        server_fd = -1;
    }

} 
std::string Server::handle_action(const std::string &req){
    std::string msg;

    if(req=="getconfig")
        msg = config_to_json();
    else
        modify_config_from_json(req);
    return msg;
}
std::string Server::config_to_json(){
    std::string rep_json="[{";
    rep_json += "\'host\':\'"+host+"\',";
    rep_json += "\'config_file_path\':\'"+config_file_path+"\'";
    return rep_json+"}]" ;
} 
int Server::modify_config_from_json(const std::string &json){
    if(json=="dad")
        return 1;
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
        apiServer.start(server_fd);
    });

    // 2. Le thread principal continue son travail en parallèle
    std::cout << "[Thread Principal] Fait autre chose pendant ce temps...\n";

    // 3. Attente bloquante : le main attend que 'monThread' ait fini avant de continuer
    monThread.join();
}