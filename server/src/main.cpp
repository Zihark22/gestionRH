#include "../includes/server.hpp"

int main(int argc, char* argv[]) {

    Server server(argc, argv);
    server.start();

    return 0;
}
