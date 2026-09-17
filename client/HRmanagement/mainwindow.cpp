#include "mainwindow.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    setWindowTitle("ERP Scalian - RH management");
    resize(900, 800);

    // Connexion du signal d'ajout à une méthode
    connect(&apiClient, &ApiClient::employeeAdded, this, &MainWindow::onEmployeeAdded);
    connect(&apiClient, &ApiClient::employeeModified, this, &MainWindow::onEmployeeModified);
    connect(&apiClient, &ApiClient::configModified, this, &MainWindow::onConfigModified);
    connect(&apiClient, &ApiClient::errorReachingApiServer, this, &MainWindow::errorDisplay);

    QMenuBar *bar = menuBar();
    QMenu *appMenu = bar->addMenu("Application");
    QAction *reloadAction = appMenu->addAction(tr("Recharger"));
    reloadAction->setShortcut(QKeySequence(tr("Ctrl+R")));
    connect(reloadAction, &QAction::triggered, this, &MainWindow::reloadData);
    QAction *quitAction = appMenu->addAction(tr("&Quitter"));
    quitAction->setShortcuts(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    QMenu *paramMenu = bar->addMenu("&Paramètres");
    QAction *logsAction = paramMenu->addAction(tr("Ouvrir &Logs"));
    logsAction->setShortcut(QKeySequence(tr("Ctrl+L")));
    connect(logsAction, &QAction::triggered, this, &MainWindow::openLogs);
    QAction *configAppAction = paramMenu->addAction(tr("Config &App"));
    configAppAction->setShortcut(QKeySequence(tr("Ctrl+A")));
    connect(configAppAction, &QAction::triggered, this, &MainWindow::openConfigAppWindow);
    QAction *configServAction = paramMenu->addAction(tr("Config &Server"));
    configServAction->setShortcut(QKeySequence(tr("Ctrl+S")));
    connect(configServAction, &QAction::triggered, this, &MainWindow::openConfigServerWindow);

    // charger les données dans les onglets
    reloadData();
}

void MainWindow::parseMyJson() {
    // clear for reload data
    employees.clear();

    // Parser le QString
    QJsonParseError parseError;
    // QJsonDocument attend un QByteArray en UTF-8
    QJsonDocument doc = QJsonDocument::fromJson(apiClient.getResponseData(), &parseError);

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

void MainWindow::extractManagers() {
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

// Création du premier onglet (Tableau)
QWidget* MainWindow::createGeneralTab() {
    auto *tab = new QWidget(this);
    auto *layout = new QVBoxLayout(tab);

    generalTableWidget = fillGeneralTab(tab);

    auto *onLine = new QWidget(this);
    auto *onLineLayout = new QHBoxLayout(onLine);
    auto *addButton = new QPushButton("Ajouter", this);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addingEmployee);
    auto *exportButton = new QPushButton("Exporter", this);
    auto *cmptLabel = new QLabel("Nombre d'employés : ", this);
    counterGeneral = new QLabel(tr("%1").arg(employees.size()), this);

    onLineLayout->addWidget(addButton);
    onLineLayout->addWidget(exportButton);
    onLineLayout->addStretch();
    onLineLayout->addWidget(cmptLabel);
    onLineLayout->addWidget(counterGeneral);

    layout->addWidget(onLine);
    layout->addWidget(generalTableWidget);

    return tab;
}

// Remplissage du 1er tableau
QTableWidget* MainWindow::fillGeneralTab(QWidget* tab) {
    // Instanciation du tableau (3 lignes, 3 colonnes)
    QTableWidget *tableWidget=nullptr;

    tableWidget = new QTableWidget(employees.size(), 7, tab);
    tableWidget->setHorizontalHeaderLabels({"Prénom", "Naissance", "Poste","Statut cadre", "Position (Syntec)","Coefficient (Syntec)", "Début"});

    // 3. Préparation du QTableWidget
    tableWidget->clearContents();
    tableWidget->setRowCount(0); // Réinitialise les lignes

    int row = 0;

    // 4. Parcours de chaque employe
    for (const Employee &e : employees) {

        // Insertion d'une nouvelle ligne dans le tableWidget
        tableWidget->insertRow(row);

        QTableWidgetItem *firstname_widget = new QTableWidgetItem(e.firstname());
        firstname_widget->setTextAlignment(Qt::AlignCenter);

        // On stocke l'ID unique dans les données cachées du widget
        firstname_widget->setData(Qt::UserRole, e.id());

        // Ajout du widget
        tableWidget->setItem(row, 0, firstname_widget);

        QTableWidgetItem *cell = new QTableWidgetItem(e.birthdate().toString("yyyy-MM-dd"));
        cell->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 1, cell);

        cell = new QTableWidgetItem(e.job());
        cell->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 2, cell);


        QString executive_status_str = e.isExecutive() ? "Executive status" : "No executive status";
        cell = new QTableWidgetItem(executive_status_str);
        cell->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 3, cell);

        cell = new QTableWidgetItem(QString::number(e.position()));
        cell->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 4, cell);

        cell = new QTableWidgetItem(QString::number(e.coefficient()));
        cell->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 5, cell);

        cell = new QTableWidgetItem(e.startDate().toString("yyyy-MM-dd"));
        cell->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 6, cell);

        row++;
    }

    // 5. Options
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // rendre le tableau non editable
    tableWidget->setAlternatingRowColors(true);
    tableWidget->setShowGrid(false); // Rend le rendu encore plus moderne et épuré
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); // selection par ligne
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); // Ne permettre la sélection que d'une seule ligne à la fois
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Étirer automatiquement les colonnes sur toute la largeur disponible
    tableWidget->setSortingEnabled(true);  // active le trie sur les headers
    tableWidget->verticalHeader()->setVisible(false);     // Cacher les numéros de ligne

    // 6. Signal lors du double clic d'une ligne
    connect(tableWidget, &QTableWidget::cellDoubleClicked, this, &MainWindow::onTableDoubleClicked);

    return tableWidget;
}

// Création du second onglet (Formulaire)
QWidget* MainWindow::createPreventionTab() {
    auto *tab = new QWidget();
    auto *layout = new QVBoxLayout(tab);

    preventionTableWidget = fillPreventionTab(tab);

    auto *onLine = new QWidget(tab);
    auto *onLineLayout = new QHBoxLayout(onLine);
    auto *exportButton = new QPushButton("Exporter", tab);
    auto *cmptLabel = new QLabel("Nombre d'employés : ", tab);
    counterPrevention = new QLabel(tr("%1").arg(employees.size()), this);

    onLineLayout->addWidget(exportButton);
    onLineLayout->addStretch();
    onLineLayout->addWidget(cmptLabel);
    onLineLayout->addWidget(counterPrevention);

    layout->addWidget(onLine);
    layout->addWidget(preventionTableWidget);
    return tab;
}

// Remplissage du 2nd tableau
QTableWidget* MainWindow::fillPreventionTab(QWidget* tab) {
    // Instanciation du tableau (3 lignes, 3 colonnes)
    QTableWidget *tableWidget=nullptr;

    tableWidget = new QTableWidget(employees.size(), 6, tab);
    tableWidget->setHorizontalHeaderLabels({"Prénom", "Nom", "Poste", "Manager", "Plan de prévention", "Plan signé"});

    // 3. Préparation du QTableWidget
    tableWidget->clearContents();
    tableWidget->setRowCount(0); // Réinitialise les lignes

    int row = 0;

    // 4. Parcours de chaque collaborateur dans l'objet
    for (const Employee &e : employees) {

        // Récupération des données        
        int id = e.id();
        QString lastname{e.lastname()};
        QString firstname{e.firstname()};
        QString job{e.job()};
        QString signed_plan_str = e.signedPlan() ? "Oui" : "Non";
        QString plan_str = e.prevPlan();
        int manager_id = e.managerId();
        QString manager = get_manager_name(manager_id);

        // Insertion d'une nouvelle ligne dans le tableWidget
        tableWidget->insertRow(row);

        QTableWidgetItem *firstname_widget = new QTableWidgetItem(firstname);
        firstname_widget->setTextAlignment(Qt::AlignCenter);
        firstname_widget->setData(Qt::UserRole, id);

        // Remplissage des colonnes (Ajustez les indices 0,1,2... selon vos besoins)
        tableWidget->setItem(row, 0, firstname_widget);
        tableWidget->setItem(row, 1, new QTableWidgetItem(lastname));
        tableWidget->setItem(row, 2, new QTableWidgetItem(job));
        tableWidget->setItem(row, 3, new QTableWidgetItem(manager));
        tableWidget->setItem(row, 4, new QTableWidgetItem(plan_str));
        tableWidget->setItem(row, 5, new QTableWidgetItem(signed_plan_str));

        row++;
    }

    // 5. Options
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // rendre le tableau non editable
    tableWidget->setAlternatingRowColors(true);
    tableWidget->setShowGrid(false); // Rend le rendu encore plus moderne et épuré
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); // selection par ligne
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); // Ne permettre la sélection que d'une seule ligne à la fois
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Étirer automatiquement les colonnes sur toute la largeur disponible
    tableWidget->setSortingEnabled(true);  // active le trie sur les headers
    tableWidget->verticalHeader()->setVisible(false);     // Cacher les numéros de ligne

    // 6. Center text in cells
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        for (int col = 0; col < tableWidget->columnCount(); ++col)
            tableWidget->item(row, col)->setTextAlignment(Qt::AlignCenter);
    }

    return tableWidget;
}

// Création du Formulaire d'ajout
void MainWindow::addingEmployee() {
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
        apiClient.sendPostEmployeeRequest(json);
    }
    else
    {
        // L'utilisateur a cliqué sur "Annuler" ou fermé la fenêtre
        // Ajouter une fenêtre de confirmation
        // std::cout << "Saisie annulée" << std::endl;
    }
}

// Slot activé lors du clic sur la ligne du premier onglet pour modification via formulaire
void MainWindow::onTableDoubleClicked(int row, int column) {
    Q_UNUSED(column); // On ignore la colonne cliquée car on veut toute la ligne

    // 1. Récupérer l'item de la 1ère colonne de cette ligne
    QTableWidgetItem *firstItem = generalTableWidget->item(row, 0);
    if (!firstItem)
        return;

    // 2. Extraire l'ID qu'on avait caché dedans avec Qt::UserRole
    int id = firstItem->data(Qt::UserRole).toInt();
    // qDebug() << "Collaborateur ID:" << id;

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
            apiClient.sendPutEmployeeRequest(json, id, row, e);
        }
        else {
            // L'utilisateur a cliqué sur "Annuler" ou fermé la fenêtre
            // std::cout << "Saisie annulée" << std::endl;
        }
    }
}

// Actions after sending API requests
void MainWindow::onEmployeeModified(const int row, const Employee &e) {
    qDebug() << "Employé modifié dans BDD dont id =" << e.id();

    // update en local
    generalTableWidget->setSortingEnabled(false); // le sorting peut créer un décalage lors de l'ajout des nouvelles cellules
    preventionTableWidget->setSortingEnabled(false);

    updateRows(row, e);

    generalTableWidget->setSortingEnabled(true);
    preventionTableWidget->setSortingEnabled(true);
}
void MainWindow::onEmployeeAdded(int id) {

    qDebug() << "Employé ajouté dans BDD avec id =" << id;

    employees.back().setId(id);

    // update en local
    generalTableWidget->setSortingEnabled(false); // le sorting peut créer un décalage lors de l'ajout des nouvelles cellules
    preventionTableWidget->setSortingEnabled(false);

    generalTableWidget->insertRow(employees.size()-1);
    preventionTableWidget->insertRow(employees.size()-1);

    updateRows(employees.size()-1, employees.back());

    generalTableWidget->setSortingEnabled(true);
    preventionTableWidget->setSortingEnabled(true);
    updateCmpt();
}
void MainWindow::updateRows(const int &row, const Employee &e) {

    QString executive_status_str = e.isExecutive() ? "Executive status" : "No executive status";
    QString plan_signed_str = e.signedPlan() ? "Oui" : "Non";

    QTableWidgetItem *firstname_widget = new QTableWidgetItem(e.firstname());
    firstname_widget->setData(Qt::UserRole, e.id()); // ajout de l'ID caché

    ///////////////// update General Tab /////////////////
    generalTableWidget->setItem(row, 0, firstname_widget);
    generalTableWidget->setItem(row, 1, new QTableWidgetItem(e.birthdate().toString("yyyy-MM-dd")));
    generalTableWidget->setItem(row, 2, new QTableWidgetItem(e.job()));
    generalTableWidget->setItem(row, 3, new QTableWidgetItem(executive_status_str));
    generalTableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(e.position())));
    generalTableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(e.coefficient())));
    generalTableWidget->setItem(row, 6, new QTableWidgetItem(e.startDate().toString("yyyy-MM-dd")));

    for (int col = 0; col < generalTableWidget->columnCount(); ++col) {
        if (auto item = generalTableWidget->item(row, col))
            item->setTextAlignment(Qt::AlignCenter);
    }
    generalTableWidget->selectRow(row);


    ////////////// update Prevention Table /////////////
    // find row of the employee in Prevention tab
    int rowPrevention = preventionTableWidget->rowCount()-1; // initialise à la dernière ligne si ajout d'un collab
    QTableWidgetItem *firstCell;
    for(int iRow=0; iRow<preventionTableWidget->rowCount()-1;iRow++) {
        firstCell = preventionTableWidget->item(iRow, 0);
        if (!firstCell)
            return;

        // Extraire l'ID qu'on avait caché dedans avec Qt::UserRole
        int id = firstCell->data(Qt::UserRole).toInt();

        if(id==e.id()) {
            rowPrevention = iRow;
            break;
        }
    }
    preventionTableWidget->setItem(rowPrevention, 0, firstname_widget->clone());
    preventionTableWidget->setItem(rowPrevention, 1, new QTableWidgetItem(e.lastname()));
    preventionTableWidget->setItem(rowPrevention, 2, new QTableWidgetItem(e.job()));
    preventionTableWidget->setItem(rowPrevention, 3, new QTableWidgetItem(get_manager_name(e.managerId())));
    preventionTableWidget->setItem(rowPrevention, 4, new QTableWidgetItem(e.prevPlan()));
    preventionTableWidget->setItem(rowPrevention, 5, new QTableWidgetItem(plan_signed_str));
    preventionTableWidget->selectRow(rowPrevention);
    for (int col = 0; col < preventionTableWidget->columnCount(); ++col) {
        if (auto item = preventionTableWidget->item(rowPrevention, col))
            item->setTextAlignment(Qt::AlignCenter);
    }

}
void MainWindow::updateCmpt() {
    counterGeneral->setText(QString::fromStdString(to_string(employees.size())));
    counterPrevention->setText(QString::fromStdString(to_string(employees.size())));
}
void MainWindow::openConfigServerWindow() {
    ConfigServerWindow configserv;

    if (configserv.exec() == QDialog::Accepted) {
        // L'utilisateur a cliqué sur "Valider"
        QString json = configserv.toJson();
        qDebug() << "Saisie validée :" << json;
        apiClient.sendPutConfigRequest(json.toStdString());
    }
    else {
        // L'utilisateur a cliqué sur "Annuler" ou fermé la fenêtre
        qDebug() << "Saisie annulée";
    }
}
void MainWindow::openConfigAppWindow() {
    ConfigAppWindow configapp(this->apiClient.getPort(), apiClient.getHost());

    if (configapp.exec() == QDialog::Accepted) {
        // L'utilisateur a cliqué sur "Valider"
        qDebug() << "Config désirée sur http://" << configapp.getHost() << ":" << configapp.getPort();
        apiClient.setHost(configapp.getHost());
        apiClient.setPort(configapp.getPort());

        // Sauvegarde de la nouvelle config dans le fichier config.ini
        QMap<QString, QString> map;
        map[HOST_KEY] = configapp.getHost();
        map[PORT_KEY] = QString::fromStdString(to_string(configapp.getPort()));
        if(IniParser::saveConfig(CONFIG_FILE_NAME, map))
            qDebug() << "Configuration modifiée avec succès";
        else{
            qDebug() << "Erreur de sauvegarde de la nouvelle config";
            errorDisplay("Erreur de sauvegarde de la nouvelle config");
        }
        reloadData();
    }
    else {
        // L'utilisateur a cliqué sur "Annuler" ou fermé la fenêtre
        qDebug() << "Saisie annulée";
    }
}
void MainWindow::onConfigModified(const std::string json) {
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
            apiClient.setPort(obj.value(PORT_KEY).toInt());
            apiClient.setHost(obj.value(HOST_KEY).toString());
        }
    }
}

// Get manager name on employee ID
QString MainWindow::get_manager_name(const int &manager_id) {
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

// display error inside window
void MainWindow::errorDisplay(const QString &msg) {
    // 1. Création d'un QLabel pour l'erreur
    QLabel *errorLabel = new QLabel(this);

    // 2. Texte de l'erreur (supporte le HTML de base pour la mise en forme)
    errorLabel->setText(msg);

    // 3. Styliser avec du QSS pour capter l'attention (Rouge, marge...)
    errorLabel->setStyleSheet("color: #d32f2f; font-size: 22px; padding: 10px;");

    // 4. Centrer le texte si besoin
    errorLabel->setAlignment(Qt::AlignCenter);

    // 5. L'ajouter à votre layout principal
    setCentralWidget(errorLabel);
}

// load data by recreating tabs and getting all DB
void MainWindow::reloadData() {
    apiClient.sendGetEmployeeRequest(0);

    if(apiClient.getStatus()!=0){
        QString msg("");
        msg += "<b>Erreur de connexion :</b> ";
        msg += apiClient.getMsg();
        msg += "<br><br>Pensez à vérifier la configuration (host/port)...";
        errorDisplay(msg);
    }
    else {
        // Widget de gestion des onglets
        auto *tabWidget = new QTabWidget(this);

        // Save employees list
        parseMyJson();

        // Extract managers list
        extractManagers();

        // Ajout des deux onglets
        tabWidget->addTab(createGeneralTab(), "Général");
        tabWidget->addTab(createPreventionTab(), "Prévention");

        setCentralWidget(tabWidget);
    }
}

void MainWindow::openLogs() {

}
