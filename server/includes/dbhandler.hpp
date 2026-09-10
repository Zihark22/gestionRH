#ifndef DBHANDLER_HPP
#define DBHANDLER_HPP

#include "employee.hpp"

#include <iostream>
#include <sqlite3.h> // Bibliothèque SQLite3 pour la gestion de la base de données
#include <vector>

class DBhandler {

public:
    DBhandler();
    
    // Constructeur qui initialise le gestionnaire de base de données avec le chemin fourni
    DBhandler(const std::string& dbPath);
    
    ~DBhandler();

    /////// SQL actions ///////

    int get_all_employees();
    int get_employee(const int &id);
    int delete_employee(const int &id);
    int modify_employee(const Employee &e, const std::string &id);
    int add_employee(const Employee &e);
    

    /** @brief Formate les données des employés au format JSON
     *  @return Chaîne JSON contenant une liste des données des employés
     */
    std::string formatter_JSON();
    
    void open_db();
    void close_db();
    int count_employees(void);

private:
    sqlite3* db = nullptr;                      //< Pointeur vers la base de données SQLite
    char* messageError = nullptr;               //< Pointeur vers le message d'erreur de retour de la BDD lors d'une requête
    std::string db_path;                        //< chemin de la BDD
    std::vector<Employee> employees;            //< Vecteur pour stocker les employés récupérés de la base de données


    // Le callback SQLite (doit être static)
    static int save_data(void* data, int argc, char** argv, char** azColName);

    // Méthode pour afficher les employés dans la console
    void display_employees(void);
    
};

#endif