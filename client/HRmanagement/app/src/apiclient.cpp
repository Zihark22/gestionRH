#include "include/apiclient.hpp"
#include "include/parameters.hpp"

// Constructeur
ApiClient::ApiClient(QObject *parent) : QObject(parent) {
    this->networkManager = new QNetworkAccessManager(this);
    this->configure("config.ini");
}
void ApiClient::configure(const std::string& file_path) {
    QString cheminConfig = ApiClient::getConfigPath(QString::fromStdString(file_path));
    QMap<QString, QString> config = ApiClient::loadConfig(cheminConfig);
    this->port = config["port"].toInt();
    this->host = config["host"];
}

// Méthode API REST
void ApiClient::sendGetEmployeeRequest(int id) {
    QString apiURL = "http://" + this->host+":"+QString::number(this->port)+"/api/employees";
    if(id>0)
        apiURL += "/"+std::to_string(id);
    QUrl url(apiURL);
    QNetworkRequest request(url);
    QNetworkReply *reply = this->networkManager->get(request);

    // if(id<1) {
        // requête bloquante pour chargement au lancement

        // 1. Création d'une boucle d'événements locale
        QEventLoop loop;

        // 2. Connexion de la fin de la requête pour débloquer la boucle
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

        // 3. Bloque l'exécution ICI jusqu'à ce que loop.quit() soit appelé
        loop.exec();
    // }

    // 4. Traitement de la réponse UNE FOIS LA REQUÊTE TERMINÉE
    if (reply->error() == QNetworkReply::NoError) {
        this->responseData = reply->readAll();

        // // Succès : Affichage dans la console
        // qDebug() << "\n--- Réponse du serveur ---";
        // qDebug() << QString::fromUtf8(responseData).toStdString();
        this->status = 0;
        this->errorMsg = "";

        // Nettoyage
        reply->deleteLater();
        emit finished();

    }
    else {
        qCritical().noquote() << "Erreur :" << reply->errorString();
        this->errorMsg = reply->errorString();
        this->status = -1;
        QString msg("");
        msg += "<b>Erreur de connexion à l'API :</b> ";
        msg += "<br><br>Pensez à vérifier la configuration (host/port)...";
        emit errorReachingApiServer(msg);

        // Nettoyage
        reply->deleteLater();
        emit finished();

    }
}
void ApiClient::sendPostEmployeeRequest(const std::string &json) {
    QString apiURL = "http://" + this->host+":"+QString::number(this->port)+"/api/employees";
    QUrl url(apiURL);
    QNetworkRequest request(url);

    // Définir les headers HTTP indispensables pour du JSON
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");

    // Convertir le std::string en QByteArray (garde les octets UTF-8 tels quels)
    QByteArray data = QByteArray::fromStdString(json);
    QNetworkReply *reply = this->networkManager->post(request, data);

    // Gérer la réponse de manière asynchrone
    QObject::connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseD = reply->readAll();
            QJsonParseError parseError;
            QJsonDocument doc{QJsonDocument::fromJson(responseD, &parseError)};

          // Vérification des erreurs de parsing
            if (parseError.error != QJsonParseError::NoError) {
                qWarning() << "Erreur de parsing JSON :" << parseError.errorString();
                this->status = -1;
            }

            // Vérifier qu'il s'agit bien d'un tableau JSON (Array)
            if (doc.isArray()) {
                QJsonArray jsonArray = doc.array();

                // Vérifier qu'on a bien au moins 1 élément
                if (jsonArray.size() >= 1) {

                    QJsonObject obj = jsonArray.at(0).toObject();

                    // Extraction des valeurs du premier objet :
                    int val = obj.value("id").toInt();

                    // Emet le signal connecté à mainwindow
                    emit employeeAdded(val);
                }
            }
        }
        else {
            qDebug() << "Erreur HTTP :" << reply->errorString();
            qDebug() << "Code statut HTTP :" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            qDebug() << "Détails réponse :" << reply->readAll();            
            this->status = -1;
            this->errorMsg = reply->errorString();
            QString msg("");
            msg += "<b>Erreur de connexion à l'API :</b> ";
            msg += "<br><br>Pensez à vérifier la configuration (host/port)...";
            emit errorReachingApiServer(msg);
        }

        // Très important en Qt : libérer la mémoire de la réponse
        reply->deleteLater();
    });
    this->status = 0;
}
void ApiClient::sendPutEmployeeRequest(const std::string &json, const int &id, const int row, const Employee &e) {
    QString u = "http://" + this->host+":"+QString::number(this->port)+"/api/employees/" + QString::number(id);
    QString apiURL(u);
    QUrl url(apiURL);
    QNetworkRequest request(url);

    // Définir les headers HTTP indispensables pour du JSON
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");

    // Convertir le std::string en QByteArray (garde les octets UTF-8 tels quels)
    QByteArray data = QByteArray::fromStdString(json);
    QNetworkReply *reply = this->networkManager->put(request, data);

    // Gérer la réponse de manière asynchrone
    QObject::connect(reply, &QNetworkReply::finished, [this, reply, row, e]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseD = reply->readAll();
            qDebug() << "Succès ! Réponse API :" << responseD;
            emit employeeModified(row, e);
        }
        else {
            qDebug() << "Erreur HTTP :" << reply->errorString();
            qDebug() << "Code statut HTTP :" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            qDebug() << "Détails réponse :" << reply->readAll();
            this->errorMsg = reply->errorString();
            this->status = -1;
            QString msg("");
            msg += "<b>Erreur de connexion à l'API :</b> ";
            msg += "<br><br>Pensez à vérifier la configuration (host/port)...";
            emit errorReachingApiServer(msg);
        }

        // Très important en Qt : libérer la mémoire de la réponse
        reply->deleteLater();
    });
    this->status = 0;
}
void ApiClient::sendGetConfigRequest() {
    QString apiURL = "http://" + this->host+":" + QString::number(this->port) + "/api/config";
    QUrl url(apiURL);
    QNetworkRequest request(url);

    // Définir les headers HTTP indispensables pour du JSON
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");

    // Convertir le std::string en QByteArray (garde les octets UTF-8 tels quels)
    QNetworkReply *reply = this->networkManager->get(request);


    // Création d'une boucle d'événements locale
    QEventLoop loop;

    // Connexion de la fin de la requête pour débloquer la boucle
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    // Bloque l'exécution ICI jusqu'à ce que loop.quit() soit appelé
    loop.exec();

    // 4. Traitement de la réponse UNE FOIS LA REQUÊTE TERMINÉE
    if (reply->error() == QNetworkReply::NoError) {
        this->responseData = reply->readAll();

        QJsonParseError parseError;
        QJsonDocument doc{QJsonDocument::fromJson(responseData, &parseError)};

        // Vérification des erreurs de parsing
        if (parseError.error != QJsonParseError::NoError) {
            qWarning() << "Erreur de parsing JSON :" << parseError.errorString();
            this->status = -1;
            this->errorMsg =  parseError.errorString();
        }

        // Vérifier qu'il s'agit bien d'un tableau JSON (Array)
        if (doc.isArray()) {
            QJsonArray jsonArray = doc.array();

            // Vérifier qu'on a bien au moins 1 élément
            if (jsonArray.size() >= 1) {

                QJsonObject obj = jsonArray.at(0).toObject();

                //Utilisation d'une boucle range-based moderne (C++17/20 & Qt6)
                for (auto it = obj.constBegin(); it != obj.constEnd(); ++it) {
                    QString key = it.key();
                    QString valueStr = it.value().toVariant().toString(); // toVariant universel pour convertir même les nombres

                    qDebug().noquote() << key << " = " << valueStr;
                }
            }
        }

        // Nettoyage
        reply->deleteLater();
        emit finished();
        this->status = 0;
    }
    else {
        this->errorMsg = reply->errorString();
        this->status = -1;
        qCritical().noquote() << "Erreur :" << errorMsg;

        // Nettoyage
        reply->deleteLater();
        QString msg("");
        msg += "<b>Erreur de connexion à l'API :</b> ";
        msg += "<br><br>Pensez à vérifier la configuration (host/port)...";
        emit errorReachingApiServer(msg);

    }
}
void ApiClient::sendPutConfigRequest(const std::string &json) {
    QString apiURL = "http://" + this->host+":"+QString::number(this->port)+"/api/config";
    QUrl url(apiURL);
    QNetworkRequest request(url);

    // Définir les headers HTTP indispensables pour du JSON
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");

    // Convertir le std::string en QByteArray (garde les octets UTF-8 tels quels)
    QByteArray data = QByteArray::fromStdString(json);
    QNetworkReply *reply = this->networkManager->put(request, data);

    // Gérer la réponse de manière asynchrone
    QObject::connect(reply, &QNetworkReply::finished, [this, reply, json]() {
        if (reply->error() == QNetworkReply::NoError) {
            emit configModified(json);
        }
        else {
            qDebug() << "Erreur HTTP :" << reply->errorString();
            qDebug() << "Code statut HTTP :" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            qDebug() << "Détails réponse :" << reply->readAll();
            this->errorMsg = reply->errorString();
            this->status = -1;
            QString msg("");
            msg += "<b>Erreur de connexion à l'API :</b> ";
            msg += "<br><br>Pensez à vérifier la configuration (host/port)...";
            emit errorReachingApiServer(msg);
        }

        // Très important en Qt : libérer la mémoire de la réponse
        reply->deleteLater();
    });
    this->status = 0;
}

// setters
void ApiClient::setHost(const QString &newhost) {
    this->host = newhost;
}
void ApiClient::setPort(const int &port) {
    this->port = port;
}

// getters
int ApiClient::getPort() {
    return this->port;
}
QString ApiClient::getHost() {
    return this->host;
}
QByteArray ApiClient::getResponseData() {
    return this->responseData;
}
int ApiClient::getStatus() {
    return this->status;
}
QString ApiClient::getMsg() {
    return this->errorMsg;
}





QMap<QString, QString> ApiClient::loadConfig(const QString &cheminFichier) {
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

QString ApiClient::getConfigPath(const QString &nomFichier) {
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

bool ApiClient::saveConfig(const QString &cheminFichier, const QMap<QString, QString> &map) {
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

