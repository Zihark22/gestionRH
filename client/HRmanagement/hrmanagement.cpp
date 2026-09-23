#include "hrmanagement.hpp"

#include "parameters.hpp"
#include "formwindow.hpp"
#include "configserverwindow.hpp"
#include "configappwindow.hpp"

HRmanagement::HRmanagement(QWidget *parent) : QObject(parent) {
    apiClient = std::make_unique<ApiClient>();
}

void HRmanagement::start() {

    // Connexion des siganux asynchrones aux méthodes
    connect(apiClient.get(), &ApiClient::employeeAdded, this, &HRmanagement::onEmployeeAdd);
    connect(apiClient.get(), &ApiClient::employeeModified, this, &HRmanagement::onEmployModify);
    connect(apiClient.get(), &ApiClient::configModified, this, &HRmanagement::onConfigModified);
    connect(apiClient.get(), &ApiClient::errorReachingApiServer, this, &HRmanagement::errorDetected);
}
void HRmanagement::onEmployeeAdd(const int &id) {
    employees.back().setId(id);
    QString manager = get_manager_name(get_employee_from_id(id).managerId());
    emit onEmployeeAdded(id, employees, manager);
}
void HRmanagement::onEmployModify(const int &id, const Employee &e) {
    QString manager = get_manager_name(e.managerId());
    emit onEmployeeModified(id, e, manager);
}


// load data by recreating tabs and getting all DB
void HRmanagement::loadData() {
    apiClient->sendGetEmployeeRequest(0); // bloquant pour charger les données avant

    if(apiClient->getStatus()!=0){
        QString msg("");
        msg += "<b>Erreur de connexion</b> : ";
        msg += apiClient->getMsg();
        msg += "<br/><br/>Pensez à vérifier la configuration (host/port)...";
        emit errorDetected(msg);
    }
    else {
        // Save employees list
        parseMyJson();

        // Extract managers list
        extractManagers();

        emit employeesListUpdated(employees);
    }
}


// Get manager name on employee ID
QString HRmanagement::get_manager_name(const int &manager_id) {
    for (const Employee &e : employees) {
        int id = e.id();
        if(id==manager_id)
        {
            QString lastname = e.lastname();
            QString firstname = e.firstname();
            return firstname + " " + lastname;
        }
    }
    return "Aucun renseigné";
}

Employee HRmanagement::get_employee_from_id(const int &employee_id) {
    for (const Employee &e : employees) {
        int id = e.id();
        if(id==employee_id)
        {
            return e;
        }
    }
    throw std::invalid_argument("L'ID de l'employé ne fait pas partie de la liste d'employés : " + to_string(employee_id));
}

void HRmanagement::parseMyJson() {
    // clear for reload data
    employees.clear();

    // Parser le QString
    QJsonParseError parseError;
    // QJsonDocument attend un QByteArray en UTF-8
    QJsonDocument doc = QJsonDocument::fromJson(apiClient.get()->getResponseData(), &parseError);

    // Vérification des erreurs de parsing
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Erreur de parsing JSON :" << parseError.errorString();
        return;
    }

    // 3. Vérifier qu'il s'agit bien d'un tableau JSON (Array)
    if (doc.isArray()) {
        QJsonArray jsonArray = doc.array();

        // Vérifier qu'on a bien au moins 2 éléments
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
    managers.append({-1, fullname});

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

// Création du Formulaire d'ajout
void HRmanagement::addingEmployee() {
    // Instanciation du dialogue avec 'this' en parent
    FormWindow dialog(managers);

    // .exec() rend la fenêtre MODALE et bloque le flux jusqu'à la fermerture
    if (dialog.exec() == QDialog::Accepted)
    {
        // L'utilisateur a cliqué sur "Valider"
        Employee e = dialog.toEmployee();
        // qDebug() << "Formualire saisie :" << e.toJson();
        std::string json = "[" + e.toJson() + "]";
        employees.append(e);

        // Appel du service HTTP pour envoyer le JSON au serveur
        apiClient.get()->sendPostEmployeeRequest(json);
    }
    else
    {
        // L'utilisateur a cliqué sur "Annuler" ou fermé la fenêtre
        // Ajouter une fenêtre de confirmation
        // std::cout << "Saisie annulée" << std::endl;
    }
}



void HRmanagement::openConfigServerWindow() {
    ConfigServerWindow configserv;

    if (configserv.exec() == QDialog::Accepted) {
        // L'utilisateur a cliqué sur "Valider"
        QString json = configserv.toJson();
        qDebug() << "Saisie validée :" << json;
        apiClient.get()->sendPutConfigRequest(json.toStdString());
    }
    else {
        // L'utilisateur a cliqué sur "Annuler" ou fermé la fenêtre
        qDebug() << "Saisie annulée";
    }
}
void HRmanagement::openConfigAppWindow() {
    ConfigAppWindow configapp(this->apiClient.get()->getPort(), apiClient.get()->getHost());

    if (configapp.exec() == QDialog::Accepted) {
        // L'utilisateur a cliqué sur "Valider"
        qDebug() << "Config désirée sur http://" << configapp.getHost() << ":" << configapp.getPort();
        apiClient.get()->setHost(configapp.getHost());
        apiClient.get()->setPort(configapp.getPort());

        // Sauvegarde de la nouvelle config dans le fichier config.ini
        QMap<QString, QString> map;
        map[HOST_KEY] = configapp.getHost();
        map[PORT_KEY] = QString::fromStdString(to_string(configapp.getPort()));
        if(ApiClient::saveConfig(CONFIG_FILE_NAME, map))
            qDebug() << "Configuration modifiée avec succès";
        else{
            qDebug() << "Erreur de sauvegarde de la nouvelle config";
            emit errorDetected("Erreur de sauvegarde de la nouvelle config");
        }
        loadData();
    }
    else {
        // L'utilisateur a cliqué sur "Annuler" ou fermé la fenêtre
        qDebug() << "Saisie annulée";
    }
}
void HRmanagement::onConfigModified(const std::string json) {
    qDebug() << "Configuration modifié côté serveur : " << json;

    QJsonParseError parseError;
    QJsonDocument doc{QJsonDocument::fromJson(QString::fromStdString(json).toUtf8(), &parseError)};

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
            apiClient.get()->setPort(obj.value(PORT_KEY).toInt());
            apiClient.get()->setHost(obj.value(HOST_KEY).toString());
        }
    }
}
void HRmanagement::openLogs() {

}
void HRmanagement::openEditEmployeeWindow(const int id, const int row) {

    // 3. Chercher le collaborateur correspondant dans votre QList
    auto it = std::find_if(employees.begin(), employees.end(), [id](const Employee &c) {
        return c.id() == id;
    });

    if (it != employees.end()) {

            // 4. Ouvrir le formualire remplit avec les données de l'employé
        Employee e = *it; // Copie de l'objet à modifier
        FormWindow dialog(e, managers); // ouvre formulaire

        // .exec() rend la fenêtre MODALE et bloque le flux jusqu'à la fermerture
        if (dialog.exec() == QDialog::Accepted) {
            // L'utilisateur a cliqué sur "Valider"
            Employee e = dialog.toEmployee();
            std::string json = "[" + e.toJson() + "]";
            // qDebug() << "Formualire saisie :" << e.toJson();
            *it = e;

            // Appel du service HTTP pour envoyer le JSON au serveur
            apiClient.get()->sendPutEmployeeRequest(json, id, row, e);
        }
        else {
            // L'utilisateur a cliqué sur "Annuler" ou fermé la fenêtre
            // std::cout << "Saisie annulée" << std::endl;
        }
    }
}
