#include "../includes/server.hpp"

#include <sqlite3.h>
#include <iostream>
#include <string>

int main(void) {

    Server server;
    server.start();

    return 0;
}


// cmake .. && make -j$(nproc) && ./ServerApp