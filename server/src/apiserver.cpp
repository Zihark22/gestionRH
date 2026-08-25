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

        std::string request(buffer);
        std::cout << "\n============ Requête reçue ============\n" << request << "\n" << std::endl;
        parse_request_http(request, client_fd);
/*
        // Route GET : Obtenir la liste des collaborateurs
        if( (request.rfind("GET /api/employees", 0) == 0 || request.find("GET /api/employees") != std::string::npos) and authentication_ok) {

            // On crée une instance de DBhandler pour interagir avec la base de données
            DBhandler db_handler(db_path);
            int result = db_handler.get_all_employees();
            if(result != 0) {
                std::string body = "{\"error\": \"Erreur lors de la récupération des employees\"}";
                std::string response = 
                    "HTTP/1.1 500 Internal Server Error\r\n"
                    "Content-Type: application/json\r\n"
                    "Content-Length: " + std::to_string(body.length()) + "\r\n\r\n" + 
                    body;

                write(client_fd, response.c_str(), response.length());
                close(client_fd);
                continue;
            }
            std::string jsonOutput = db_handler.formatter_JSON(); // JSON de sortie de la base de données
            std::string body = jsonOutput;
            
            std::string response = 
                "HTTP/1.1 200 OK\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Content-Type: application/json\r\n"
                "Content-Length: " + std::to_string(body.length()) + "\r\n\r\n" + 
                body;

            write(client_fd, response.c_str(), response.length());
        } 
        // Route de secours (404 Not Found)
        else {
            std::string body = "{\"error\": \"Route non trouvée\"}";
            std::string response = 
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: application/json\r\n"
                "Content-Length: " + std::to_string(body.length()) + "\r\n\r\n" + 
                body;

            write(client_fd, response.c_str(), response.length());
        }
*/
        close(client_fd);
    }
}

void ApiServer::parse_request_http(std::string request, const int client_fd) {
    /**
     * @brief Extrait la méthode HTTP et l'endpoint de la requête
     *  
     */
    // std::cout << "start parsing" << std::endl;
    std::string method;
    std::string endpoint;
    
    // Extraction de la méthode HTTP (GET, POST, DELETE, PUT, etc.)
    size_t space_pos = request.find(" ");
    if (space_pos != std::string::npos) {
        method = request.substr(0, space_pos);
    }
    
    // Extraction de l'endpoint (entre la première et la deuxième espace)
    size_t first_space = request.find(" ");
    size_t second_space = request.find(" ", first_space + 1);
    
    if (first_space != std::string::npos && second_space != std::string::npos) {
        endpoint = request.substr(first_space + 1, second_space - first_space - 1);
    }

    
    /*
        code to check authentification
    */
    authentication_ok = true; // remove after implementation of login function
    // std::cout << endpoint << std::endl;
    if(authentication_ok)
        execute_request(method, endpoint, client_fd);

} 

void ApiServer::execute_request(const std::string &method, const std::string &endpoint, const int client_fd) {
    std::string sql_command = "";
    std::string jsonOutput; // JSON de sortie de la base de données
    std::string body;
    std::string response;
    int result=-1;

    db_handler.open_db();
    cout << "Action demandée par le client : ";


    // si le endpoint employees est dans la requete (ex: ID)
    if (endpoint.rfind("/api/employees", 0) == 0) {
        std::string id = endpoint.substr(std::string("/api/employees").size());
        if(id[0]=='/') id = id.substr(1); // pour gérer avec / ou sans à la fin
        
        // si ID à la fin du endpoint
        if (!id.empty() && id.find_first_not_of("0123456789") == std::string::npos) {
        
            if (method == "GET") {
                // Traiter la requête GET /api/employees
                std::cout << "Obtenir employé" << std::endl;

                // commande
                result = db_handler.get_employee(atoi(id.c_str()));

                // reponse en fonction du resultat
                if(result != 0) {
                    messageError = "Erreur lors de la récupération de l'employé";
                    response_status_code = 500;
                    response_msg = "Internal Server Error";
                }
                else {
                    response_status_code = 200;
                    response_msg = "OK";
                }
            }
            else if (method == "DELETE") {
                // Traiter la requête DELETE
                std::cout << "supprimer employé" << std::endl;
            }
            else if (method == "POST") {
                // Traiter la requête POST
                std::cout << "ajout employé" << std::endl;
            }
            else if (method == "PUT") {
                // Traiter la requête PUT
                std::cout << "modifier employé" << std::endl;
            }
            else{
                std::cout << "méthode non supportée" << std::endl;
            } 

            std::cout << "\tID numérique: " << id << std::endl;
        }
        // si la requete concerne tout les employes
        else {
            if (method == "GET") {
                // Traiter la requête GET /api/employees
                std::cout << "Obtenir employés" << std::endl;

                // commande
                result = db_handler.get_all_employees();

                // reponse en fonction du resultat
                if(result != 0) {
                    messageError = "Erreur lors de la récupération des employees";
                    response_status_code = 500;
                    response_msg = "Internal Server Error";
                }
                else {
                    response_status_code = 200;
                    response_msg = "OK";
                }
            }
            else if (method == "DELETE") {
                // Traiter la requête DELETE
                std::cout << "supprimer employés" << std::endl;
            }
            else if (method == "POST") {
                // Traiter la requête POST
                std::cout << "ajout employés : erreur" << std::endl;
            }
            else if (method == "PUT") {
                // Traiter la requête PUT
                std::cout << "modifier employé" << std::endl;
            }
            else{
                std::cout << "méthode non supportée" << std::endl;
            } 
        } 
    }
    
    // Route de secours (404 Not Found)
    else {
        messageError = "Route non trouvée";
        response_status_code = 404;
        response_msg = "Not Found";
    }


    if(result != 0) {
        body = "{\"error\": \""+ messageError +"\"}";
        response = 
            "HTTP/1.1 "+ to_string(response_status_code) + " " + response_msg +"\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: " + std::to_string(body.length()) + "\r\n\r\n" + 
            body;
    }
    else {
        body = db_handler.formatter_JSON(); // JSON de sortie de la base de données
        response = 
            "HTTP/1.1 "+ to_string(response_status_code) + " " + response_msg +"\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Content-Typexecute_requeste: application/json\r\n"
            "Content-Length: " + std::to_string(body.length()) + "\r\n\r\n" + 
            body;
    }

    write(client_fd, response.c_str(), response.length());
    db_handler.close_db();
} 