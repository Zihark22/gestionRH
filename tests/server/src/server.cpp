#include "../includes/server.hpp"
#include "../includes/dbhandler.hpp"
#include "../includes/IniParser.hpp"

Server::Server(const std::string& configFilePath) {
    this->configFilePath = configFilePath;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Erreur création socket" << std::endl;
        exit(EXIT_FAILURE);
    }
    this->opt = 1;
    loadConfig(configFilePath);
}

void Server::loadConfig(const std::string& configFilePath) {
    auto config = IniParser::parse(configFilePath);

    for (const auto& section : config) {
        if (section.nom == "Server") {
            if (auto var = section.getVariable("port")) {
                try {
                    this->port = std::stoi(var->valeur);
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
void Server::startServer() {
    // Réutilisation du port pour éviter l'erreur "Address already in use"
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
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

    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd < 0) continue;

        char buffer[2048] = {0};
        read(client_fd, buffer, sizeof(buffer) - 1);

        std::string request(buffer);
        std::cout << "\n--- Requête reçue ---\n" << request << "\n" << std::endl;

        // Route GET : Obtenir la liste des collaborateurs
        if (request.rfind("GET /api/collaborateurs", 0) == 0 || request.find("GET /api/collaborateurs") != std::string::npos) {

            // On crée une instance de DBhandler pour interagir avec la base de données
            DBhandler dbHandler(this->db_path);
            dbHandler.getAllCollaborateurs();
            std::string jsonOutput = dbHandler.formatter_JSON(); // JSON de sortie de la base de données
            std::string body = jsonOutput;
            
            // std::string body = "[{\"id\": 1, \"nom\": \"Dupont\", \"prenom\": \"Jean\", \"poste\": \"Ingénieur C++\"}]";
            std::string response = 
                "HTTP/1.1 200 OK\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Content-Type: application/json\r\n"
                "Content-Length: " + std::to_string(body.length()) + "\r\n\r\n" + 
                body;

            write(client_fd, response.c_str(), response.length());
        } 
        // Route de secours (404 Not Found)
        else {
            std::string body = "{\"error\": \"Route non trouvée\"}";
            std::string response = 
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: application/json\r\n"
                "Content-Length: " + std::to_string(body.length()) + "\r\n\r\n" + 
                body;

            write(client_fd, response.c_str(), response.length());
        }

        close(client_fd);
    }
}