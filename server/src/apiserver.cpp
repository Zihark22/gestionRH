#include "../includes/apiserver.hpp"

ApiServer::ApiServer() {
    // Initialisation si nécessaire
    db_handler = DBhandler("");
}
ApiServer::ApiServer(const string &db_path) {
    // Initialisation si nécessaire
    db_handler = DBhandler(db_path);
}
ApiServer::~ApiServer() {
    // Libération des ressources si nécessaire
    
}

void ApiServer::start(int server_fd) {
    // Implémentez la logique pour démarrer le serveur API ici
    // Vous pouvez utiliser le socket server_fd pour écouter les requêtes entrantes
    // et interagir avec la base de données en utilisant db_path
    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd < 0) continue;

        char buffer[2048] = {0};
        read(client_fd, buffer, sizeof(buffer) - 1);

        string request(buffer);
        cout << "\n============ Requête reçue ============\n" << request << "\n" << endl;
        parse_request_http(request, client_fd);

        close(client_fd);
    }
}

void ApiServer::parse_request_http(string request, const int client_fd) {
    /**
     * @brief Extrait la méthode HTTP et l'endpoint de la requête
     *  
     */
    // cout << "start parsing" << endl;
    string method;
    string endpoint;
    
    // Extraction de la méthode HTTP (GET, POST, DELETE, PUT, etc.)
    size_t space_pos = request.find(" ");
    if (space_pos != string::npos) {
        method = request.substr(0, space_pos);
    }
    
    // Extraction de l'endpoint (entre la première et la deuxième espace)
    size_t first_space = request.find(" ");
    size_t second_space = request.find(" ", first_space + 1);
    
    if (first_space != string::npos && second_space != string::npos) {
        endpoint = request.substr(first_space + 1, second_space - first_space - 1);
    }

    // Extraction du body si présent
    size_t content_pos = request.find("Content-Length:");
    if(content_pos != string::npos) {
        size_t body_pos_deb = request.find("[", content_pos+1);
        size_t body_pos_fin = request.find("]", body_pos_deb+1);
        body = request.substr(body_pos_deb, body_pos_fin);
    }
    else
        body = "";

    /*
        code to check authentification in user table for future feature
    */
    authentication_ok = true; // remove after implementation of login function
    
    if(authentication_ok)
        execute_request(method, endpoint, client_fd);

} 

void ApiServer::execute_request(const string &method, const string &endpoint, const int client_fd) {
    string jsonOutput; // JSON de sortie de la base de données
    string body;
    string response;
    int result=-1;

    db_handler.open_db();
    cout << "Action demandée par le client : ";

    // si le endpoint employees est dans la requete (ex: ID)
    if (endpoint.rfind("/api/employees", 0) == 0) {
        string id = endpoint.substr(string("/api/employees").size());
        if(id[0]=='/') id = id.substr(1); // pour gérer avec / ou sans à la fin
        
        // si ID à la fin du endpoint
        if (!id.empty() && id.find_first_not_of("0123456789") == string::npos) {
        
            if (method == "GET") {
                // Traiter la requête GET /api/employees
                cout << "Obtenir employé dont ID = " << id << endl;

                // commande
                result = db_handler.get_employee(atoi(id.c_str()));

                // reponse en fonction du resultat
                if(result == 0) {
                    int nbE = db_handler.count_employees();
                    if(nbE<1){
                        response_status_code = 404;
                        response_msg = "Not found";
                        messageError = "L'employé avec ID = "+id+" n'est pas dans la BDD"; 
                    
                    }
                    else {
                        response_status_code = 200;
                        response_msg = "OK";
                    }
                }
                else {
                    messageError = "Erreur lors de la récupération de l'employé"; 
                    response_status_code = 500;
                    response_msg = "Internal Server Error";
                }
            }
            else if (method == "DELETE") {
                // Traiter la requête DELETE
                cout << "supprimer employé dont ID = " << id << endl;

                // commande
                result = db_handler.delete_employee(atoi(id.c_str()));

                // reponse en fonction du resultat
                if(result != 0) {
                    messageError = "Erreur lors de la suppression de l'employé";
                    response_status_code = 500;
                    response_msg = "Internal Server Error";
                }
                else {
                    response_status_code = 200;
                    response_msg = "OK";
                }
            }
            else if (method == "PUT") {
                // Traiter la requête PUT
                cout << "modifier employé dont ID = " << id << endl;

                 // commande
                Employee e(this->body);
                result = db_handler.modify_employee(e, id);

                // reponse en fonction du resultat
                if(result == 0) {                    
                    response_status_code = 200;
                    response_msg = "OK";
                }
                else {
                    messageError = "Erreur lors de la modification de l'employé";
                    response_status_code = 500;
                    response_msg = "Internal Server Error";
                }
            }
            else if (method == "POST") {
                messageError = "La méthode POST n'est pas autorisée sur cet endpoint";
                response_status_code = 405;
                response_msg = "Method Not Allowed";
            }
            else{
                cout << "méthode non supportée " << endl;
                messageError = "Requête non implémentée";
                response_status_code = 501;
                response_msg = "Not implemented";
            } 
        }
        // si la requete concerne tout les employes
        else {
            if (method == "GET") {
                // Traiter la requête GET /api/employees
                cout << "Obtenir employés" << endl;

                // commande
                result = db_handler.get_all_employees();

                // reponse en fonction du resultat
                if(result != 0) {
                    messageError = "Erreur lors de la récupération des employées";
                    response_status_code = 500;
                    response_msg = "Internal Server Error";
                }
                else {
                    response_status_code = 200;
                    response_msg = "OK";
                }
            }
            else if (method == "POST") {
                // Traiter la requête POST
                cout << "ajout employé" << endl;

                // commande
                Employee e(this->body);
                result = db_handler.add_employee(e);

                // reponse en fonction du resultat
                if(result != 0) {
                    messageError = "Erreur lors de l'ajout d'un employé";
                    response_status_code = 500;
                    response_msg = "Internal Server Error";
                }
                else {
                    response_status_code = 200;
                    response_msg = "OK";
                }
            }
            else if (method == "PUT") {
                messageError = "La méthode PUT n'est pas autorisée sur cet endpoint";
                response_status_code = 405;
                response_msg = "Method Not Allowed";
                
            }
            else if (method == "DELETE") {
                messageError = "La méthode DELETE n'est pas autorisée sur cet endpoint";
                response_status_code = 405;
                response_msg = "Method Not Allowed";
                
            }
            else {
                cout << "méthode non supportée " << endl;
                messageError = "Requête non implémentée";
                response_status_code = 501;
                response_msg = "Not implemented";
            } 
        } 
    }
    // Route de secours (404 Not Found)
    else {
        messageError = "Route non trouvée";
        response_status_code = 404;
        response_msg = "Not Found";
    }


    if(response_status_code != 200) {
        body = "{\"error\": \""+ messageError +"\"}";
        response = 
            "HTTP/1.1 "+ to_string(response_status_code) + " " + response_msg +"\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: " + to_string(body.length()) + "\r\n\r\n" + 
            body;
    }
    else {
        body = db_handler.formatter_JSON(); // JSON de sortie de la base de données
        response = 
            "HTTP/1.1 "+ to_string(response_status_code) + " " + response_msg +"\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Content-Typexecute_requeste: application/json\r\n"
            "Content-Length: " + to_string(body.length()) + "\r\n\r\n" + 
            body;
    }

    write(client_fd, response.c_str(), response.length());
    db_handler.close_db();
} 