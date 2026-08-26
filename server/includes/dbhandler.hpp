#ifndef DBHANDLER_HPP
#define DBHANDLER_HPP

#include "employee.hpp"

#include <iostream>
#include <sqlite3.h> // Bibliothèque SQLite3 pour la gestion de la base de données
#include <string>
#include <vector>
#include <map>


#define QUERY_ALL_EMPLOYEES "SELECT * FROM employees;" 

class DBhandler {
private:
    sqlite3* DB = nullptr;
    char* messageError = nullptr;
    std::string db_path;

    // La liste devient une variable membre classique d'instance (NON static)
    vector<Employee> employees;

    // Le callback SQLite doit rester static
    static int save_data(void* data, int argc, char** argv, char** azColName);

    void display_employees(void);


    // void createTable();
    // void insertData(const std::string& nom, const std::string& prenom, const std::string& email, const std::string& poste);
    // void selectData();

public:
    DBhandler();
    DBhandler(const std::string& dbPath);
    ~DBhandler();

    // SQL actions
    int get_all_employees();
    int get_employee(const int &id);
    int delete_employee(const int &id);
    int modify_employee(const int &id);
    int add_employee(const Employee &e);
    
    std::string formatter_JSON();
    void open_db();
    void close_db();
    // void insertCollaborateur(const std::string& nom, const std::string& prenom, const std::string& email, const std::string& poste);

};

#endif