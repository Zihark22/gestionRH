#include "include/apiclient.hpp"
#include "include/parameters.hpp"

ApiClient::ApiClient(QObject *parent) : QObject(parent) {
    this->networkManager = new QNetworkAccessManager(this);
    this->configure("config.ini");
}


/********* Requests *********/

void ApiClient::sendGetEmployeeRequest(const int &id) {
    QString apiURL = "http://" + this->host+":"+QString::number(this->port)+"/api/employees";
    if(id>0)
        apiURL += "/"+std::to_string(id);
    QUrl url(apiURL);
    QNetworkRequest request(url);
    QNetworkReply *reply = this->networkManager->get(request);

    // The startup flow blocks while the request completes

    // Create a local event loop
    QEventLoop loop;

    // Stop the loop when the reply is finished
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    // Block execution until the reply is complete
    loop.exec();

    // Process the response once the request is complete
    if (reply->error() == QNetworkReply::NoError) {
        this->responseData = reply->readAll();

        this->status = 0;
        this->errorMsg = "";

        // Clean up the reply object
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

        // Clean up the reply object
        reply->deleteLater();
        emit finished();

    }
}
void ApiClient::sendPostEmployeeRequest(const std::string &json) {
    QString apiURL = "http://" + this->host+":"+QString::number(this->port)+"/api/employees";
    QUrl url(apiURL);
    QNetworkRequest request(url);

    // Set the HTTP headers required for JSON
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");

    // Encode the JSON string as UTF-8 data
    QByteArray data = QByteArray::fromStdString(json);
    QNetworkReply *reply = this->networkManager->post(request, data);

    // Handle the response asynchronously
    QObject::connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseD = reply->readAll();
            QJsonParseError parseError;
            QJsonDocument doc{QJsonDocument::fromJson(responseD, &parseError)};

            // Check for JSON parsing errors
            if (parseError.error != QJsonParseError::NoError) {
                qWarning() << "JSON parsing error:" << parseError.errorString();
                this->status = -1;
            }

            // Verify that the payload is a JSON array
            if (doc.isArray()) {
                QJsonArray jsonArray = doc.array();

                // Verify that the array contains at least one item
                if (jsonArray.size() >= 1) {

                    QJsonObject obj = jsonArray.at(0).toObject();

                    // Extract the values from the first object:
                    uint val = static_cast<uint>(obj.value("id").toInt());

                    // Emit the signal connected to the main window
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

        // Free the reply memory
        reply->deleteLater();
    });
    this->status = 0;
}
void ApiClient::sendPutEmployeeRequest(const std::string &json, const uint &id, const int &row, const Employee &e) {
    QString u = "http://" + this->host+":"+QString::number(this->port)+"/api/employees/" + QString::number(id);
    QString apiURL(u);
    QUrl url(apiURL);
    QNetworkRequest request(url);

    // Set the HTTP headers required for JSON
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");

    // Encode the JSON string as UTF-8 data
    QByteArray data = QByteArray::fromStdString(json);
    QNetworkReply *reply = this->networkManager->put(request, data);

    // Handle the response asynchronously
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

        // Free the reply memory
        reply->deleteLater();
    });
    this->status = 0;
}
void ApiClient::sendGetConfigRequest() {
    QString apiURL = "http://" + this->host+":" + QString::number(this->port) + "/api/config";
    QUrl url(apiURL);
    QNetworkRequest request(url);

    // Set the HTTP headers required for JSON
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");

    // Encode the JSON string as UTF-8 data
    QNetworkReply *reply = this->networkManager->get(request);


    // Create a local event loop
    QEventLoop loop;

    // Stop the loop once the request finishes
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    // Block execution until the reply is complete
    loop.exec();

    // Process the response once the request is complete
    if (reply->error() == QNetworkReply::NoError) {
        this->responseData = reply->readAll();

        QJsonParseError parseError;
        QJsonDocument doc{QJsonDocument::fromJson(responseData, &parseError)};

        // Check for JSON parsing errors
        if (parseError.error != QJsonParseError::NoError) {
            qWarning() << "JSON parsing error:" << parseError.errorString();
            this->status = -1;
            this->errorMsg =  parseError.errorString();
        }

        // Verify that the payload is a JSON array
        if (doc.isArray()) {
            QJsonArray jsonArray = doc.array();

            // Verify that the array contains at least one item
            if (jsonArray.size() >= 1) {

                QJsonObject obj = jsonArray.at(0).toObject();

                // Parse the available object values
                for (auto it = obj.constBegin(); it != obj.constEnd(); ++it) {
                    QString key = it.key();
                    QString valueStr = it.value().toVariant().toString(); // toVariant handles conversion of numbers as well

                    qDebug().noquote() << key << " = " << valueStr;
                }
            }
        }

        // Clean up the reply object
        reply->deleteLater();
        emit finished();
        this->status = 0;
    }
    else {
        this->errorMsg = reply->errorString();
        this->status = -1;
        qCritical().noquote() << "Erreur :" << errorMsg;

        // Clean up the reply object
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

    // Set the HTTP headers required for JSON
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");

    // Encode the JSON string as UTF-8 data
    QByteArray data = QByteArray::fromStdString(json);
    QNetworkReply *reply = this->networkManager->put(request, data);

    // Handle the response asynchronously
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

        // Very important in Qt: free the response memory
        reply->deleteLater();
    });
    this->status = 0;
}

/********* Setters *********/

void ApiClient::setHost(const QString &newhost) {
    this->host = newhost;
}
void ApiClient::setPort(const int &port) {
    this->port = port;
}


/********* Getters *********/

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


/********* Handle config *********/

void ApiClient::configure(const std::string& file_path) {
    QString cheminConfig = ApiClient::getConfigPath(QString::fromStdString(file_path));
    QMap<QString, QString> config = ApiClient::loadConfig(cheminConfig);
    this->port = config["port"].toInt();
    this->host = config["host"];
}

QMap<QString, QString> ApiClient::loadConfig(const QString &cheminFichier) {
    QSettings settings(cheminFichier, QSettings::IniFormat);
    QMap<QString, QString> map;

    // 1. Read a root key directly, with a default value if it is missing
    QString version = settings.value("Version", APP_VERSION).toString();
    qDebug() << "Version de l'application :" << version;

    // 2. Read from the [Server] group
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
    // Location where the real editable file will be stored (next to the executable)
    QString cheminDisque = QCoreApplication::applicationDirPath() + "/" + nomFichier;

    // If the file does not exist on disk yet, copy it from the resources
    if (!QFile::exists(cheminDisque)) {
        qDebug() << "First launch: extracting the default config...";

        // Copy from the resources (":/config.ini" or the configured prefix in your .qrc)
        QString configFile = ":/";
        configFile += CONFIG_FILE_NAME;
        if (QFile::copy(configFile, cheminDisque)) {
            // IMPORTANT: copying a resource inherits read-only permissions
            // Grant write permissions to the user:
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
    // File in the same folder as the executable:
    QString path = QCoreApplication::applicationDirPath() + "/" + cheminFichier;

    // 1. Instancier QSettings avec le format IniFormat
    QSettings settings(cheminFichier, QSettings::IniFormat);

    // if (settings.contains("Server/host")) {
    //     qDebug() << "Server existe";
    // }

    // Write a root key (without a group)
    settings.setValue("Version", APP_VERSION);

    // 2. Write to a [Server] section
    settings.beginGroup(SERVER_GROUP_NAME);
    settings.setValue(HOST_KEY, map[HOST_KEY]);
    settings.setValue(PORT_KEY, map[PORT_KEY]);
    settings.endGroup(); // Fin de la section [Server]

    // Optional: ensure immediate disk writes
    settings.sync();
    return 1;
}

