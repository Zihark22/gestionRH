#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
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
    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, (last - first + 1));
    }

public:
    // Méthode principale de parsing
    static std::vector<SectionConfig> parse(const std::string& filepath) {
        std::vector<SectionConfig> sections;
        std::ifstream file(filepath);

        if (!file.is_open()) {
            std::cerr << "[IniParser] Erreur : Impossible d'ouvrir le fichier : " << filepath << std::endl;
            return sections;
        }

        std::string line;
        std::string accumulatedComments;
        SectionConfig* currentSection = nullptr;

        while (std::getline(file, line)) {
            std::string trimmedLine = trim(line);

            // 1. Ligne vide -> On réinitialise la description accumulée
            if (trimmedLine.empty()) {
                accumulatedComments.clear();
                continue;
            }

            // 2. Ligne de commentaire (commence par ';' ou '#')
            if (trimmedLine[0] == ';' || trimmedLine[0] == '#') {
                std::string commentContent = trim(trimmedLine.substr(1));
                if (!accumulatedComments.empty()) {
                    accumulatedComments += " "; // Séparateur si commentaire multi-lignes
                }
                accumulatedComments += commentContent;
                continue;
            }

            // 3. Déclaration d'une section [NomDeLaSection]
            if (trimmedLine.front() == '[' && trimmedLine.back() == ']') {
                std::string sectionName = trim(trimmedLine.substr(1, trimmedLine.size() - 2));
                sections.push_back({sectionName, {}});
                currentSection = &sections.back();
                accumulatedComments.clear(); // Reset des commentaires pour la section
                continue;
            }

            // 4. Déclaration d'une variable (cle = valeur)
            size_t delimiterPos = trimmedLine.find('=');
            if (delimiterPos != std::string::npos && currentSection != nullptr) {
                std::string key = trim(trimmedLine.substr(0, delimiterPos));
                std::string value = trim(trimmedLine.substr(delimiterPos + 1));

                VariableConfig var;
                var.nom = key;
                var.valeur = value;
                var.description = accumulatedComments; // Assigne la description collectée

                currentSection->variables.push_back(var);
                
                // Consommé : Réinitialisation pour la prochaine variable
                accumulatedComments.clear();
            }
        }

        file.close();
        return sections;
    }
};