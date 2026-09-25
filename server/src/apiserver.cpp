#include "../includes/apiserver.hpp"

ApiServer::ApiServer(const std::string &dbPath) {
    dbHandler = DBhandler(dbPath);
}

void ApiServer::setRequestHandler(RequestHandler handler) {
    myHandler = handler;
}

void ApiServer::start(int server_fd) {
    // Main loop accepting incoming connections.
    while (_isRunning) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd < 0) 
            continue;

        char buffer[2048] = {0};
        read(client_fd, buffer, sizeof(buffer) - 1);

        std::string request(buffer);
        std::cout << "\n============ Request received ============\n" << request << "\n" << std::endl;
        parseRequestHttp(request, client_fd);

        close(client_fd);
    }
}

void ApiServer::parseRequestHttp(std::string request, const int client_fd) {
    std::string method;
    std::string endpoint;
    
    // Extract the HTTP method (GET, POST, DELETE, PUT, etc.).
    size_t space_pos = request.find(" ");
    if (space_pos != std::string::npos)
        method = request.substr(0, space_pos);
    
    // Extract the endpoint between the first and second spaces.
    size_t first_space = request.find(" ");
    size_t second_space = request.find(" ", first_space + 1);
    
    if (first_space != std::string::npos && second_space != std::string::npos)
        endpoint = request.substr(first_space + 1, second_space - first_space - 1);

    // Extract the body, if present, using the HTTP header boundary.
    const size_t headerEnd = request.find("\r\n\r\n");
    body = headerEnd == std::string::npos ? "" : request.substr(headerEnd + 4);

    // Authentication will be implemented when user accounts are added.
    authenticationOk = true;
    
    if(authenticationOk)
        executeRequest(method, endpoint, client_fd);
    else
        std::cout << "Authentication failed" << std::endl;

} 

void ApiServer::executeRequest(const std::string &method, const std::string &endpoint, const int client_fd) {
    responseStatusCode = 200;
    responseMsg = "OK";
    messageError.clear();

    std::string repbody;
    std::string responseHTTP;
    std::string responseGlobal; // Response data other than the employee list, such as config or a new ID.

    dbHandler.openDB();
    std::cout << "Action demandée par le client : ";

    handleRequest(method, endpoint, responseGlobal); // Dispatch the request and build the response data.

    // Build the HTTP response.
    if(responseStatusCode != 200) { // Return an error payload for non-success responses.
        repbody = "{\"error\": \""+ messageError +"\"}";
        responseHTTP = 
            "HTTP/1.1 "+ std::to_string(responseStatusCode) + " " + responseMsg +"\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: " + std::to_string(repbody.length()) + "\r\n\r\n" + 
            repbody;
    }
    else {  // Return either the database JSON or the handler response.
        if(responseGlobal.empty())
            repbody = dbHandler.formatterJson(); // Serialized database results.
        else
            repbody = responseGlobal; // Another server response, such as configuration.

        responseHTTP = 
            "HTTP/1.1 "+ std::to_string(responseStatusCode) + " " + responseMsg +"\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: " + std::to_string(repbody.length()) + "\r\n\r\n" + 
            repbody;
    }

    write(client_fd, responseHTTP.c_str(), responseHTTP.length());
    dbHandler.closeDB();
} 


RouteMatch ApiServer::parseRoute(const std::string& uri) {
    // Map the static routes.
    static const std::unordered_map<std::string, EndpointAction> routeTable = {
        {"/api/employees", EndpointAction::EMPLOYEES_COLLECTION},
        {"/api/config",    EndpointAction::CONFIG}
    };

    // Look up the URI directly.
    auto it = routeTable.find(uri);
    if (it != routeTable.end()) {
        return {it->second, std::nullopt};
    }

    // Handle the dynamic /api/employees/:id route.
    const std::string prefix = "/api/employees/";
    if (uri.rfind(prefix, 0) == 0) { // The URI starts with "/api/employees/".
        std::string idStr = uri.substr(prefix.length());
        try {
            size_t parsedLength = 0;
            int id = std::stoi(idStr, &parsedLength);
            if (parsedLength != idStr.size())
                return {EndpointAction::UNKNOWN, std::nullopt};
            return {EndpointAction::EMPLOYEE_BY_ID, id};
        } catch (...) {
            return {EndpointAction::UNKNOWN, std::nullopt}; // The ID is not numeric.
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
                // Handle GET /api/employees.
                std::cout << "Obtenir employés" << std::endl;

                // Execute the database operation.
                result = dbHandler.getAllEmployees();

                // Build the response from the result.
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
                // Handle POST /api/employees.
                std::cout << "ajout employé" << std::endl;

                // Execute the database operation.
                Employee e(this->body);
                result = dbHandler.addEmployee(e);

                // Build the response from the result.
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
                // Handle GET /api/employees/:id.
                std::cout << "Obtenir employé dont ID = " << route.id.value() << std::endl;

                // Execute the database operation.
                result = dbHandler.getEmployee(route.id.value());

                // Build the response from the result.
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
                // Handle DELETE /api/employees/:id.
                std::cout << "supprimer employé dont ID = " << route.id.value() << std::endl;

                // Execute the database operation.
                result = dbHandler.deleteEmployee(route.id.value());

                // Build the response from the result.
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
                // Handle PUT /api/employees/:id.
                std::cout << "modifier employé dont ID = " << route.id.value() << std::endl;

                // Execute the database operation.
                Employee e(this->body);
                result = dbHandler.modifyEmployee(e, std::to_string(route.id.value()));

                // Build the response from the result.
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
                // Handle GET /api/config.
                std::cout << "Obtenir config" << std::endl;

                // Call the server handler.
                if (myHandler) {
                    responseGlobal = myHandler("getconfig", "");
                    std::cout << "reponse : " << responseGlobal << std::endl;
                    result = 0;
                } 
                else {
                    responseGlobal = "ERROR 500: No handler";
                    result = -1;
                }

                // Build the response from the result.
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
                // Handle PUT /api/config.
                std::cout << "Modifier config" << std::endl;

                // Ask the server to update config.ini.
                if (myHandler) {
                    responseGlobal = myHandler("modifyconfig",this->body);
                    std::cout << "reponse : " << responseGlobal << std::endl;
                    result = 0;
                } 
                else {
                    responseGlobal = "ERROR 500: No handler";
                    result = -1;
                }

                // Build the response from the result.
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
            else {
                messageError = "The method is not allowed for this endpoint";
                responseStatusCode = 405;
                responseMsg = "Method Not Allowed";
            }
            break;

        case EndpointAction::UNKNOWN: // Fallback route (404 Not Found).
            messageError = "Route invalide";
            responseStatusCode = 404;
            responseMsg = "Not Found";
            std::cerr << "Route non reconnue : " << std::endl;
        default:  
            std::cerr << "Le endpoint n'a pas été parser correctement !" << std::endl;
            break;
    }
}