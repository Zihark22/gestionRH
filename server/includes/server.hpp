#include <iostream>
#include <sys/socket.h>

#include "apiserver.hpp"
#include "iniparser.hpp"

class Server {
private:
    int server_fd;
    sockaddr_in address{};
    int opt;
    int port;
    std::string config_file_path= "config.ini"; // Valeur par défaut
    std::string host;
    std::string db_path;
    std::string log_file;
    ApiServer apiServer; // Instance de ApiServer pour gérer les requêtes API

    // Chargement de la configuration depuis le fichier config.ini
    void load_config(const std::string& file_path);

public:
    Server();

    // lancement du serveur
    void start();
};