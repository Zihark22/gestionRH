#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


class Server {
private:
    int server_fd;
    sockaddr_in address{};
    int opt;
    int port;
    std::string config_file_path= "config.ini"; // Valeur par défaut
    std::string host;
    std::string db_path;
    std::string log_file;


public:
    Server(); // Désactiver le constructeur par défaut
    // Server(const char* file_path);
    void startServer();
    void loadConfig(const std::string& file_path);
};