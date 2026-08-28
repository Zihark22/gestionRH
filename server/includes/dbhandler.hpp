#ifndef DBHANDLER_HPP
#define DBHANDLER_HPP

#include "employee.hpp"

#include <iostream>
#include <sqlite3.h> // Bibliothèque SQLite3 pour la gestion de la base de données
#include <vector>

class DBhandler {
private:
    sqlite3* db = nullptr;
    char* messageError = nullptr; // message d'erreur de retour de la BDD lors d'une requête
    std::string db_path;        // chemin de la BDD

    // La liste des employés récupérés
    vector<Employee> employees;

    // Le callback SQLite doit rester static
    static int save_data(void* data, int argc, char** argv, char** azColName);

    void display_employees(void);
    

public:
    DBhandler();
    DBhandler(const std::string& dbPath);
    ~DBhandler();

    // SQL actions
    int get_all_employees();
    int get_employee(const int &id);
    int delete_employee(const int &id);
    int modify_employee(const Employee &e, const string &id);
    int add_employee(const Employee &e);
    
    std::string formatter_JSON();
    void open_db();
    void close_db();
    int count_employees(void);
};

#endif