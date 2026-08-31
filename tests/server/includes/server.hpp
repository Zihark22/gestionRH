#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


class Server {
private:
    int server_fd;
    sockaddr_in address;
    int opt;
    int port;
    std::string configFilePath;
    std::string host;
    std::string db_path;
    std::string log_file;


public:
    Server(const std::string& configFilePath = "../config/config.ini");
    void startServer();
    void loadConfig(const std::string& configFilePath);
};