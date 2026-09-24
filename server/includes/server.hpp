#pragma once

#include "apiserver.hpp"
#include "iniparser.hpp"

#include <iostream>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <chrono>
#include <unistd.h>

class Server {
    
public:
    // Initialize the server with command-line arguments.
    Server(int argc, char* argv[]);

    ~Server();

    // Prevent copying to avoid closing the socket twice.
    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    // Start the server.
    void start();

    // Handle API requests that modify the configuration.
    std::string handleAction(const std::string &req, const std::string &body);

private:
    int server_fd;                              //< Server socket.
    sockaddr_in address{};                      //< Socket address.
    int opt;                                    //< Socket configuration option.
    int port;                                   //< Server port.
    std::string configFilePath = "config.ini"; //< Configuration file path.
    std::string host;                           //< Server host.
    std::string dbPath;                         //< Database path.
    std::string logFile;                        //< Log file path.
    ApiServer apiServer;                        //< API request handler.
    std::string executablePath;                 //< Executable path.
    std::vector<std::string> args;              //< Process arguments.

    // Load configuration from an INI file.
    void loadConfig(const std::string& file_path);

    // Convert the current configuration to JSON for the API.
    std::string configToJson();

    // Update the configuration file from JSON received through the API.
    int modifyConfigFromJson(const std::string &json);

    // Restart the server process.
    void executeRestart();

    // Request an asynchronous server restart.
    void requestRestart();

};