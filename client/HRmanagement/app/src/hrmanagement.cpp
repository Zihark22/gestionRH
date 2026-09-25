#include "include/hrmanagement.hpp"

#include "include/parameters.hpp"
#include "include/formwindow.hpp"
#include "include/configserverwindow.hpp"
#include "include/configappwindow.hpp"

HRmanagement::HRmanagement(QWidget *parent) : QObject(parent) {
    apiClient = std::make_unique<ApiClient>();
}

void HRmanagement::start() {
    // Connect the asynchronous API signals to the handlers
    connect(apiClient.get(), &ApiClient::employeeAdded, this, &HRmanagement::onEmployeeAdd);
    connect(apiClient.get(), &ApiClient::employeeModified, this, &HRmanagement::onEmployModify);
    connect(apiClient.get(), &ApiClient::configModified, this, &HRmanagement::onConfigModified);
    connect(apiClient.get(), &ApiClient::errorReachingApiServer, this, &HRmanagement::errorDetected);
}


/********* Slots *********/

void HRmanagement::onEmployeeAdd(const uint &id) {
    employees.back().setId(id);
    QString manager = get_manager_name(get_employee_from_id(id).managerId());
    emit onEmployeeAdded(employees, manager);
}
void HRmanagement::onEmployModify(const int &row, const Employee &e) {
    QString manager = get_manager_name(e.managerId());
    emit onEmployeeModified(row, e, manager);
}
void HRmanagement::onConfigModified(const std::string json) {
    qDebug() << "Configuration modified on the server side: " << json;

    QJsonParseError parseError;
    QJsonDocument doc{QJsonDocument::fromJson(QString::fromStdString(json).toUtf8(), &parseError)};

    // Check for parsing errors
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parsing error:" << parseError.errorString();
        return;
    }

    // Verify that the payload is a JSON array
    if (doc.isArray()) {
        QJsonArray jsonArray = doc.array();

        // Extract the available values when present
        if (jsonArray.size() >= 1) {
            QJsonObject obj = jsonArray.at(0).toObject();
            apiClient.get()->setPort(obj.value(PORT_KEY).toInt());
            apiClient.get()->setHost(obj.value(HOST_KEY).toString());
        }
    }
}


/********* Getters *********/

QString HRmanagement::get_manager_name(const uint &manager_id) {
    for (const Employee &e : employees) {
        uint id = e.id();
        if(id==manager_id)
        {
            QString lastname = e.lastname();
            QString firstname = e.firstname();
            return firstname + " " + lastname;
        }
    }
    return "Aucun renseigné";
}

Employee HRmanagement::get_employee_from_id(const uint &employee_id) {
    for (const Employee &e : employees) {
        uint id = e.id();
        if(id==employee_id)
        {
            return e;
        }
    }
    throw std::invalid_argument("L'ID de l'employé ne fait pas partie de la liste d'employés : " + to_string(employee_id));
}


/********* Data *********/

void HRmanagement::loadData() {
    apiClient->sendGetEmployeeRequest(0); // Block while loading the initial data

    if(apiClient->getStatus()!=0){
        QString msg("");
        msg += "<b>Erreur de connexion</b> : ";
        msg += apiClient->getMsg();
        msg += "<br/><br/>Pensez à vérifier la configuration (host/port)...";
        emit errorDetected(msg);
    }
    else {
        parseMyJson();
        extractManagers();
        emit employeesListUpdated(employees);
    }
}

void HRmanagement::parseMyJson() {
    // Clear the list before reloading data
    employees.clear();

    // Parse the response payload
    QJsonParseError parseError;
    // QJsonDocument expects a UTF-8 QByteArray
    QJsonDocument doc = QJsonDocument::fromJson(apiClient.get()->getResponseData(), &parseError);

    // Check for parsing errors
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parsing error:" << parseError.errorString();
        return;
    }

    // Verify that the payload is a valid JSON array
    if (doc.isArray()) {
        QJsonArray jsonArray = doc.array();

        // Convert each object into an Employee instance
        for (int i = 0; i < jsonArray.size(); ++i) {
            QJsonObject empl_json = jsonArray.at(i).toObject();
            QJsonDocument empl_array(empl_json);
            std::string e_str = QString::fromUtf8(empl_array.toJson(QJsonDocument::Compact)).toStdString();
            e_str = "[" + e_str + "]";
            Employee e(e_str);
            employees.append(e);
        }
    }
}

void HRmanagement::extractManagers() {
    QString fullname = "";
    managers.clear();
    managers.append({0, fullname});

    for(int i=0; i<employees.size(); i++) {
        Employee e = employees[i];
        fullname = e.lastname() + " " + e.firstname();
        if(e.isExecutive())
            managers.append({e.id(),fullname});
    }
    std::sort(managers.begin(), managers.end(), [](const QPair<int, QString> &m1, const QPair<int, QString> &m2) {
        return m1.second < m2.second;
    });
}


/********* Dialog windows *********/

void HRmanagement::addingEmployee() {
    // Instantiate the dialog with this as the parent
    FormWindow dialog(managers);

    // The dialog is modal and blocks until the user confirms
    if (dialog.exec() == QDialog::Accepted)
    {
        // The user clicked validate
        Employee e = dialog.toEmployee();
        std::string json = "[" + e.toJson() + "]";
        employees.append(e);

        // Send the new employee payload to the backend
        apiClient.get()->sendPostEmployeeRequest(json);
    }
    else
    {
        // The user cancelled the creation form
    }
}
void HRmanagement::openConfigServerWindow() {
    ConfigServerWindow configserv;

    if (configserv.exec() == QDialog::Accepted) {
        // The user clicked validate
        QString json = configserv.toJson();
        qDebug() << "Validated input:" << json;
        apiClient.get()->sendPutConfigRequest(json.toStdString());
    }
    else {
        // The user cancelled the configuration form
        qDebug() << "Input cancelled";
    }
}
void HRmanagement::openConfigAppWindow() {
    ConfigAppWindow configapp(this->apiClient.get()->getPort(), apiClient.get()->getHost());

    if (configapp.exec() == QDialog::Accepted) {
        // The user clicked validate
        qDebug() << "Requested config on http://" << configapp.getHost() << ":" << configapp.getPort();
        apiClient.get()->setHost(configapp.getHost());
        apiClient.get()->setPort(configapp.getPort());

        // Save the new configuration to the INI file
        QMap<QString, QString> map;
        map[HOST_KEY] = configapp.getHost();
        map[PORT_KEY] = QString::fromStdString(to_string(configapp.getPort()));
        if(ApiClient::saveConfig(CONFIG_FILE_NAME, map))
            qDebug() << "Configuration updated successfully";
        else{
            qDebug() << "Error while saving the new config";
            emit errorDetected("Error while saving the new config");
        }
        loadData();
    }
    else {
        // The user cancelled the configuration form
        qDebug() << "Input cancelled";
    }
}
void HRmanagement::openLogs() {

}
void HRmanagement::openEditEmployeeWindow(const uint &id, const int &row) {

    // Find the matching employee in the list
    auto it = std::find_if(employees.begin(), employees.end(), [id](const Employee &c) {
        return c.id() == id;
    });

    if (it != employees.end()) {

        // Open the form prefilled with the employee data
        Employee e = *it; // Copy of the object to edit
        FormWindow dialog(e, managers); // open form

        // The form is modal and blocks until validation or cancellation
        if (dialog.exec() == QDialog::Accepted) {
            // The user clicked validate
            Employee e = dialog.toEmployee();
            std::string json = "[" + e.toJson() + "]";
            *it = e;

            // Send the updated employee payload to the backend
            apiClient.get()->sendPutEmployeeRequest(json, id, row, e);
        }
        else {
            // The user cancelled the edit form
        }
    }
}
