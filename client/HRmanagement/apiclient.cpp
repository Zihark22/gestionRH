#include "apiclient.hpp"

ApiClient::ApiClient(QObject *parent) : QObject(parent) {
    this->networkManager = new QNetworkAccessManager(this);
    this->load_config(":/config.ini");
    this->status = this->sendGetConfigRequest();
    this->status = this->sendGetEmployeeRequest(0); // load everything
}
void ApiClient::load_config(const std::string& file_path) {
    QResource res(QString::fromStdString(file_path));
    if (res.isValid()) {
        // res.data() renvoie un const uchar* directement mappé dans l'exécutable
        std::string_view content(reinterpret_cast<const char*>(res.data()), res.size());

        // Vous passez le contenu au parser standard C++
        auto config = IniParser::parseFromString(content);

        for (const auto& section : config) {
            if (section.nom == "Server") {
                if (auto var = section.getVariable("port")) {
                    try {
                        this->port = stoi(var->valeur);
                    } catch (...) {
                        qDebug() << "[loadConfig] Invalid port value: " << var->valeur;
                    }
                }
                if (auto var = section.getVariable("host")) {
                    this->host = QString::fromStdString(var->valeur);
                }
                qDebug() << "[Client] Config loaded: host=" << (host.isEmpty()?"(none)":host) << " port=" << port;
            }
        }
    } else {
        qWarning() << "Impossible de trouver la ressource :/file.in";
    }
}
QByteArray ApiClient::getResponseData() {
    return responseData;
}
int ApiClient::getStatus() {
    return this->status;
}
int ApiClient::sendGetEmployeeRequest(int id) {
    QString apiURL = "http://"+host+":"+QString::number(port)+"/api/employees";
    if(id>0)
        apiURL += "/"+std::to_string(id);
    QUrl url(apiURL);
    QNetworkRequest request(url);
    QNetworkReply *reply = networkManager->get(request);

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
        responseData = reply->readAll();

        // // Succès : Affichage dans la console
        // qDebug() << "\n--- Réponse du serveur ---";
        // qDebug() << QString::fromUtf8(responseData).toStdString();

        // Nettoyage
        reply->deleteLater();
        emit finished();

        return 0;
    } else {
        qCritical().noquote() << "Erreur :" << reply->errorString();

        // Nettoyage
        reply->deleteLater();
        emit finished();

        return -1;
    }
}
int ApiClient::sendPostEmployeeRequest(const std::string &json) {
    QString apiURL = "http://"+host+":"+QString::number(port)+"/api/employees";
    QUrl url(apiURL);
    QNetworkRequest request(url);

    // Définir les headers HTTP indispensables pour du JSON
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");

    // Convertir le std::string en QByteArray (garde les octets UTF-8 tels quels)
    QByteArray data = QByteArray::fromStdString(json);
    QNetworkReply *reply = networkManager->post(request, data);

    // Gérer la réponse de manière asynchrone
    QObject::connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseD = reply->readAll();
            QJsonParseError parseError;
            QJsonDocument doc{QJsonDocument::fromJson(responseD, &parseError)};

          // Vérification des erreurs de parsing
            if (parseError.error != QJsonParseError::NoError) {
                qWarning() << "Erreur de parsing JSON :" << parseError.errorString();
                return;
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
        } else {
            qDebug() << "Erreur HTTP :" << reply->errorString();
            qDebug() << "Code statut HTTP :" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            qDebug() << "Détails réponse :" << reply->readAll();
        }

        // Très important en Qt : libérer la mémoire de la réponse
        reply->deleteLater();
    });
    return 0;
}
int ApiClient::sendPutEmployeeRequest(const std::string &json, const int &id) {
    QString u = "http://"+host+":"+QString::number(port)+"/api/employees/" + QString::number(id);
    QString apiURL(u);
    QUrl url(apiURL);
    QNetworkRequest request(url);

    // Définir les headers HTTP indispensables pour du JSON
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");

    // Convertir le std::string en QByteArray (garde les octets UTF-8 tels quels)
    QByteArray data = QByteArray::fromStdString(json);
    QNetworkReply *reply = networkManager->put(request, data);

    // Gérer la réponse de manière asynchrone
    QObject::connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseD = reply->readAll();
            qDebug() << "Succès ! Réponse API :" << responseD;
        } else {
            qDebug() << "Erreur HTTP :" << reply->errorString();
            qDebug() << "Code statut HTTP :" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            qDebug() << "Détails réponse :" << reply->readAll();
        }

        // Très important en Qt : libérer la mémoire de la réponse
        reply->deleteLater();
    });
    return 0;
}
int ApiClient::sendGetConfigRequest() {
    QString apiURL = "http://"+host+":"+QString::number(port)+"/api/config";
    QUrl url(apiURL);
    QNetworkRequest request(url);

    // Définir les headers HTTP indispensables pour du JSON
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");

    // Convertir le std::string en QByteArray (garde les octets UTF-8 tels quels)
    QNetworkReply *reply = networkManager->get(request);

    // Gérer la réponse de manière asynchrone
    QObject::connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseD = reply->readAll();
            QJsonParseError parseError;
            QJsonDocument doc{QJsonDocument::fromJson(responseD, &parseError)};

            // Vérification des erreurs de parsing
            if (parseError.error != QJsonParseError::NoError) {
                qWarning() << "Erreur de parsing JSON :" << parseError.errorString();
                return;
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
        } else {
            qDebug() << "Erreur HTTP :" << reply->errorString();
            qDebug() << "Code statut HTTP :" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            qDebug() << "Détails réponse :" << reply->readAll();
        }

        // Très important en Qt : libérer la mémoire de la réponse
        reply->deleteLater();
    });
    return 0;
}
int ApiClient::sendPutConfigRequest(const std::string &json) {
    QString apiURL = "http://"+host+":"+QString::number(port)+"/api/employees";
    QUrl url(apiURL);
    QNetworkRequest request(url);

    // Définir les headers HTTP indispensables pour du JSON
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");

    // Convertir le std::string en QByteArray (garde les octets UTF-8 tels quels)
    QByteArray data = QByteArray::fromStdString(json);
    QNetworkReply *reply = networkManager->post(request, data);

    // Gérer la réponse de manière asynchrone
    QObject::connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseD = reply->readAll();
            QJsonParseError parseError;
            QJsonDocument doc{QJsonDocument::fromJson(responseD, &parseError)};

            // Vérification des erreurs de parsing
            if (parseError.error != QJsonParseError::NoError) {
                qWarning() << "Erreur de parsing JSON :" << parseError.errorString();
                return;
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
        } else {
            qDebug() << "Erreur HTTP :" << reply->errorString();
            qDebug() << "Code statut HTTP :" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            qDebug() << "Détails réponse :" << reply->readAll();
        }

        // Très important en Qt : libérer la mémoire de la réponse
        reply->deleteLater();
    });
    return 0;
}
