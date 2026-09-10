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

    // Constructeur qui initialise le gestionnaire de base de données avec le chemin fourni
    ApiServer(const std::string &db_path);

    // Signature du handler : prend les données de la requête et retourne le résultat à renvoyer au client
    using RequestHandler = std::function<std::string(const std::string& request, const std::string& body)>;

    // Permet de définir le handler pour traiter les requêtes spécifiques
    void set_request_handler(RequestHandler handler);

    // Démarrage de la communication API
    void start(int server_fd); 

private:
    std::string messageError;       //< Message de retour en json 
    int response_status_code = 200; //< Code de statut HTTP par défaut
    std::string response_msg;       //< Message sur le status code
    bool authentication_ok = false; //< pour implémenter l'authentification plus tard
    std::string body="";            //< body des requêtes HTTP
    DBhandler db_handler;           //< Handler pour interagir avec la base de données
    RequestHandler m_handler;       //< Handler pour traiter les requêtes spécifiques

    
    ///////// Méthodes /////////

    /**
     * @brief Décompose la requête HTTP pour extraire la méthode, l'endpoint et le corps de la requête.
     * 
     * @param request La chaîne de caractères représentant la requête HTTP.
     * @param client_fd Le descripteur de fichier du client.
     */
    void parse_request_http(std::string request, const int client_fd);
    
    /**
     * @brief Exécute la requête correspondante en fonction de sa méthode et de son endpoint puis construit la réponse HTTP à renvoyer au client.
     * 
     * @param method La méthode HTTP de la requête.
     * @param endpoint L'endpoint de la requête.
     * @param client_fd Le descripteur de fichier du client.
     */
    void execute_request(const std::string &method, const std::string &endpoint, const int client_fd);

};

#endif