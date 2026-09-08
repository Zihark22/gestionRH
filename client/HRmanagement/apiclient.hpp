#ifndef APICLIENT_HPP
#define APICLIENT_HPP

#include "iniparser.hpp"
#include "employee.hpp"

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
    void sendGetEmployeeRequest(int id); // get employees id=0 for all
    void sendPostEmployeeRequest(const std::string &json); // add employee
    void sendPutEmployeeRequest(const std::string &json, const int &id, const int row, const Employee &e);  // modify employee
    void sendGetConfigRequest(); // get config
    void sendPutConfigRequest(const std::string &json); // put config

    // getters
    int getStatus();
    QByteArray getResponseData();
    int getPort();
    QString getHost();
    QString getMsg();

    // setters
    void setHost(const QString &host);
    void setPort(const int &port);

signals:
    // Signal émis quand la requête est terminée pour indiquer à l'application qu'elle peut quitter
    void finished();
    void employeeAdded(int newId);
    void employeeModified(const int row, const Employee &e);
    void configModified(const std::string &json);
    void errorReachingApiServer();

private:
    QNetworkAccessManager *networkManager;
    QByteArray responseData;
    QString errorMsg;
    int status; // 0 for OK and error for other
    int port;
    QString host;

    void load_config(const std::string& file_path);

};

#endif // APICLIENT_HPP
