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
    std::string jsonOutput;
    std::string repbody;
    std::string response;
    int result=-1;
    std::string rep;

    dbHandler.openDB();
    std::cout << "Action demandée par le client : ";

    // count the number of non-digit characters in the endpoint to limit endpoint structure after /api/employees to a maximum of 15 non-digit characters (e.g., /api/employees/123)
    int nbCarNotDigit = 0;
    for (size_t i = 0; i < endpoint.length(); ++i) {
        if (isdigit(endpoint[i])) 
            continue;
        else 
            nbCarNotDigit++;
    }

    // si le endpoint employees est dans la requete (ex: ID)
    if (endpoint.rfind("/api/employees", 0) == 0 && nbCarNotDigit<=15) {
        std::string id = endpoint.substr(std::string("/api/employees").size());
        if(id[0]=='/') 
            id = id.substr(1); // pour gérer avec / ou sans à la fin
        
        // si ID à la fin du endpoint
        if (!id.empty() && id.find_first_not_of("0123456789") == std::string::npos) {
        
            if (method == "GET") {
                // Traiter la requête GET /api/employees
                std::cout << "Obtenir employé dont ID = " << id << std::endl;

                // commande
                result = dbHandler.getEmployee(atoi(id.c_str()));

                // reponse en fonction du resultat
                if(result == 0) {
                    int nbE = dbHandler.countEmployees();
                    if(nbE<1){
                        responseStatusCode = 404;
                        responseMsg = "Not found";
                        messageError = "L'employé avec ID = "+id+" n'est pas dans la BDD"; 
                    
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
                std::cout << "supprimer employé dont ID = " << id << std::endl;

                // commande
                result = dbHandler.deleteEmployee(atoi(id.c_str()));

                // reponse en fonction du resultat
                if(result < 0) {
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
                std::cout << "modifier employé dont ID = " << id << std::endl;

                 // commande
                Employee e(this->body);
                result = dbHandler.modifyEmployee(e, id);

                // reponse en fonction du resultat
                if(result > -1) {                    
                    responseStatusCode = 200;
                    responseMsg = "OK";
                }
                else {
                    messageError = "Erreur lors de la modification de l'employé";
                    responseStatusCode = 500;
                    responseMsg = "Internal Server Error";
                }
            }
            else if (method == "POST") {
                messageError = "La méthode POST n'est pas autorisée sur cet endpoint";
                responseStatusCode = 405;
                responseMsg = "Method Not Allowed";
            }
            else{
                std::cout << "méthode non supportée " << std::endl;
                messageError = "Requête non implémentée";
                responseStatusCode = 501;
                responseMsg = "Not implemented";
            } 
        }
        // si la requete concerne tout les employes
        else {
            if (method == "GET") {
                // Traiter la requête GET /api/employees
                std::cout << "Obtenir employés" << std::endl;

                // commande
                result = dbHandler.getAllEmployees();

                // reponse en fonction du resultat
                if(result < 0) {
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
                if(result > -1) {
                    responseStatusCode = 200;
                    responseMsg = "OK";
                    rep = "[{\"id\": "+std::to_string(result)+"}]";
                }
                else {
                    messageError = "Erreur lors de l'ajout d'un employé";
                    responseStatusCode = 500;
                    responseMsg = "Internal Server Error";
                }
            }
            else if (method == "PUT") {
                messageError = "La méthode PUT n'est pas autorisée sur cet endpoint";
                responseStatusCode = 405;
                responseMsg = "Method Not Allowed";                
            }
            else if (method == "DELETE") {
                messageError = "La méthode DELETE n'est pas autorisée sur cet endpoint";
                responseStatusCode = 405;
                responseMsg = "Method Not Allowed";                
            }
            else {
                std::cout << "méthode non supportée " << std::endl;
                messageError = "Requête non implémentée";
                responseStatusCode = 501;
                responseMsg = "Not implemented";
            } 
        } 
    }
    // si le endpoint config est dans la requete
    else if(endpoint=="/api/config" or endpoint=="/api/config/") {
        if (method == "GET") {
            // Traiter la requête GET /api/employees
            std::cout << "Obtenir config" << std::endl;

            // commande en appelant le serveur
             if (myHandler) {
                rep = myHandler("getconfig", "");
                std::cout << "reponse : " << rep << std::endl;
                result = 0;
            } 
            else {
                rep = "ERROR 500: No handler";
                result = -1;
            }

            // reponse en fonction du resultat
            if(result != 0) {
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
                rep = myHandler("modifyconfig",this->body);
                std::cout << "reponse : " << rep << std::endl;
                result = 0;
            } 
            else {
                rep = "ERROR 500: No handler";
                result = -1;
            }

            // reponse en fonction du resultat
            if(result != 0) {
                messageError = "Erreur lors de la modification de la config";
                responseStatusCode = 500;
                responseMsg = "Internal Server Error";
            }
            else {
                responseStatusCode = 200;
                responseMsg = "rep";
            }
        }
        else {
            std::cout << "méthode non supportée " << std::endl;
            messageError = "Requête non implémentée";
            responseStatusCode = 501;
            responseMsg = "Not implemented";
        } 
    } 
    // Route de secours (404 Not Found)
    else {
        messageError = "Route invalide : " + endpoint;
        responseStatusCode = 404;
        responseMsg = "Not Found";
        std::cout << "Non reconnue : " << endpoint << std::endl;
    }

    // Construction de la réponse HTTP
    if(responseStatusCode != 200) { // Si le code de statut n'est pas 200, on renvoie un message d'erreur
        repbody = "{\"error\": \""+ messageError +"\"}";
        response = 
            "HTTP/1.1 "+ std::to_string(responseStatusCode) + " " + responseMsg +"\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: " + std::to_string(repbody.length()) + "\r\n\r\n" + 
            repbody;
    }
    else {  // si le code de statut est 200, on renvoie le JSON de la base de données ou la réponse du serveur
        if(rep.empty())
            repbody = dbHandler.formatterJson(); // JSON de sortie de la base de données
        else
            repbody = rep; // autre réponse du serveur (ex: config)

        response = 
            "HTTP/1.1 "+ std::to_string(responseStatusCode) + " " + responseMsg +"\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Content-TypexecuteRequeste: application/json\r\n"
            "Content-Length: " + std::to_string(repbody.length()) + "\r\n\r\n" + 
            repbody;
    }

    write(client_fd, response.c_str(), response.length());
    dbHandler.closeDB();
} 