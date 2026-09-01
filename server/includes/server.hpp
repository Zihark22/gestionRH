#pragma once // inclusion une seule fois 

#include <iostream>
#include <sys/socket.h>
#include <thread>

#include "apiserver.hpp"
#include "iniparser.hpp"

class Server {
    
public:
    Server();
    ~Server();

    // Règle des 5 : Interdire la copie pour éviter les doubles fermetures de socket
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    void start();  // lancement du serveur
    std::string handle_action(const std::string &req, const std::string &body); // gestion des requetes API pour modifier la config

private:
    int server_fd; // socket
    sockaddr_in address{}; // store the address of the socket
    int opt;  // option de config de la socket
    int port; 
    std::string config_file_path= "config.ini"; // Valeur par défaut
    std::string host;
    std::string db_path;
    std::string log_file;
    ApiServer apiServer; // Instance de ApiServer pour gérer les requêtes API

    // Chargement de la configuration depuis le fichier config.ini
    void load_config(const std::string& file_path);
    std::string config_to_json();
    int modify_config_from_json(const std::string &json);

};