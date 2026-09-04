#include "apiclient.hpp"

ApiClient::ApiClient(QObject *parent) : QObject(parent) {
    this->networkManager = new QNetworkAccessManager(this);
    this->status = this->sendGetEmployeeRequest(0); // load everything
}
QByteArray ApiClient::getResponseData() {
    return responseData;
}
int ApiClient::getStatus() {
    return this->status;
}
int ApiClient::sendGetEmployeeRequest(int id) {
    QString apiURL = "http://127.0.0.1:8080/api/employees";
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
    QString apiURL = "http://127.0.0.1:8080/api/employees";
    QUrl url(apiURL);
    QNetworkRequest request(url);

    // 2. Définir les headers HTTP indispensables pour du JSON
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");

    // 3. Convertir le std::string en QByteArray (garde les octets UTF-8 tels quels)
    QByteArray data = QByteArray::fromStdString(json);
    QNetworkReply *reply = networkManager->post(request, data);

    // 5. Gérer la réponse de manière asynchrone
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

            // 3. Vérifier qu'il s'agit bien d'un tableau JSON (Array)
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
    std::string u = "http://127.0.0.1:8080/api/employees/" + std::to_string(id);
    QString apiURL(u.c_str());
    QUrl url(apiURL);
    QNetworkRequest request(url);

    // 2. Définir les headers HTTP indispensables pour du JSON
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json; charset=utf-8");

    // 3. Convertir le std::string en QByteArray (garde les octets UTF-8 tels quels)
    QByteArray data = QByteArray::fromStdString(json);
    QNetworkReply *reply = networkManager->put(request, data);

    // 5. Gérer la réponse de manière asynchrone
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
