#ifndef INIPARSER_HPP
#define INIPARSER_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>

// Structure représentant une variable
struct VariableConfig {
    std::string description; // Commentaire associé
    std::string nom;         // Clé (ex: host, port)
    std::string valeur;      // Valeur (ex: 127.0.0.1, 8080)
};

// Structure représentant une section (ex: Server, Database)
struct SectionConfig {
    std::string nom;                         // Nom de la section (ex: Server)
    std::vector<VariableConfig> variables;  // Liste des variables de cette section

    // Méthode utilitaire pour chercher une variable par son nom
    const VariableConfig* getVariable(const std::string& varName) const {
        for (const auto& var : variables) {
            if (var.nom == varName) return &var;
        }
        return nullptr;
    }
};

class IniParser {
private:
    // Nettoie les espaces/tabulations inutiles en début et fin de chaîne
    static std::string trim(const std::string& str);

public:
    // Méthode principale de parsing
    static std::vector<SectionConfig> parse(const std::string& filepath);

    // static std::string serialize_to_json(std::map<std::string, std::string> );

    // static std::map<std::string, std::string> parse_from_json(const std::string &json); 
};

#endif // INIPARSER_HPP