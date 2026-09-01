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
