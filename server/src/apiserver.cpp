#include "../includes/apiserver.hpp"

ApiServer::ApiServer(const std::string &dbPath) {
    dbHandler = DBhandler(dbPath);
}

void ApiServer::setRequestHandler(RequestHandler handler) {
    myHandler = handler;
}

void ApiServer::start(int server_fd) {
    // Boucle principale pour accepter les connexions entrantes
    while (_isRunning) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd < 0) 
            continue;

        char buffer[2048] = {0};
        read(client_fd, buffer, sizeof(buffer) - 1);

        std::string request(buffer);
        std::cout << "\n============ Requête reçue ============\n" << request << "\n" << std::endl;
        parseRequestHttp(request, client_fd);

        close(client_fd);
    }
}

void ApiServer::parseRequestHttp(std::string request, const int client_fd) {
    std::string method;
    std::string endpoint;
    
    // Extraction de la méthode HTTP (GET, POST, DELETE, PUT, etc.)
    size_t space_pos = request.find(" ");
    if (space_pos != std::string::npos)
        method = request.substr(0, space_pos);
    
    // Extraction de l'endpoint (entre le premier et le deuxième espace)
    size_t first_space = request.find(" ");
    size_t second_space = request.find(" ", first_space + 1);
    
    if (first_space != std::string::npos && second_space != std::string::npos)
        endpoint = request.substr(first_space + 1, second_space - first_space - 1);

    // Extraction du body si présent
    size_t content_pos = request.find("Content-Length:");
    if(content_pos != std::string::npos) {
        size_t body_pos_deb = request.find("[", content_pos+1);
        size_t body_pos_fin = request.find("]", body_pos_deb+1);
        if(body_pos_deb != std::string::npos and body_pos_fin != std::string::npos)
            body = request.substr(body_pos_deb, body_pos_fin);
    }
    else
        body = "";


    /*
        code to extract authentification token in user table for future feature and check in DB if user registered
    */
    authenticationOk = true; // remove after implementation of login function
    
    if(authenticationOk)
        executeRequest(method, endpoint, client_fd);
    else
        std::cout << "Authentification failed" << std::endl; 

} 

void ApiServer::executeRequest(const std::string &method, const std::string &endpoint, const int client_fd) {
    std::string repbody;
    std::string responseHTTP;
    std::string repsponseGlobal; // informations à retourner différentes d'une liste d'employés (ici la config ou id du nouvel employé)

    dbHandler.openDB();
    std::cout << "Action demandée par le client : ";

    handleRequest(method, endpoint, repsponseGlobal); // appelle la fonction associée au endpoint + méthode et retourne la réponse

    // Construction de la réponse HTTP
    if(responseStatusCode != 200) { // Si le code de statut n'est pas 200, on renvoie un message d'erreur
        repbody = "{\"error\": \""+ messageError +"\"}";
        responseHTTP = 
            "HTTP/1.1 "+ std::to_string(responseStatusCode) + " " + responseMsg +"\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: " + std::to_string(repbody.length()) + "\r\n\r\n" + 
            repbody;
    }
    else {  // si le code de statut est 200, on renvoie le JSON de la base de données ou la réponse du serveur
        if(repsponseGlobal.empty())
            repbody = dbHandler.formatterJson(); // JSON de sortie de la base de données
        else
            repbody = repsponseGlobal; // autre réponse du serveur (ex: config)

        responseHTTP = 
            "HTTP/1.1 "+ std::to_string(responseStatusCode) + " " + responseMsg +"\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Content-TypexecuteRequeste: application/json\r\n"
            "Content-Length: " + std::to_string(repbody.length()) + "\r\n\r\n" + 
            repbody;
    }

    write(client_fd, responseHTTP.c_str(), responseHTTP.length());
    dbHandler.closeDB();
} 


RouteMatch ApiServer::parseRoute(const std::string& uri) {
    // Table de correspondance pour les routes statiques de base
    static const std::unordered_map<std::string, EndpointAction> routeTable = {
        {"/api/employees", EndpointAction::EMPLOYEES_COLLECTION},
        {"/api/config",    EndpointAction::CONFIG}
    };

    // 1. Recherche directe dans la map
    auto it = routeTable.find(uri);
    if (it != routeTable.end()) {
        return {it->second, std::nullopt};
    }

    // 2. Traitement du cas dynamique /api/employees/:id
    const std::string prefix = "/api/employees/";
    if (uri.rfind(prefix, 0) == 0) { // Démarre par "/api/employees/"
        std::string idStr = uri.substr(prefix.length());
        try {
            int id = std::stoi(idStr);
            return {EndpointAction::EMPLOYEE_BY_ID, id};
        } catch (...) {
            return {EndpointAction::UNKNOWN, std::nullopt}; // ID non numérique
        }
    }

    return {EndpointAction::UNKNOWN, std::nullopt};
}

void ApiServer::handleRequest(const std::string& method, const std::string& uri, std::string &responseGlobal) {
    RouteMatch route = parseRoute(uri);
    int result = -1;
    switch (route.action) {
        case EndpointAction::EMPLOYEES_COLLECTION:
            if (method == "GET") {
                // Traiter la requête GET /api/employees
                std::cout << "Obtenir employés" << std::endl;

                // commande
                result = dbHandler.getAllEmployees();

                // reponse en fonction du resultat
                if(result == -1) {
                    messageError = "Erreur lors de la récupération des employées";
                    responseStatusCode = 500;
                    responseMsg = "Internal Server Error";
                }
                else {
                    responseStatusCode = 200;
                    responseMsg = "OK";
                }
            }
            else if (method == "POST") {
                // Traiter la requête POST
                std::cout << "ajout employé" << std::endl;

                // commande
                Employee e(this->body);
                result = dbHandler.addEmployee(e);

                // reponse en fonction du resultat
                if(result == -1) {
                    messageError = "Erreur lors de l'ajout d'un employé";
                    responseStatusCode = 500;
                    responseMsg = "Internal Server Error";
                }
                else {
                    responseStatusCode = 200;
                    responseMsg = "OK";
                    responseGlobal = "[{\"id\": "+std::to_string(result)+"}]";
                }
            }
            else {
                std::cerr << "-> Error 405 method not allowed\n";
                messageError = "La méthode n'est pas autorisée sur cet endpoint";
                responseStatusCode = 405;
                responseMsg = "Method Not Allowed";   
            } 
            break;

        case EndpointAction::EMPLOYEE_BY_ID:
            if (method == "GET") {
                // Traiter la requête GET /api/employees
                std::cout << "Obtenir employé dont ID = " << route.id.value() << std::endl;

                // commande
                result = dbHandler.getEmployee(route.id.value());

                // reponse en fonction du resultat
                if(result == 0) {
                    int nbE = dbHandler.countEmployees();
                    if(nbE<1){
                        responseStatusCode = 404;
                        responseMsg = "Not found";
                        messageError = "L'employé avec ID = " + std::to_string(route.id.value()) + " n'est pas dans la BDD"; 
                    }
                    else {
                        responseStatusCode = 200;
                        responseMsg = "OK";
                    }
                }
                else {
                    messageError = "Erreur lors de la récupération de l'employé"; 
                    responseStatusCode = 500;
                    responseMsg = "Internal Server Error";
                }
            }
            else if (method == "DELETE") {
                // Traiter la requête DELETE
                std::cout << "supprimer employé dont ID = " << route.id.value() << std::endl;

                // commande
                result = dbHandler.deleteEmployee(route.id.value());

                // reponse en fonction du resultat
                if(result == -1) {
                    messageError = "Erreur lors de la suppression de l'employé";
                    responseStatusCode = 500;
                    responseMsg = "Internal Server Error";
                }
                else {
                    responseStatusCode = 200;
                    responseMsg = "OK";
                }
            }
            else if (method == "PUT") {
                // Traiter la requête PUT
                std::cout << "modifier employé dont ID = " << route.id.value() << std::endl;

                 // commande
                Employee e(this->body);
                result = dbHandler.modifyEmployee(e, std::to_string(route.id.value()));

                // reponse en fonction du resultat
                if(result == -1) {
                    messageError = "Erreur lors de la modification de l'employé";
                    responseStatusCode = 500;
                    responseMsg = "Internal Server Error";
                }
                else {                    
                    responseStatusCode = 200;
                    responseMsg = "OK";
                }
            }
            else {
                std::cerr << "-> Error 405 method not allowed\n";
                messageError = "La méthode n'est pas autorisée sur cet endpoint";
                responseStatusCode = 405;
                responseMsg = "Method Not Allowed";   
            } 
            break;

        case EndpointAction::CONFIG:
            if (method == "GET") {
                // Traiter la requête GET /api/employees
                std::cout << "Obtenir config" << std::endl;

                // commande en appelant le serveur
                if (myHandler) {
                    responseGlobal = myHandler("getconfig", "");
                    std::cout << "reponse : " << responseGlobal << std::endl;
                    result = 0;
                } 
                else {
                    responseGlobal = "ERROR 500: No handler";
                    result = -1;
                }

                // reponse en fonction du resultat
                if(result == -1) {
                    messageError = "Erreur lors de la récupération de la config";
                    responseStatusCode = 500;
                    responseMsg = "Internal Server Error";
                }
                else {
                    responseStatusCode = 200;
                    responseMsg = "OK";
                }
            }
            else if (method == "PUT") {
                // Traiter la requête GET /api/employees
                std::cout << "Modifier config" << std::endl;

                // envoi de la requete au serveur pour modifier le fichier config.ini
                if (myHandler) {
                    responseGlobal = myHandler("modifyconfig",this->body);
                    std::cout << "reponse : " << responseGlobal << std::endl;
                    result = 0;
                } 
                else {
                    responseGlobal = "ERROR 500: No handler";
                    result = -1;
                }

                // reponse en fonction du resultat
                if(result == -1) {
                    messageError = "Erreur lors de la modification de la config";
                    responseStatusCode = 500;
                    responseMsg = "Internal Server Error";
                }
                else {
                    responseStatusCode = 200;
                    responseMsg = "Ok";
                }
            }
            break;

        case EndpointAction::UNKNOWN: // Route de secours (404 Not Found)
            messageError = "Route invalide";
            responseStatusCode = 404;
            responseMsg = "Not Found";
            std::cerr << "Route non reconnue : " << std::endl;
        default:  
            std::cerr << "Le endpoint n'a pas été parser correctement !" << std::endl;
            break;
    }
}