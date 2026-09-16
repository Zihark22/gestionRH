#include "iniparser.hpp"

string IniParser::trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

std::string IniParser::getField(const std::string &obj, const std::string &key) {
    std::string pattern = "\"" + key + "\"";
    size_t pos = obj.find(pattern);
    if (pos == std::string::npos)
        return "";

    size_t colon = obj.find(':', pos + pattern.size());
    if (colon == std::string::npos)
        return "";

    size_t valueStart = obj.find_first_not_of(" \t\r\n", colon + 1);
    if (valueStart == std::string::npos) {
        return "";
    }

    // Cas chaîne de caractères
    if (obj[valueStart] == '"') {
        size_t valueEnd = valueStart + 1;
        while (valueEnd < obj.size()) {
            if (obj[valueEnd] == '\\' && valueEnd + 1 < obj.size()) {
                valueEnd += 2;
                continue;
            }
            if (obj[valueEnd] == '"')
                break;

            ++valueEnd;
        }
        return obj.substr(valueStart + 1, valueEnd - valueStart - 1);
    }

    // Cas nombre / bool / null
    size_t valueEnd = valueStart;
    while (valueEnd < obj.size() && obj[valueEnd] != ',' && obj[valueEnd] != '}')
        ++valueEnd;
    return IniParser::trim(obj.substr(valueStart, valueEnd - valueStart));
}

vector<SectionConfig> IniParser::parse(const string& filepath) {
    vector<SectionConfig> sections;
    ifstream file(filepath);

    if (!file.is_open()) {
        qDebug() << "[IniParser] Erreur : Impossible d'ouvrir le fichier : " << filepath;
        return sections;
    }

    string line;
    string accumulatedComments;
    SectionConfig* currentSection = nullptr;

    while (getline(file, line)) {
        string trimmedLine = trim(line);

        // 1. Ligne vide -> On réinitialise la description accumulée
        if (trimmedLine.empty()) {
            accumulatedComments.clear();
            continue;
        }

        // 2. Ligne de commentaire (commence par ';' ou '#')
        if (trimmedLine[0] == ';' || trimmedLine[0] == '#') {
            string commentContent = trim(trimmedLine.substr(1));
            if (!accumulatedComments.empty())
                accumulatedComments += " "; // Séparateur si commentaire multi-lignes

            accumulatedComments += commentContent;
            continue;
        }

        // 3. Déclaration d'une section [NomDeLaSection]
        if (trimmedLine.front() == '[' && trimmedLine.back() == ']') {
            string sectionName = trim(trimmedLine.substr(1, trimmedLine.size() - 2));
            sections.push_back({sectionName, {}});
            currentSection = &sections.back();
            accumulatedComments.clear(); // Reset des commentaires pour la section
            continue;
        }

        // 4. Déclaration d'une variable (cle = valeur)
        size_t delimiterPos = trimmedLine.find('=');
        if (delimiterPos != string::npos && currentSection != nullptr) {
            string key = trim(trimmedLine.substr(0, delimiterPos));
            string value = trim(trimmedLine.substr(delimiterPos + 1));

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

vector<SectionConfig> IniParser::parseFromString(std::string_view content) {
    vector<SectionConfig> sections;

    // On crée un flux de lecture à partir de la vue mémoire
    std::istringstream file{std::string(content)};

    string line;
    string accumulatedComments;
    SectionConfig* currentSection = nullptr;

    while (getline(file, line)) {
        string trimmedLine = trim(line);

        // 1. Ligne vide -> On réinitialise la description accumulée
        if (trimmedLine.empty()) {
            accumulatedComments.clear();
            continue;
        }

        // 2. Ligne de commentaire (commence par ';' ou '#')
        if (trimmedLine[0] == ';' || trimmedLine[0] == '#') {
            string commentContent = trim(trimmedLine.substr(1));
            if (!accumulatedComments.empty()) {
                accumulatedComments += " "; // Séparateur si commentaire multi-lignes
            }
            accumulatedComments += commentContent;
            continue;
        }

        // 3. Déclaration d'une section [NomDeLaSection]
        if (trimmedLine.front() == '[' && trimmedLine.back() == ']') {
            string sectionName = trim(trimmedLine.substr(1, trimmedLine.size() - 2));
            sections.push_back({sectionName, {}});
            currentSection = &sections.back();
            accumulatedComments.clear(); // Reset des commentaires pour la section
            continue;
        }

        // 4. Déclaration d'une variable (cle = valeur)
        size_t delimiterPos = trimmedLine.find('=');
        if (delimiterPos != string::npos && currentSection != nullptr) {
            string key = trim(trimmedLine.substr(0, delimiterPos));
            string value = trim(trimmedLine.substr(delimiterPos + 1));

            VariableConfig var;
            var.nom = key;
            var.valeur = value;
            var.description = accumulatedComments; // Assigne la description collectée

            currentSection->variables.push_back(var);

            // Consommé : Réinitialisation pour la prochaine variable
            accumulatedComments.clear();
        }
    }
    return sections;
}

QMap<QString, QString> IniParser::loadConfig(const QString &cheminFichier) {
    QSettings settings(cheminFichier, QSettings::IniFormat);
    QMap<QString, QString> map;

    // 1. Lecture directe d'une clé racine avec valeur par défaut si absente
    QString version = settings.value("Version", APP_VERSION).toString();
    qDebug() << "Version de l'application :" << version;

    // 2. Lecture dans un groupe [Server]
    settings.beginGroup(SERVER_GROUP_NAME);
    QString host = settings.value(HOST_KEY, DEFAULT_HOST).toString();
    QString port = settings.value(PORT_KEY, DEFAULT_PORT).toString();
    settings.endGroup();

    qDebug() << "Serveur :" << host << "sur le port :" << port;

    map[HOST_KEY] = host;
    map[PORT_KEY] = port;

    return map;
}

QString IniParser::getConfigPath(const QString &nomFichier) {
    // Emplacement où sera stocké le vrai fichier modifiable (à côté de l'exécutable)
    QString cheminDisque = QCoreApplication::applicationDirPath() + "/" + nomFichier;

    // Si le fichier n'existe pas encore sur le disque, on copie celui des ressources
    if (!QFile::exists(cheminDisque)) {
        qDebug() << "Premier lancement : extraction de la config par défaut...";

        // Copie depuis les ressources (":/config.ini" ou le préfixe configuré dans votre .qrc)
        QString configFile = ":/";
        configFile += CONFIG_FILE_NAME;
        if (QFile::copy(configFile, cheminDisque)) {
            // IMPORTANT : La copie d'une ressource hérite des permissions lecture-seule.
            // On accorde les droits en écriture à l'utilisateur :
            QFile::setPermissions(cheminDisque,
                                  QFileDevice::ReadOwner | QFileDevice::WriteOwner |
                                      QFileDevice::ReadUser  | QFileDevice::WriteUser);
            qDebug() << "Fichier initialisé avec succès :" << cheminDisque;
        } else {
            qWarning() << "Échec de la copie du fichier depuis les ressources !";
        }
    }
    return cheminDisque;
}

bool IniParser::saveConfig(const QString &cheminFichier, const QMap<QString, QString> &map) {
    // Fichier dans le même dossier que l'exécutable :
    QString path = QCoreApplication::applicationDirPath() + "/" + cheminFichier;

    // 1. Instancier QSettings avec le format IniFormat
    QSettings settings(cheminFichier, QSettings::IniFormat);

    // if (settings.contains("Server/host")) {
    //     qDebug() << "Server existe";
    // }

    // Écriture d'une clé racine (sans groupe)
    settings.setValue("Version", APP_VERSION);

    // 2. Écriture dans une section [Server]
    settings.beginGroup(SERVER_GROUP_NAME);
    settings.setValue(HOST_KEY, map[HOST_KEY]);
    settings.setValue(PORT_KEY, map[PORT_KEY]);
    settings.endGroup(); // Fin de la section [Server]

    // Optionnel : s'assurer de l'écriture immédiate sur disque
    settings.sync();
    return 1;
}
