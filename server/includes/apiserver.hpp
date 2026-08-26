#ifndef APISERVER_HPP
#define APISERVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "../includes/dbhandler.hpp"

#define QUERY_ALL_EMPLOYEES "SELECT * FROM employees;" 

class ApiServer {
private:
    std::string messageError;
    int response_status_code = 200; // Code de statut HTTP par défaut
    std::string response_msg;
    bool authentication_ok = false;
    DBhandler db_handler;
    string body="";
    void parse_request_http(std::string request, const int client_fd);
    void execute_request(const std::string &method, const std::string &endpoint, const int client_fd);

public:
    ApiServer();
    ApiServer(const string &db_path);
    ~ApiServer();
    void start(int server_fd);
};

#endif