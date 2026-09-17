#include "../includes/dbhandler.hpp"

DBhandler::DBhandler(const std::string& newdbPath) {
    this->dbPath = newdbPath;
}

DBhandler::~DBhandler() {
    if (this->db) {
        sqlite3_close(this->db); // fermeture de la base de données SQLite
        this->db = nullptr;
        std::cout << "Connection DB closed" << std::endl;
    }
}

void DBhandler::openDB() {
    int exit = sqlite3_open(dbPath.c_str(), &this->db);
    
    if (exit != SQLITE_OK) {
        std::cerr << "Error opening DB: " << sqlite3_errmsg(this->db) << std::endl;
        sqlite3_close(this->db);
        this->db = nullptr;
    } 
    else
        std::cout << "--- Connection Database: connected ---" << std::endl;
} 

void DBhandler::closeDB() {
    if (this->db) {
        sqlite3_close(this->db);
        std::cout << "Connection DB closed" << std::endl;
    }
}

int DBhandler::saveData(void* data, int argc, char** argv, char** azColName) {
    // Le callback reçoit 'data' qui est notre pointeur 'this'
    
    auto* self = static_cast<DBhandler*>(data); // On re-caste le void* en pointeur d'instance DBhandler*

    if (!self) 
        return SQLITE_ERROR;

    std::map<std::string, std::string> row;
    for (int i = 0; i < argc; i++)
        row[azColName[i]] = argv[i] ? argv[i] : "NULL";


    // On remplit le vecteur de L'INSTANCE courante
    self->employees.emplace_back(Employee::fromSql(row));

    return SQLITE_OK;
}

int DBhandler::getAllEmployees() {
    if (!this->db) 
        return -1;

    employees.clear();

    // On passe 'this' en 4ème paramètre à sqlite3_exec
    int rc = sqlite3_exec(this->db, "SELECT * FROM employees ORDER BY id ASC;", DBhandler::saveData, static_cast<void*>(this), &messageError);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL Error: " << messageError << std::endl;
        sqlite3_free(messageError);
        return -1; // Indiquer qu'il y a eu une erreur
    }
    std::cout << "All data retrieved successfully" << std::endl;
    return 0; // Indiquer que tout s'est bien passé
}

int DBhandler::getEmployee(const int &id) {
    if (!this->db) 
        return -1; 

    employees.clear();

    std::string query = "SELECT * FROM employees WHERE employees.id=" + std::to_string(id) + ";";

    // On passe 'this' en 4ème paramètre à sqlite3_exec
    int rc = sqlite3_exec(this->db, query.c_str(), DBhandler::saveData, static_cast<void*>(this), &messageError);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL Error: " << messageError << std::endl;
        sqlite3_free(messageError);
        return -1; // Indiquer qu'il y a eu une erreur
    }

    std::cout << "All data retrieved successfully" << std::endl;
    return 0; // Indiquer que tout s'est bien passé
}

int DBhandler::deleteEmployee(const int &id) {
    if (!this->db) 
        return -1;

    employees.clear();

    std::string query = "DELETE FROM employees WHERE employees.id=" + std::to_string(id) + ";";

    // On passe 'this' en 4ème paramètre à sqlite3_exec
    int rc = sqlite3_exec(this->db, query.c_str(), NULL, NULL, &messageError);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL Error: " << messageError << std::endl;
        sqlite3_free(messageError);
        return -1; // Indiquer qu'il y a eu une erreur
    }
    std::cout << "Employee successfully deleted" << std::endl;
    return 0; // Indiquer que tout s'est bien passé
}

int DBhandler::modifyEmployee(const Employee &e, const std::string &id) {
    if (!this->db) 
        return -1;

    employees.clear();

    std::string query = "\
        UPDATE employees \
        SET \
            firstname=\'" + e.firstname() + "\', \
            lastname=\'" + e.lastname() + "\', \
            birthdate=\'" + e.birthdate() + "\', \
            job=\'" + e.job() + "\', \
            executive_status=" + std::to_string(e.isExecutive())+", \
            position=" + std::to_string(e.position())+", \
            coefficient=" + std::to_string(e.coefficient())+", \
            start_date=\'" + e.startDate() + "\', \
            manager_id=" + std::to_string(e.managerId())+", \
            prev_plan=\'" + e.prevPlan()+"\',\
            signed_plan=" + std::to_string(e.signedPlan())+" \
        WHERE id="+id+";";
 
    // On passe 'this' en 4ème paramètre à sqlite3_exec
    int rc = sqlite3_exec(this->db, query.c_str(), NULL, NULL, &messageError);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL Error: " << messageError << std::endl;
        sqlite3_free(messageError);
        return -1; // Indiquer qu'il y a eu une erreur
    }
    std::cout << "Employee successfully modified" << std::endl;
    return 0; // Indiquer que tout s'est bien passé
} 

int DBhandler::addEmployee(const Employee &e) {
    if (!this->db) 
        return -1;

    employees.clear();

    std::string query = "INSERT INTO employees (\
        firstname, lastname, birthdate, job, executive_status, \
        position, coefficient, start_date, \
        manager_id, prev_plan, signed_plan\
    ) VALUES (\
        '"+e.firstname()+"', '"+e.lastname()+"', '"+e.birthdate()+"', '"+e.job()+"', " + std::to_string(e.isExecutive())+", \
        '"+ std::to_string(e.position())+"', " + std::to_string(e.coefficient())+", '"+e.startDate()+"', \
        " + std::to_string(e.managerId())+", '"+e.prevPlan()+"', " + std::to_string(e.signedPlan())+"\
    );";

    // On passe 'this' en 4ème paramètre à sqlite3_exec
    int rc = sqlite3_exec(this->db, query.c_str(), NULL, NULL, &messageError);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL Error: " << messageError << std::endl;
        sqlite3_free(messageError);
        return -1; // Indiquer qu'il y a eu une erreur
    }

    // Récupération de l'ID généré (retourne un sqlite3_int64 / long long)
    int64_t newId = sqlite3_last_insert_rowid(this->db);

    std::cout << "Employee successfully added with ID: " << newId << std::endl;
    return newId; // Indiquer que tout s'est bien passé
}


void DBhandler::displayEmployees(void) {
    for (const auto& emp : employees)
        emp.display();
}

int DBhandler::countEmployees(void){
    return employees.size();
} 

std::string DBhandler::formatterJson() {
    std::string json = "[";
    for (size_t i = 0; i < employees.size(); ++i) {
        json += employees[i].toJson();
        if (i < employees.size() - 1)
            json += ",";
    }
    json += "]";
    return json;
}
