#ifndef DBHANDLER_HPP
#define DBHANDLER_HPP

#include "employee.hpp"

#include <iostream>
#include <sqlite3.h> // SQLite library for database access.
#include <vector>

class DBhandler {

public:
    DBhandler() = default;
    
    // Initialize the database handler with the supplied path.
    DBhandler(const std::string& newdbPath);
    
    ~DBhandler();

    /////// SQL actions ///////

    int getAllEmployees();
    int getEmployee(const int &id);
    int deleteEmployee(const int &id);
    int modifyEmployee(const Employee &e, const std::string &id);
    int addEmployee(const Employee &e);
    

    /** @brief Format employee data as JSON.
     *  @return A JSON array containing the employee data.
     */
    std::string formatterJson();
    
    void openDB();
    void closeDB();
    int countEmployees();

private:
    sqlite3* db = nullptr;                      //< SQLite database handle.
    char* messageError = nullptr;               //< SQLite error message returned by a query.
    std::string dbPath = "";                    //< Database path.
    std::vector<Employee> employees;            //< Employees loaded from the database.


    // SQLite callback (must be static).
    static int saveData(void* data, int argc, char** argv, char** azColName);

    // Display employees in the console.
    void displayEmployees();
    
};

#endif