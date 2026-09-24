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
    EMPLOYEES_COLLECTION, // /api/employees (GET all, POST new)
    EMPLOYEE_BY_ID,       // /api/employees/:id (GET one, PUT, DELETE)
    CONFIG                // /api/config
};

struct RouteMatch {
    EndpointAction action = EndpointAction::UNKNOWN;
    std::optional<int> id; // Set when an ID is detected.
};

class ApiServer {

public:
    ApiServer() = default;

    // Initialize the database handler with the supplied path.
    ApiServer(const std::string &dbPath);

    // Handler signature: receive request data and return the client response.
    using RequestHandler = std::function<std::string(const std::string& request, const std::string& body)>;

    // Set the handler for application-specific requests.
    void setRequestHandler(RequestHandler handler);

    // Start API communication.
    void start(int server_fd); 

private:
    std::string messageError;       //< JSON error message.
    int responseStatusCode = 200;   //< Default HTTP status code.
    std::string responseMsg;        //< HTTP status message.
    bool authenticationOk = false;  //< Authentication result.
    std::string body = "";          //< HTTP request body.
    DBhandler dbHandler;            //< Database access handler.
    RequestHandler myHandler;       //< Application-specific request handler.
    bool _isRunning = true;          //< Whether the server is running.

    
    ///////// Methods /////////

    /**
    * @brief Parse an HTTP request to extract its method, endpoint, and body.
     * 
    * @param request String containing the HTTP request.
    * @param client_fd Client file descriptor.
     */
    void parseRequestHttp(std::string request, const int client_fd);
    
    /**
    * @brief Execute a request and build the HTTP response returned to the client.
     * 
    * @param method HTTP request method.
    * @param endpoint Request endpoint.
    * @param client_fd Client file descriptor.
     */
    void executeRequest(const std::string &method, const std::string &endpoint, const int client_fd);

    /**
    * @brief Match a URI to an API route.
     * 
     * @param uri 
     */
    RouteMatch parseRoute(const std::string& uri);
      
    /**
     * @brief 
     * 
    * @param method HTTP request method.
    * @param uri    Request URI.
    * @param responseGlobal Response body produced by a handler.
     */
    void handleRequest(const std::string& method, const std::string& uri, std::string &responseGlobal);

};

#endif