#include "../includes/server.hpp"

Server::Server(int argc, char* argv[]) {
    this->configFilePath = "config/config.ini"; // Default configuration path.
    this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    this->opt = 1;
    loadConfig(configFilePath);
    this->apiServer = ApiServer(dbPath);

    // Save the executable path and arguments.
    executablePath = argv[0];
    for (int i = 0; i < argc; ++i) {
        args.push_back(argv[i]);
    }

    // Register the server callback with the API layer.
    apiServer.setRequestHandler([this](const std::string& req, const std::string& body) -> std::string {
        return this->handleAction(req, body);
    });
}
Server::~Server(){
    if (server_fd >= 0) {
        ::close(server_fd);
        server_fd = -1;
    }

} 
std::string Server::handleAction(const std::string &req, const std::string &body) {
    std::string msg="OK";

    if(req=="getconfig")
        return configToJson();
    else if(req=="modifyconfig") { 
        if (modifyConfigFromJson(body) != 0)
            return "Invalid configuration";
        
        this->requestRestart();  // Trigger an asynchronous restart.

        return "Configuration updated successfully. Restarting the server to apply changes...";
    }
    else
        return "Invalid request";
}

void Server::requestRestart() {
    // Run the restart in a separate thread so the request can finish,
    // flush the HTTP socket, and return 200 OK to the client.
    std::thread([this]() {
        this->executeRestart();
    }).detach();
}
void Server::executeRestart() {
    // Give the HTTP response time to leave the process.
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "[SERVER] Redémarrage du processus en cours..." << std::endl;

    // Rebuild the char* array expected by the POSIX execv call.
    std::vector<char*> rawArgs;
    for (auto& arg : args) {
        rawArgs.push_back(arg.data());
    }
    rawArgs.push_back(nullptr); // execv must end with a null pointer.

    // Replace the current process.
    execv(executablePath.c_str(), rawArgs.data());

    // Reaching this point means the call failed.
    perror("[SERVER] Échec critique de execv");
}

std::string Server::configToJson(){
    std::string rep_json="[{";
    rep_json += "\"host\":\""+host+"\",";
    rep_json += "\"port\":" + std::to_string(port) + ",";
    rep_json += "\"dbPath\":\""+dbPath+"\"";
    return rep_json+"}]" ;
}

int Server::modifyConfigFromJson(const std::string &json){
    // Example: [{"host":"127.0.0.1","port":8080,"dbPath":"../data/employees.db"}]
    std::cout << "modifying ...." << std::endl;
    
    std::string s = IniParser::trim(json);

    if (s.size() < 2 || s.front() != '[' || s.back() != ']')
        return 1;

    std::string inner = IniParser::trim(s.substr(1, s.size() - 2));

    if( inner.empty() || inner.front() != '{' || inner.back() != '}' )
        return 1;

    std::string obj = IniParser::trim(inner.substr(1, inner.size() - 2));

    std::string new_host = IniParser::getField(obj, "host");
    int new_port = 0;
    try {
        new_port = std::stoi(IniParser::getField(obj, "port"));
    } catch (const std::exception&) {
        return 1;
    }
    std::string new_dbPath = IniParser::getField(obj, "dbPath");

    // Replace the configuration in the file; restart to apply it.
    std::fstream fichier;
    fichier.open(configFilePath.c_str(), std::ios::in | std::ios::out);
    if (fichier.is_open()) {
        std::cout << "Fichier ouvert pour modification dont chemin: " << configFilePath << std::endl;
        std::string ligne;
        std::string contenu;
        fichier.seekg(0);
        while (getline(fichier, ligne)) {
            if (ligne.find("host =") != std::string::npos)
                contenu += "host = " + new_host + "\n";
            else if (ligne.find("port =") != std::string::npos)
                contenu += "port = " + std::to_string(new_port) + "\n";
            else if (ligne.find("dbPath =") != std::string::npos)
                contenu += "dbPath = " + new_dbPath + "\n";
            else
                contenu += ligne + "\n";
        }
        fichier.clear();
        fichier.seekp(0);
        fichier.write(contenu.c_str(), contenu.length());
        fichier.close();
    } 
    else
        std::cerr << "Impossible d'ouvrir le fichier en écriture." << std::endl;

    return 0;
} 
void Server::loadConfig(const std::string& file_path) {
    auto config = IniParser::parse(file_path);

    for (const auto& section : config) {
        if (section.nom == "Server") {
            if (auto var = section.getVariable("port")) {
                try {
                    this->port = std::stoi(var->valeur);
                } catch (...) {
                    std::cerr << "[loadConfig] Invalid port value: " << var->valeur << std::endl;
                }
            }
            if (auto var = section.getVariable("host"))
                this->host = var->valeur;
        }
        else if (section.nom == "Database") {
            if (auto var = section.getVariable("db_path"))
                this->dbPath = var->valeur;
        }
        else if (section.nom == "Logging") {
            if (auto var = section.getVariable("log_file"))
                this->logFile = var->valeur;
        }
    }

    std::cout << "[Server] Config loaded: host=" << (host.empty()?"(none)":host) << " port=" << port << " dbPath=" << (dbPath.empty()?"(none)":dbPath) << " logFile=" << (logFile.empty()?"(none)":logFile) << std::endl;
}
void Server::start() {
    // Reuse the port to avoid "Address already in use" errors.
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) { // server_fd is the server socket.
        std::cerr << "Bind failed" << std::endl;
        close(server_fd);
        return;
    }

    if (listen(server_fd, 10) < 0) {
        std::cerr << "Listen failed" << std::endl;
        close(server_fd);
        return;
    }

    std::cout << "\n============= Server started at http://" << host << ":" << port << " ==============" << std::endl;

    std::thread monThread([this]() {
        std::cout << "[Thread API] Démarrage du serveur API..." << std::endl;
        apiServer.start(server_fd);
    });

    // The main thread continues while the API thread runs.
    std::cout << "[Thread Server] Attente d'une demande du thread API..." << std::endl;

    // Block until the API thread finishes.
    monThread.join();
}