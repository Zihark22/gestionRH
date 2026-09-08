#include "../includes/server.hpp"

#include <sqlite3.h>

int main(int argc, char* argv[]) {

    Server server(argc, argv);
    server.start();

    // string json = R"([{"firstname":"Marc","lastname":"Dumort","job":"Dev","birthdate":"12-05-1998","executive_status":false,"position":2.5,"coefficient":150,"start_date":"01/05/2000","manager_id":7,"prev_plan":"Plan A","signed_plan":true}])";

    return 0;
}
