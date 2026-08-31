
#include "apiclientwindow.hpp"


ApiClientWindow::ApiClientWindow(QWidget *parent) : QWidget(parent) {
    // 1. Initialisation de l'IHM
    this->setWindowTitle("Client Qt6 - Test API");
    this->resize(500, 400);

    QVBoxLayout *layout = new QVBoxLayout(this);

    fetchButton = new QPushButton("Récupérer les collaborateurs", this);
    resultTextEdit = new QTextEdit(this);
    resultTextEdit->setReadOnly(true);

    layout->addWidget(fetchButton);
    layout->addWidget(resultTextEdit);

    // 2. Initialisation du gestionnaire de réseau
    networkManager = new QNetworkAccessManager(this);

    // 3. Connexion du bouton au clic
    connect(fetchButton, &QPushButton::clicked, this, &ApiClientWindow::sendGetRequest);
}

void ApiClientWindow::sendGetRequest() {
    // Configuration de la requête HTTP
    QUrl url("http://127.0.0.1:8080/api/collaborateurs");
    QNetworkRequest request(url);

    resultTextEdit->setText("Envoi de la requête au serveur...");

    // Envoi asynchrone (ne bloque pas l'IHM)
    QNetworkReply *reply = networkManager->get(request);

    // Traitement de la réponse lorsque le serveur répond
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            // Succès : Lecture des données brutes reçues du serveur C++
            QByteArray responseData = reply->readAll();
            resultTextEdit->setText(QString::fromUtf8(responseData));
        } else {
            // Erreur de connexion ou HTTP (ex: Serveur éteint, 404, etc.)
            resultTextEdit->setText("Erreur : " + reply->errorString());
        }

        // Libération obligatoire de la mémoire liée à la réponse
        reply->deleteLater();
    });
}