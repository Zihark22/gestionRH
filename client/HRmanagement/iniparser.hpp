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

class IniParser {
public:
    // Nettoie les espaces/tabulations inutiles en début et fin de chaîne
    static string trim(const string& str);

    // Fonction pour extraire la valeur d'un champ JSON
    static QString getField(const std::string &obj, const std::string &key);

    //Méthode de lecture de fichier de configuration
    static QMap<QString, QString> loadConfig(const QString &cheminFichier);

    // Méthode de sauvegarde d'une configuration dans un fichier .ini
    static bool saveConfig(const QString &cheminFichier, const QMap<QString, QString> &map);

    static QString getConfigPath(const QString &nomFichier);
};
#endif // INIPARSER_HPP
