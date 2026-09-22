#ifndef APICLIENT_HPP
#define APICLIENT_HPP

#include "employee.hpp"


#include <QSettings>
#include <QDebug>
#include <QMap>
#include <QStandardPaths>
#include <QDir>
#include <QCoreApplication>

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
    void setHost(const QString &newhost);
    void setPort(const int &port);

    // Méthode de sauvegarde d'une configuration dans un fichier .ini
    static bool saveConfig(const QString &cheminFichier, const QMap<QString, QString> &map);

signals:
    // Signal émis quand la requête est terminée pour indiquer à l'application qu'elle peut quitter
    void finished();
    void employeeAdded(const int &id);
    void employeeModified(const int row, const Employee &e);
    void configModified(const std::string &json);
    void errorReachingApiServer(const QString &msg);

private:
    QNetworkAccessManager *networkManager;
    QByteArray responseData;
    QString errorMsg;
    int status; // 0 for OK and error for other
    int port;
    QString host;

    /// Config ///

    void configure(const std::string& file_path);

    //Méthode de lecture de fichier de configuration
    static QMap<QString, QString> loadConfig(const QString &cheminFichier);


    static QString getConfigPath(const QString &nomFichier);

};

#endif // APICLIENT_HPP
