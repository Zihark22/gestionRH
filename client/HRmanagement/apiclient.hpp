#ifndef APICLIENT_HPP
#define APICLIENT_HPP

#include "iniparser.hpp"

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QDebug>
#include <QEventLoop> // Ne pas oublier cet include pour requete bloquante
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QResource> // pour lecture raw de config.ini

class ApiClient : public QObject {
    Q_OBJECT

public:
    explicit ApiClient(QObject *parent = nullptr);

    // requests
    int sendGetEmployeeRequest(int id); // get employees id=0 for all
    int sendPostEmployeeRequest(const std::string &json); // add employee
    int sendPutEmployeeRequest(const std::string &json, const int &id);  // modify employee
    int sendGetConfigRequest(); // get config
    int sendPutConfigRequest(const std::string &json); // put config

    // getters
    int getStatus();
    QByteArray getResponseData();

signals:
    // Signal émis quand la requête est terminée pour indiquer à l'application qu'elle peut quitter
    void finished();
    void employeeAdded(int newId);

private:
    QNetworkAccessManager *networkManager;
    QByteArray responseData;
    int status;
    int port = 8080;
    QString host = "localhost"; // 127.0.0.1

    void load_config(const std::string& file_path);

};

#endif // APICLIENT_HPP
