#ifndef APISERVER_HPP
#define APISERVER_HPP

#include <iostream>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <functional>

#include "dbhandler.hpp"

class ApiServer {

public:
    ApiServer() = default;
    ApiServer(const std::string &db_path);

    // Signature du handler : prend les données de la requête et retourne le résultat à renvoyer au client
    using RequestHandler = std::function<std::string(const std::string& request, const std::string& body)>;

    void set_request_handler(RequestHandler handler);

    void start(int server_fd); // Démarrage du serveur

private:
    std::string messageError;       // Message de retour en json 
    int response_status_code = 200; // Code de statut HTTP par défaut
    std::string response_msg;       // Message sur le status code
    bool authentication_ok = false; // pour implémenter l'authentification plus tard
    DBhandler db_handler;           // 
    std::string body="";                 // body des requêtes HTTP
    RequestHandler m_handler;

    // méthodes
    void parse_request_http(std::string request, const int client_fd); // décompose la requête API en Verbe, endpoint et body (apres token d'authentification)
    void execute_request(const std::string &method, const std::string &endpoint, const int client_fd); // en fonction du verbe et endpoint appelle la méthode associée à la requête SQL et construit la réponse

};

#endif