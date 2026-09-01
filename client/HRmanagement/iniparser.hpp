#ifndef INIPARSER_HPP
#define INIPARSER_HPP

#include <QDebug>
#include <fstream>
#include <vector>
#include <cctype>

using namespace std;

// Structure représentant une variable
struct VariableConfig {
    string description; // Commentaire associé
    string nom;         // Clé (ex: host, port)
    string valeur;      // Valeur (ex: 127.0.0.1, 8080)
};

// Structure représentant une section (ex: Server, Database)
struct SectionConfig {
    string nom;                         // Nom de la section (ex: Server)
    vector<VariableConfig> variables;  // Liste des variables de cette section

    // Méthode utilitaire pour chercher une variable par son nom
    const VariableConfig* getVariable(const string& varName) const {
        for (const auto& var : variables) {
            if (var.nom == varName) return &var;
        }
        return nullptr;
    }
};

class IniParser {
private:
    // Nettoie les espaces/tabulations inutiles en début et fin de chaîne
    static string trim(const string& str);

public:
    // Méthode principale de parsing
    static vector<SectionConfig> parse(const string& filepath);
};
#endif // INIPARSER_HPP
