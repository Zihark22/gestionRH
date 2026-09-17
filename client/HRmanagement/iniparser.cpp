#include "iniparser.hpp"

string IniParser::trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

QString IniParser::getField(const std::string &obj, const std::string &key) {
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
        return QString::fromStdString(obj.substr(valueStart + 1, valueEnd - valueStart - 1));
    }

    // Cas nombre / bool / null
    size_t valueEnd = valueStart;
    while (valueEnd < obj.size() && obj[valueEnd] != ',' && obj[valueEnd] != '}')
        ++valueEnd;
    return QString::fromStdString(IniParser::trim(obj.substr(valueStart, valueEnd - valueStart)));
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
