#ifndef INIPARSER_HPP
#define INIPARSER_HPP

#include "parameters.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>

#include <QSettings>
#include <QDebug>
#include <QMap>
#include <QString>
#include <QStandardPaths>
#include <QDir>
#include <QCoreApplication>

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
        for (const auto& var : variables)
            if (var.nom == varName) return &var;
        return nullptr;
    }
};

class IniParser {
public:
    // Nettoie les espaces/tabulations inutiles en début et fin de chaîne
    static string trim(const string& str);

    // Fonction pour extraire la valeur d'un champ JSON
    static std::string getField(const std::string &obj, const std::string &key);

    // Méthode principale de parsing depuis un fichier
    static vector<SectionConfig> parse(const string& filepath);

    // Méthode principale de parsing depuis un string
    static vector<SectionConfig> parseFromString(std::string_view content);

    //Méthode de lecture de fichier de configuration
    static QMap<QString, QString> loadConfig(const QString &cheminFichier);

    // Méthode de sauvegarde d'une configuration dans un fichier .ini
    static bool saveConfig(const QString &cheminFichier, const QMap<QString, QString> &map);

    static QString getConfigPath(const QString &nomFichier);
};
#endif // INIPARSER_HPP
