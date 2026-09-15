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
    // Constructeur qui initialise le serveur avec les arguments de la ligne de commande
    Server(int argc, char* argv[]);

    ~Server();

    // Règle des 5 : Interdire la copie pour éviter les doubles fermetures de socket
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    // Lancement du serveur
    void start();

    // Gestion des requêtes API pour modifier la configuration
    std::string handleAction(const std::string &req, const std::string &body); // gestion des requetes API pour modifier la config

private:
    int server_fd;                              //< socket
    sockaddr_in address{};                      //< stock l'adresse de la socket
    int opt;                                    //< option de config de la socket
    int port;                                   //< port du serveur
    std::string configFilePath= "config.ini"; //< chemin du fichier de configuration
    std::string host;                           //< hôte du serveur
    std::string dbPath;                        //< chemin de la base de données
    std::string logFile;                       //< chemin du fichier de journalisation
    ApiServer apiServer;                        //< Instance de ApiServer pour gérer les requêtes API
    std::string executablePath;               //< Chemin de l'exécutable
    std::vector<std::string> args;            //< Arguments passés à l'exécutable

    // Chargement de la configuration depuis le fichier config.ini
    void loadConfig(const std::string& file_path);

    // Convertit la configuration actuelle en format JSON pour l'API
    std::string configToJson();

    // Modifie le fichier de configuration actuelle à partir d'une chaîne JSON reçue via l'API
    int modifyConfigFromJson(const std::string &json);

    // Redémarrage du serveur
    void executeRestart();

    // Demande de redémarrage du serveur (asynchrone)
    void requestRestart();

};