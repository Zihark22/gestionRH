#ifndef APISERVER_HPP
#define APISERVER_HPP

#include "dbhandler.hpp"

#include <iostream>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <functional>
#include <unordered_map>
#include <optional>
#include <sstream>

enum class EndpointAction {
    UNKNOWN,
    EMPLOYEES_COLLECTION, // /api/employees (GET tous, POST nouveau)
    EMPLOYEE_BY_ID,       // /api/employees/:id (GET un, PUT, DELETE)
    CONFIG                // /api/config
};

struct RouteMatch {
    EndpointAction action = EndpointAction::UNKNOWN;
    std::optional<int> id; // Rempli si un ID est détecté
};

class ApiServer {

public:
    ApiServer() = default;

    // Constructeur qui initialise le gestionnaire de base de données avec le chemin fourni
    ApiServer(const std::string &dbPath);

    // Signature du handler : prend les données de la requête et retourne le résultat à renvoyer au client
    using RequestHandler = std::function<std::string(const std::string& request, const std::string& body)>;

    // Permet de définir le handler pour traiter les requêtes spécifiques
    void setRequestHandler(RequestHandler handler);

    // Démarrage de la communication API
    void start(int server_fd); 

private:
    std::string messageError;       //< Message de retour en json 
    int responseStatusCode = 200; //< Code de statut HTTP par défaut
    std::string responseMsg;       //< Message sur le status code
    bool authenticationOk = false; //< pour implémenter l'authentification plus tard
    std::string body="";            //< body des requêtes HTTP
    DBhandler dbHandler;           //< Handler pour interagir avec la base de données
    RequestHandler myHandler;       //< Handler pour traiter les requêtes spécifiques
    bool _isRunning = true;         //< Indique si le serveur est en cours d'exécution

    
    ///////// Méthodes /////////

    /**
     * @brief Décompose la requête HTTP pour extraire la méthode, l'endpoint et le corps de la requête.
     * 
     * @param request La chaîne de caractères représentant la requête HTTP.
     * @param client_fd Le descripteur de fichier du client.
     */
    void parseRequestHttp(std::string request, const int client_fd);
    
    /**
     * @brief Exécute la requête correspondante en fonction de sa méthode et de son endpoint puis construit la réponse HTTP à renvoyer au client.
     * 
     * @param method La méthode HTTP de la requête.
     * @param endpoint L'endpoint de la requête.
     * @param client_fd Le descripteur de fichier du client.
     */
    void executeRequest(const std::string &method, const std::string &endpoint, const int client_fd);

    /**
     * @brief Décompose
     * 
     * @param uri 
     */
    RouteMatch parseRoute(const std::string& uri);
      
    /**
     * @brief 
     * 
     * @param method La méthode HTTP de la requête.
     * @param uri    L'URI de la requête.
     * @param rep    
     */
    void handleRequest(const std::string& method, const std::string& uri, std::string &responseGlobal);

};

#endif