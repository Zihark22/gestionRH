#include "../includes/dbhandler.hpp"

DBhandler::DBhandler() {
    db_path = "";
}

DBhandler::DBhandler(const string& dbPath) {
    db_path = dbPath;
}
DBhandler::~DBhandler() {
    if (this->DB) {
        // fermeture de la base de données SQLite
        sqlite3_close(this->DB);
        this->DB = nullptr;
        cout << "Connection DB closed" << endl;
    }
}
void DBhandler::open_db() {
    int exit = sqlite3_open(db_path.c_str(), &this->DB);
    
    if (exit != SQLITE_OK) {
        cerr << "Error opening DB: " << sqlite3_errmsg(this->DB) << endl;
        sqlite3_close(this->DB);
        this->DB = nullptr;
    } else {
        cout << "--- Connection Database: connected ---" << endl;
    }
} 
void DBhandler::close_db() {
    if (this->DB) {
        // fermeture de la base de données SQLite
        sqlite3_close(this->DB);
        cout << "Connection DB closed" << endl;
    }
}


// Le callback reçoit 'data' qui est notre pointeur 'this'
int DBhandler::save_data(void* data, int argc, char** argv, char** azColName) {
    // On re-caste le void* en pointeur d'instance DBhandler*
    auto* self = static_cast<DBhandler*>(data);

    if (!self) return SQLITE_ERROR;

    map<string, string> row;
    for (int i = 0; i < argc; i++) {
        row[azColName[i]] = argv[i] ? argv[i] : "NULL";
    }

    // On remplit le vecteur de L'INSTANCE courante
    self->employees.emplace_back(Employee::from_sql(row));

    return SQLITE_OK;
}

int DBhandler::get_all_employees() {
    if (!DB) return 1; // Indiquer que la base de données n'est pas ouverte

    // Réinitialiser la liste si vous souhaitez rafraîchir les données
    employees.clear();

    // On passe 'this' en 4ème paramètre à sqlite3_exec
    int rc = sqlite3_exec(DB, QUERY_ALL_EMPLOYEES, DBhandler::save_data, static_cast<void*>(this), &messageError);

    if (rc != SQLITE_OK) {
        cerr << "SQL Error: " << messageError << endl;
        sqlite3_free(messageError);
        return 1; // Indiquer qu'il y a eu une erreur
    }
    cout << "All data retrieved successfully" << endl;
    // displayEmployees();
    // cout << "JSON Output: " << formatter_JSON() << endl;
    return 0; // Indiquer que tout s'est bien passé
}


int DBhandler::get_employee(const int &id) {
    if (!DB) return 1; // Indiquer que la base de données n'est pas ouverte

    // Réinitialiser la liste si vous souhaitez rafraîchir les données
    employees.clear();

    string query = "SELECT * FROM employees WHERE employees.id=" + to_string(id) + ";";

    // On passe 'this' en 4ème paramètre à sqlite3_exec
    int rc = sqlite3_exec(DB, query.c_str(), DBhandler::save_data, static_cast<void*>(this), &messageError);

    if (rc != SQLITE_OK) {
        cerr << "SQL Error: " << messageError << endl;
        sqlite3_free(messageError);
        return 1; // Indiquer qu'il y a eu une erreur
    }
    cout << "All data retrieved successfully" << endl;
    // displayEmployees();
    // cout << "JSON Output: " << formatter_JSON() << endl;
    return 0; // Indiquer que tout s'est bien passé
}


int DBhandler::delete_employee(const int &id) {
    if (!DB) return 1; // Indiquer que la base de données n'est pas ouverte

    // Réinitialiser la liste si vous souhaitez rafraîchir les données
    employees.clear();

    string query = "DELETE FROM employees WHERE employees.id=" + to_string(id) + ";";

    // On passe 'this' en 4ème paramètre à sqlite3_exec
    int rc = sqlite3_exec(DB, query.c_str(), NULL, NULL, &messageError);

    if (rc != SQLITE_OK) {
        cerr << "SQL Error: " << messageError << endl;
        sqlite3_free(messageError);
        return 1; // Indiquer qu'il y a eu une erreur
    }
    cout << "Employee successfully deleted" << endl;
    return 0; // Indiquer que tout s'est bien passé
}


int DBhandler::modify_employee(const int &id){
    if (!DB) return 1; // Indiquer que la base de données n'est pas ouverte

    // Réinitialiser la liste si vous souhaitez rafraîchir les données
    employees.clear();

    string query = "INSERT ";

    // On passe 'this' en 4ème paramètre à sqlite3_exec
    int rc = sqlite3_exec(DB, query.c_str(), NULL, NULL, &messageError);

    if (rc != SQLITE_OK) {
        cerr << "SQL Error: " << messageError << endl;
        sqlite3_free(messageError);
        return 1; // Indiquer qu'il y a eu une erreur
    }
    cout << "Employee successfully deleted" << endl;
    return 0; // Indiquer que tout s'est bien passé
} 
int DBhandler::add_employee(const Employee &e) {
    if (!DB) return 1; // Indiquer que la base de données n'est pas ouverte

    // Réinitialiser la liste si vous souhaitez rafraîchir les données
    employees.clear();

    string query = "INSERT INTO employees (\
        firstname, lastname, birthdate, job, executive_status, \
        position, coefficient, start_date, \
        manager_id, prev_plan, signed_plan\
    ) VALUES (\
        '"+e.firstname()+"', '"+e.lastname()+"', '"+e.birthdate().toString()+"', '"+e.job()+"', "+to_string(e.is_executive())+", \
        '"+to_string(e.position())+"', "+to_string(e.coefficient())+", '"+e.start_date().toString()+"', \
        "+to_string(e.manager_id())+", '"+e.prev_plan()+"', "+to_string(e.signed_plan())+"\
    );";

    // On passe 'this' en 4ème paramètre à sqlite3_exec
    int rc = sqlite3_exec(DB, query.c_str(), NULL, NULL, &messageError);

    if (rc != SQLITE_OK) {
        cerr << "SQL Error: " << messageError << endl;
        sqlite3_free(messageError);
        return 1; // Indiquer qu'il y a eu une erreur
    }
    cout << "Employee successfully added" << endl;
    return 0; // Indiquer que tout s'est bien passé
}


void DBhandler::display_employees(void) {
    for (const auto& emp : employees) {
        emp.display();
    }
}

string DBhandler::formatter_JSON() {
    string json = "[";
    for (size_t i = 0; i < employees.size(); ++i) {
        json += employees[i].toJSON();
        if (i < employees.size() - 1) {
            json += ",";
        }
    }
    json += "]";
    return json;
}
