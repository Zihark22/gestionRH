#ifndef DBHANDLER_HPP
#define DBHANDLER_HPP

#include "employee.hpp"

#include <iostream>
#include <sqlite3.h> // Bibliothèque SQLite3 pour la gestion de la base de données
#include <vector>

class DBhandler {

public:
    DBhandler() = default;
    
    // Constructeur qui initialise le gestionnaire de base de données avec le chemin fourni
    DBhandler(const std::string& newdbPath);
    
    ~DBhandler();

    /////// SQL actions ///////

    int getAllEmployees();
    int getEmployee(const int &id);
    int deleteEmployee(const int &id);
    int modifyEmployee(const Employee &e, const std::string &id);
    int addEmployee(const Employee &e);
    

    /** @brief Formate les données des employés au format JSON
     *  @return Chaîne JSON contenant une liste des données des employés
     */
    std::string formatterJson();
    
    void openDB();
    void closeDB();
    int countEmployees(void);

private:
    sqlite3* db = nullptr;                      //< Pointeur vers la base de données SQLite
    char* messageError = nullptr;               //< Pointeur vers le message d'erreur de retour de la BDD lors d'une requête
    std::string dbPath = "";                    //< chemin de la BDD
    std::vector<Employee> employees;            //< Vecteur pour stocker les employés récupérés de la base de données


    // Le callback SQLite (doit être static)
    static int saveData(void* data, int argc, char** argv, char** azColName);

    // Méthode pour afficher les employés dans la console
    void displayEmployees(void);
    
};

#endif