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
#include <QEventLoop> // Keep this include for the blocking request loop
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QResource> // for raw reading of config.ini

/// API client used for server requests and local configuration handling
class ApiClient : public QObject {
    Q_OBJECT

public:
    /// Create the API client
    explicit ApiClient(QObject *parent = nullptr);

    /// Fetch one employee or the full list
    void sendGetEmployeeRequest(const int &id);

    /// Create a new employee from the provided JSON payload
    void sendPostEmployeeRequest(const std::string &json);

    /// Update an existing employee
    void sendPutEmployeeRequest(const std::string &json, const uint &id, const int &row, const Employee &e);

    /// Fetch the server configuration from the API
    void sendGetConfigRequest();

    /// Send an updated server configuration payload
    void sendPutConfigRequest(const std::string &json);

    /// Return the latest request status
    int getStatus();

    /// Return the raw HTTP response payload
    QByteArray getResponseData();

    /// Return the configured server port
    int getPort();

    /// Return the configured server host
    QString getHost();

    /// Return the last error message
    QString getMsg();

    /// Update the API host
    void setHost(const QString &newhost);

    /// Update the API port
    void setPort(const int &port);

    /// Save a configuration map into an INI file
    static bool saveConfig(const QString &cheminFichier, const QMap<QString, QString> &map);

signals:
    /// Emitted when a request completes
    void finished();

    /// Emitted after a new employee is created
    void employeeAdded(const int &id);

    /// Emitted after an employee is updated
    void employeeModified(const int &row, const Employee &e);

    /// Emitted when server configuration is updated
    void configModified(const std::string &json);

    /// Emitted when the API is unreachable or a request fails
    void errorReachingApiServer(const QString &msg);

private:
    QNetworkAccessManager *networkManager; ///< HTTP client manager
    QByteArray responseData; ///< Last raw API response payload
    QString errorMsg; ///< Last client error message
    int status; ///< Request status code
    int port; ///< Server port used for requests
    QString host; ///< Server host used for requests

    /// Load values from the INI file
    void configure(const std::string& file_path);

    /// Read configuration values from an INI file
    static QMap<QString, QString> loadConfig(const QString &cheminFichier);

    /// Resolve the writable path to the config file
    static QString getConfigPath(const QString &nomFichier);
};

#endif // APICLIENT_HPP
