#ifndef APICLIENT_HPP
#define APICLIENT_HPP

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QDebug>
#include <QEventLoop> // Ne pas oublier cet include pour requete bloquante

class ApiClient : public QObject {
    Q_OBJECT

public:
    explicit ApiClient(QObject *parent = nullptr);

    // requests
    int sendGetEmployeeRequest(int id); // get employees id=0 for all
    int sendPostEmployeeRequest(int id); // add employee
    int sendPutEmployeeRequest(int id);  // modify employee

    // getters
    int getStatus();
    QByteArray getResponseData();

signals:
    // Signal émis quand la requête est terminée pour indiquer à l'application qu'elle peut quitter
    void finished();

private:
    QNetworkAccessManager *networkManager;
    QByteArray responseData;
    int status;
};

#endif // APICLIENT_HPP
