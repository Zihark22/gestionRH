#include <sqlite3.h>
#include <iostream>
#include <string>
#include "../includes/server.hpp"
#include "../includes/dbhandler.hpp"

// static int callback(void* data, int argc, char** argv, char** azColName)
// {
// /* 
// function to display data from DB
// typedef int (*sqlite3_callback)(
//    void*,    // Data provided in the 4th argument of sqlite3_exec()
//    int,      // The number of columns in row
//    char**,   // An array of strings representing fields in the row
//    char**    // An array of strings representing column names
// );
// */
//     int i;
//     fprintf(stderr, "%s: ", (const char*)data);

//     for (i = 0; i < argc; i++) {
//         printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
//     }

//     printf("\n");
//     return 0;
// }

int main(void) {
    
    // sqlite3* DB;
    // int exit = 0;
    
    // // Open Db access
    // exit = sqlite3_open("../data/collaborateurs.db", &DB);
    // if(exit) {
    //     std::cerr << "Error openning DB : " << sqlite3_errmsg(DB) << std::endl;
    //     return(-1);
    // }
    // else {
    //     std::cout << "Connection Database : connected " << std::endl;
    // }

    // // Display all DB
    // std::string display_query = "SELECT * FROM collaborateurs;";
    // std::cout << "\nSTATE OF TABLE" << std::endl;
    // sqlite3_exec(DB, display_query.c_str(), callback, NULL, NULL);

    // // insert a new employee
    // std::string insert_query = "INSERT INTO collaborateurs ( 
    //     prenom, nom, date_naissance, poste, est_cadre,     position_syntec, coefficient_syntec, date_embauche,
    //     manager_id, plan_prevention, plan_signe
    // ) VALUES ( 
    //     'George', 'Macron', '2002-05-02', 'RH', 1, 
    //     '3.2', 210, '2013-04-10', 
    //     NULL, 'Plan B', 1 
    // );";
    // sqlite3_exec(DB, insert_query.c_str(), NULL, NULL, NULL);

    // // Display all DB after insertion
    // std::cout << "\nSTATE OF TABLE after insert" << std::endl;
    // sqlite3_exec(DB, display_query.c_str(), callback, NULL, NULL);

    // test with Postman software: GET http://127.0.0.1:8080/api/collaborateurs
    Server* server = new Server();
    server->startServer();


    // // Closing DB access 
    // exit = sqlite3_close(DB);
    // if(exit) {
    //     std::cerr << "Error closing DB" << sqlite3_errmsg(DB) << std::endl;
    //     return(-1);
    // }
    // else {
    //     std::cout << "Connection Database status : closed " << std::endl;
    // }x
    return 0;
}


// cmake .. && make -j$(nproc) && ./ServerApp