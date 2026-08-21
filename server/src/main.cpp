#include "../includes/server.hpp"

#include <sqlite3.h>
#include <iostream>
#include <string>

int main(void) {
    
    // Server server("../config/config.ini");
    Server* server = new Server();
    server->startServer();

    return 0;
}


// cmake .. && make -j$(nproc) && ./ServerApp