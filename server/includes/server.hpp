#pragma once // inclusion une seule fois 

#include "apiserver.hpp"
#include "iniparser.hpp"

#include <iostream>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <chrono>
#include <unistd.h>



class Server {
    
public:
    Server(int argc, char* argv[]);
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
    std::string m_executablePath; // Chemin de l'exécutable
    std::vector<std::string> m_args; // Arguments passés à l'exécutable

    // Chargement de la configuration depuis le fichier config.ini
    void load_config(const std::string& file_path);
    std::string config_to_json();
    int modify_config_from_json(const std::string &json);
    void requestRestart();
    void executeRestart();

};