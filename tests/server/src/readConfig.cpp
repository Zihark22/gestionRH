#include <iostream>
#include "../includes/IniParser.hpp"

int ReadConfig() {
    // Lecture du fichier config.ini
    std::vector<SectionConfig> config = IniParser::parse("config.ini");

    // --- 1. Parcours global et affichage de toute la configuration ---
    std::cout << "=== CONFIGURATION CHARGÉE COMPLÈTE ===" << std::endl;
    
    for (const auto& section : config) {
        std::cout << "\n[Section : " << section.nom << "]" << std::endl;
        
        for (const auto& var : section.variables) {
            std::cout << "  - Nom         : " << var.nom << std::endl;
            std::cout << "    Valeur      : " << var.valeur << std::endl;
            std::cout << "    Description : " << (var.description.empty() ? "(aucune)" : var.description) << std::endl;
        }
    }

    // --- 2. Recherche ciblée (Ex: Pour initialiser SQLite et le Serveur) ---
    std::cout << "\n=== EXTRATION DE VARIABLES SPÉCIFIQUES ===" << std::endl;

    for (const auto& section : config) {
        if (section.nom == "Database") {
            if (auto var = section.getVariable("db_name")) {
                std::string dbFile = var->valeur;
                std::cout << "Base de données à ouvrir : " << dbFile << std::endl;
                std::cout << "Raison/Description : " << var->description << std::endl;
            }
        }
        else if (section.nom == "Server") {
            if (auto var = section.getVariable("port")) {
                int port = std::stoi(var->valeur);
                std::cout << "Port d'écoute du serveur : " << port << std::endl;
            }
        }
    }

    return 0;
}