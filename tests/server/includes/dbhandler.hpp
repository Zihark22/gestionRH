// #include <sqlite3.h>
// #include <iostream>
// #include <string>
// #include <map>
// #include <vector>


// #define QUERY_ALL_COLLABORATEURS "SELECT * FROM collaborateurs;" 


// class DBhandler {
//     public:
//         DBhandler(const std::string& dbPath);
//         ~DBhandler(); 
//         // void insertCollaborateur(const std::string& nom, const std::string& prenom, const std::string& email, const std::string& poste);
//         void getAllCollaborateurs();
//     private :
//         sqlite3* DB;
//         int exit;
//         static std::vector<std::map<std::string, std::string>> collaborateurs;
//         char* messageError;
//         std::string sql;
//         static int formatter_JSON(void* data, int argc, char** argv, char** azColName);
//         // void createTable();
//         // void insertData(const std::string& nom, const std::string& prenom, const std::string& email, const std::string& poste);
//         // void selectData();
// };


#ifndef DBHANDLER_HPP
#define DBHANDLER_HPP

#include <sqlite3.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>

#define QUERY_ALL_COLLABORATEURS "SELECT * FROM collaborateurs;" 

class DBhandler {
private:
    sqlite3* DB = nullptr;
    char* messageError = nullptr;

    // La liste devient une variable membre classique d'instance (NON static)
    std::vector<std::map<std::string, std::string>> collaborateurs;

    // Le callback SQLite doit rester static
    static int save_data(void* data, int argc, char** argv, char** azColName);

    void displayCollaborateurs(void);

    // void createTable();
    // void insertData(const std::string& nom, const std::string& prenom, const std::string& email, const std::string& poste);
    // void selectData();

public:
    DBhandler(const std::string& dbPath);
    ~DBhandler();

    void getAllCollaborateurs();    
    std::string formatter_JSON();    
    // void insertCollaborateur(const std::string& nom, const std::string& prenom, const std::string& email, const std::string& poste);

};

#endif