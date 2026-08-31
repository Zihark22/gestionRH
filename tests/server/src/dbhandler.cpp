#include "../includes/dbhandler.hpp"

DBhandler::DBhandler(const std::string& dbPath) {
    // On utilise directement la variable membre DB (pas de "sqlite3* DB;")
    int exit = sqlite3_open(dbPath.c_str(), &this->DB);
    
    if (exit != SQLITE_OK) {
        std::cerr << "Error opening DB: " << sqlite3_errmsg(this->DB) << std::endl;
        sqlite3_close(this->DB);
        this->DB = nullptr;
    } else {
        std::cout << "--- Connection Database: connected ---" << std::endl;
    }
}

DBhandler::~DBhandler() {
    if (this->DB) {
        sqlite3_close(this->DB);
        this->DB = nullptr;
        std::cout << "Connection DB closed" << std::endl;
    }
}

// Le callback reçoit 'data' qui est notre pointeur 'this'
int DBhandler::save_data(void* data, int argc, char** argv, char** azColName) {
    // On re-caste le void* en pointeur d'instance DBhandler*
    auto* self = static_cast<DBhandler*>(data);

    if (!self) return SQLITE_ERROR;

    std::map<std::string, std::string> row;
    for (int i = 0; i < argc; i++) {
        row[azColName[i]] = argv[i] ? argv[i] : "NULL";
    }

    // On remplit le vecteur de L'INSTANCE courante
    self->collaborateurs.   (row);

    return SQLITE_OK;
}

void DBhandler::getAllCollaborateurs() {
    if (!DB) return;

    // Réinitialiser la liste si vous souhaitez rafraîchir les données
    collaborateurs.clear();

    // On passe 'this' en 4ème paramètre à sqlite3_exec
    int rc = sqlite3_exec(DB, QUERY_ALL_COLLABORATEURS, DBhandler::save_data, static_cast<void*>(this), &messageError);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL Error: " << messageError << std::endl;
        sqlite3_free(messageError);
        return;
    }
    std::cout << "All data retrieved successfully" << std::endl;
    // displayCollaborateurs();
    // std::cout << "JSON Output: " << formatter_JSON() << std::endl;
}


void DBhandler::displayCollaborateurs(void) {
    for (const auto& collaborateur : this->collaborateurs) {
        std::cout << "Collaborateur: " << std::endl;
        for (const auto& [key, value] : collaborateur) {
            std::cout << "\t" << key << " = \'" << value << "\'" << std::endl;
        }
        std::cout << std::endl;
    }
}

std::string DBhandler::formatter_JSON() {
    std::string json = "[";
    for (size_t i = 0; i < collaborateurs.size(); ++i) {
        json += "{";
        for (const auto& [key, value] : collaborateurs[i]) {
            json += "\"" + key + "\":\"" + value + "\",";
        }
        json.pop_back(); // Remove the last comma
        json += "},";
    }
    json.pop_back(); // Remove the last comma
    json += "]";
    return json;
}
