#include "mainwindow.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    // Load data
    apiClient = new ApiClient(); // launch API that get all employees

    cmptEmployees = 0;

    setWindowTitle("ERP Scalian - RH management");
    resize(900, 800);

    QMenuBar *bar = menuBar();
    QMenu *fileMenu = bar->addMenu("&Paramètres");
    QAction *logsAction = fileMenu->addAction(tr("Open &Logs"));
    QAction *configAppAction = fileMenu->addAction(tr("Config &App"));
    QAction *configServAction = fileMenu->addAction(tr("Config &Server"));
    QAction *quitAction = fileMenu->addAction(tr("E&xit"));
    quitAction->setShortcuts(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);


    if(apiClient->getStatus()) {
        // QTimer::singleShot(0, this, &QMainWindow::close);
        // 1. Création d'un QLabel pour l'erreur
        QLabel *errorLabel = new QLabel(this);

        // 2. Texte de l'erreur (supporte le HTML de base pour la mise en forme)
        errorLabel->setText("<b>Erreur de connexion :</b> Impossible de joindre la base de données SQL.");

        // 3. Styliser avec du QSS pour capter l'attention (Rouge, marge...)
        errorLabel->setStyleSheet("color: #d32f2f; font-size: 14px; padding: 10px;");

        // 4. Centrer le texte si besoin
        errorLabel->setAlignment(Qt::AlignCenter);

        // 5. L'ajouter à votre layout principal
        setCentralWidget(errorLabel);
        return;
    }

    // Widget de gestion des onglets
    auto *tabWidget = new QTabWidget(this);

    // Get DB
    jsonDB = QString::fromUtf8(apiClient->getResponseData());
    parseMyJson();


    // Ajout des deux onglets
    tabWidget->addTab(createGeneralTab(), "Général");
    tabWidget->addTab(createPreventionTab(), "Prévention");

    setCentralWidget(tabWidget);
}


void MainWindow::parseMyJson() {

    // 2. Parser le QString
    QJsonParseError parseError;
    // QJsonDocument attend un QByteArray en UTF-8
    QJsonDocument doc = QJsonDocument::fromJson(apiClient->getResponseData(), &parseError);

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




MainWindow::~MainWindow() {}


ApiClient* MainWindow::getApi() {
    return this->apiClient;
}



// Remplissage du 1er tableau
QTableWidget* MainWindow::fillGeneralTab(QWidget* tab, const QString& jsonString)
{
    // Instanciation du tableau (3 lignes, 3 colonnes)
    QTableWidget *tableWidget=nullptr;

    // 1. Désérialisation du QString en document JSON
    QJsonParseError parseError;
    QJsonDocument doc;

    if(jsonString.isEmpty()) {
        qWarning() << "Erreur de parsing JSON : JSON vide";
        return tableWidget;
    }

    doc = QJsonDocument::fromJson(jsonString.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Erreur de parsing JSON :" << parseError.errorString();
        return tableWidget;
    }

    // Vérification que la racine est un tableau
    if (!doc.isArray()) {
        qWarning() << "Le JSON fourni n'est pas un tableau.";
        return tableWidget;
    }
    QJsonArray rootArray = doc.array();
    if (rootArray.isEmpty() || !rootArray.first().isObject()) {
        qWarning() << "Le tableau JSON est vide ou ne contient pas d'objet.";
        return tableWidget;
    }

    tableWidget = new QTableWidget(rootArray.size(), 7, tab);
    tableWidget->setHorizontalHeaderLabels({"Prénom", "Naissance", "Poste","Statut cadre", "Position (Syntec)","Coefficient (Syntec)", "Début"});

    // Étirer automatiquement les colonnes sur toute la largeur disponible
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Cacher les numéros de ligne
    tableWidget->verticalHeader()->setVisible(false);

    // On récupère le nombre de collaborateurs
    this->cmptEmployees = rootArray.size();

    // 3. Préparation du QTableWidget
    tableWidget->clearContents();
    tableWidget->setRowCount(0); // Réinitialise les lignes

    int row = 0;

    // 4. Parcours de chaque collaborateur dans l'objet
    for (const QJsonValue& val : rootArray) {
        if (!val.isObject()) continue;
        QJsonObject colab = val.toObject();

        // Récupération des données depuis le JSON
        QString lastname = colab["lastname"].toString();
        QString firstname = colab["firstname"].toString(); // Si vous voulez combiner Nom + Prénom
        QString job = colab["job"].toString();

        // Formatage / Conversion Cadre (1 -> "Cadre", 0 -> "Non Cadre")
        bool executive_status = colab["executive_status"].toBool();
        QString executive_status_str = executive_status ? "Executive status" : "No executive status";

        QString position = QString::number(colab["position"].toDouble());
        QString coef = QString::number(colab["coefficient"].toInt());

        // Conversions des dates au format "dd/MM/yyyy"
        QString start_date_raw = colab["start_date"].toString();
        QDate start_date = QDate::fromString(start_date_raw, "yyyy-MM-dd");
        QString start_date_str = start_date.isValid() ? start_date.toString("dd/MM/yyyy") : start_date_raw;

        QString birthdate_raw = colab["birthdate"].toString();
        QDate birthdate = QDate::fromString(birthdate_raw, "yyyy-MM-dd");
        QString birthdate_str = birthdate.isValid() ? birthdate.toString("dd/MM/yyyy") : birthdate_raw;


        // Récupération des données depuis le JSON
        int id = colab["id"].toInt();

        // Insertion d'une nouvelle ligne dans le tableWidget
        tableWidget->insertRow(row);

        QTableWidgetItem *firstname_widget = new QTableWidgetItem(firstname);
        firstname_widget->setTextAlignment(Qt::AlignCenter);

        // --- MAGIE ICI ---
        // On stocke l'ID unique (ou l'index dans la QList) dans les données cachées du widget
        firstname_widget->setData(Qt::UserRole, id);

        // Insertion d'une nouvelle ligne dans le tableWidget
        tableWidget->insertRow(row);

        // Remplissage des colonnes (Ajustez les indices 0,1,2... selon vos besoins)
        tableWidget->setItem(row, 0, firstname_widget);

        // Remplissage des colonnes (Ajustez les indices 0,1,2... selon vos besoins)
        // tableWidget->setItem(row, 0, new QTableWidgetItem(firstname));

        QTableWidgetItem *cell = new QTableWidgetItem(birthdate_str);
        cell->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 1, cell);

        cell = new QTableWidgetItem(job);
        cell->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 2, cell);


        cell = new QTableWidgetItem(executive_status_str);
        cell->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 3, cell);

        cell = new QTableWidgetItem(position);
        cell->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 4, cell);

        cell = new QTableWidgetItem(coef);
        cell->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 5, cell);

        cell = new QTableWidgetItem(start_date_str);
        cell->setTextAlignment(Qt::AlignCenter);
        tableWidget->setItem(row, 6, cell);


        row++;
    }

    // connect(tableWidget, &QTableWidget::doubleClicked, this, &MainWindow::createFormWindow);

    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // rendre le tableau non editable
    tableWidget->setAlternatingRowColors(true);
    tableWidget->setShowGrid(false); // Rend le rendu encore plus moderne et épuré

    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); // selection par ligne
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); // Ne permettre la sélection que d'une seule ligne à la fois
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setSortingEnabled(true);

    connect(tableWidget, &QTableWidget::cellDoubleClicked, this, &MainWindow::onTableDoubleClicked);

    return tableWidget;
}

QString get_manager_name(const QJsonArray rootArray, const int &manager_id) {
    for (const QJsonValue& val : rootArray) {
        if (!val.isObject()) continue;
        QJsonObject colab = val.toObject();

        int id = colab["id"].toInt();
        if(id==manager_id)
        {
            QString lastname = colab["lastname"].toString();
            QString firstname = colab["firstname"].toString();
            return firstname + " " + lastname;
        }
    }
    return "Aucun renseigné";
}

// Remplissage du 2nd tableau
QTableWidget* MainWindow::fillPreventionTab(QWidget* tab, const QString& jsonString)
{
    // Instanciation du tableau (3 lignes, 3 colonnes)
    QTableWidget *tableWidget=nullptr;

    // 1. Désérialisation du QString en document JSON
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Erreur de parsing JSON :" << parseError.errorString();
        return tableWidget;
    }

    // 2. Vérification que la racine est un tableau
    if (!doc.isArray()) {
        qWarning() << "Le JSON fourni n'est pas un tableau.";
        return tableWidget;
    }

    QJsonArray rootArray = doc.array();
    if (rootArray.isEmpty() || !rootArray.first().isObject()) {
        qWarning() << "Le tableau JSON est vide ou ne contient pas d'objet.";
        return tableWidget;
    }

    tableWidget = new QTableWidget(rootArray.size(), 6, tab);
    tableWidget->setHorizontalHeaderLabels({"Prénom", "Nom", "Poste", "Manager", "Plan de prévention", "Plan signé"});


    // Étirer automatiquement les colonnes sur toute la largeur disponible
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Cacher les numéros de ligne
    tableWidget->verticalHeader()->setVisible(false);

    // On récupère le nombre de collaborateurs
    this->cmptEmployees = rootArray.size();

    // 3. Préparation du QTableWidget
    tableWidget->clearContents();
    tableWidget->setRowCount(0); // Réinitialise les lignes

    int row = 0;

    // 4. Parcours de chaque collaborateur dans l'objet
    for (const QJsonValue& val : rootArray) {
        if (!val.isObject()) continue;
        QJsonObject colab = val.toObject();

        // Récupération des données depuis le JSON
        QString lastname = colab["lastname"].toString();
        QString firstname = colab["firstname"].toString();
        QString job = colab["job"].toString();
        bool signed_plan = (colab["signed_plan"].toBool());
        QString signed_plan_str = signed_plan ? "Oui" : "Non";
        QString plan_str = colab["prev_plan"].toString();
        int manager_id = colab["manager_id"].toInt();
        QString manager = get_manager_name(rootArray, manager_id);    // A MODIFIER POUR METTRE LE NOM -> parsing du json puis selection dans tableau l'ID correspondant

        // Insertion d'une nouvelle ligne dans le tableWidget
        tableWidget->insertRow(row);

        // Remplissage des colonnes (Ajustez les indices 0,1,2... selon vos besoins)
        tableWidget->setItem(row, 0, new QTableWidgetItem(firstname));
        tableWidget->setItem(row, 1, new QTableWidgetItem(lastname));
        tableWidget->setItem(row, 2, new QTableWidgetItem(job));
        tableWidget->setItem(row, 3, new QTableWidgetItem(manager));
        tableWidget->setItem(row, 4, new QTableWidgetItem(plan_str));
        tableWidget->setItem(row, 5, new QTableWidgetItem(signed_plan_str));

        row++;
    }

    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // rendre le tableau non editable
    tableWidget->setAlternatingRowColors(true);
    tableWidget->setShowGrid(false); // Rend le rendu encore plus moderne et épuré
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); // selection par ligne
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); // Ne permettre la sélection que d'une seule ligne à la fois
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    return tableWidget;
}

// Création du premier onglet (Tableau)
QWidget* MainWindow::createGeneralTab() {
    auto *tab = new QWidget();
    auto *layout = new QVBoxLayout(tab);

    generalTableWidget = fillGeneralTab(tab, jsonDB);

    auto *onLine = new QWidget();
    auto *onLineLayout = new QHBoxLayout(onLine);
    auto *addButton = new QPushButton("Ajouter");
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addingEmployee);
    auto *exportButton = new QPushButton("Exporter");
    auto *cmptLabel = new QLabel("Nombre collaborateurs : ");
    auto *cmptVal = new QLabel(tr("%1").arg(this->cmptEmployees));

    // // Couleur bouton "Ajouter"
    // QPalette palette = addButton->palette();
    // palette.setColor(QPalette::ButtonText, Qt::white); // Couleur du texte
    // palette.setColor(QPalette::Button, QColor(0, 122, 204)); // Couleur du fond
    // addButton->setPalette(palette);

    // // Couleur bouton "Exporter"
    // palette = exportButton->palette();
    // palette.setColor(QPalette::ButtonText, Qt::black); // Couleur du texte
    // palette.setColor(QPalette::Button, QColor(0, 204, 100)); // Couleur du fond
    // exportButton->setPalette(palette);

    // RECOMMANDÉ : Forcer le bouton à dessiner son propre fond
    this->setAutoFillBackground(true);

    onLineLayout->addWidget(addButton);
    onLineLayout->addWidget(exportButton);
    onLineLayout->addStretch();
    onLineLayout->addWidget(cmptLabel);
    onLineLayout->addWidget(cmptVal);

    layout->addWidget(onLine);
    layout->addWidget(generalTableWidget);

    return tab;
}

// Création du second onglet (Formulaire)
QWidget* MainWindow::createPreventionTab() {
    auto *tab = new QWidget();
    auto *layout = new QVBoxLayout(tab);

    auto *tableWidget = fillPreventionTab(tab, jsonDB);

    auto *onLine = new QWidget();
    auto *onLineLayout = new QHBoxLayout(onLine);
    auto *exportButton = new QPushButton("Exporter");
    auto *cmptLabel = new QLabel("Nombre collaborateurs : ");
    auto *cmptVal = new QLabel(tr("%1").arg(this->cmptEmployees));

    // Couleur bouton "Exporter"
    QPalette palette = exportButton->palette();
    palette.setColor(QPalette::ButtonText, Qt::black); // Couleur du texte
    palette.setColor(QPalette::Button, QColor(0, 204, 100)); // Couleur du fond
    exportButton->setPalette(palette);

    // RECOMMANDÉ : Forcer le bouton à dessiner son propre fond
    this->setAutoFillBackground(true);

    onLineLayout->addWidget(exportButton);
    onLineLayout->addStretch();
    onLineLayout->addWidget(cmptLabel);
    onLineLayout->addWidget(cmptVal);

    layout->addWidget(onLine);

    // Étirer automatiquement les colonnes sur toute la largeur disponible
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Cacher les numéros de ligne
    tableWidget->verticalHeader()->setVisible(false);

    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // rendre le tableau non editable
    layout->addWidget(tableWidget);
    return tab;
}

// Création du Formulaire
void MainWindow::addingEmployee() {
    // Instanciation du dialogue avec 'this' en parent
    FormWindow dialog;

    // .exec() rend la fenêtre MODALE et bloque le flux jusqu'à la fermerture
    if (dialog.exec() == QDialog::Accepted)
    {
        // L'utilisateur a cliqué sur "Valider"
        QString nom = dialog.getNom();
        QString prenom = dialog.getPrenom();

        // std::cout << "Saisie validée :" << nom.toStdString() << " " << prenom.toStdString() << std::endl;

        // Ici : Appel de votre service HTTP pour envoyer le JSON au serveur
        // m_apiService->createCollaborateur(nom, prenom);

    }
    else
    {
        // L'utilisateur a cliqué sur "Annuler" ou fermé la fenêtre
        // std::cout << "Saisie annulée" << std::endl;
    }
}

// Slot activé lors du clic sur la ligne du premier onglet
void MainWindow::onTableDoubleClicked(int row, int column)
{
    Q_UNUSED(column); // On ignore la colonne cliquée car on veut toute la ligne

    // 1. Récupérer l'item de la 1ère colonne de cette ligne
    QTableWidgetItem *firstItem = generalTableWidget->item(row, 0);
    if (!firstItem) return;

    // 2. Extraire l'ID qu'on avait caché dedans avec Qt::UserRole
    int id = firstItem->data(Qt::UserRole).toInt();
    // qDebug() << "Collaborateur ID:" << id;

    // 3. Chercher le collaborateur correspondant dans votre QList
    auto it = std::find_if(employees.begin(), employees.end(),
                           [id](const Employee &c) {
                               return c.id() == id;
                           });

    // apiClient->sendGetEmployeeRequest(id);
    // Employee e(QString::fromUtf8(apiClient->getResponseData()).toStdString());
    // int indice = 0;
    // for (int var = 0; var < employees.size(); ++var) {
    //     if(employees[var].id()==id)
    //         indice = var;
    // }
    // Employee e = employees[indice];


    if (it != employees.end())
    {

    // 4. Ouvrir le formualire remplit avec les données de l'employé
        Employee e = *it; // Copie de l'objet à modifier
        FormWindow dialog(e); // ouvre formulaire

        // .exec() rend la fenêtre MODALE et bloque le flux jusqu'à la fermerture
        if (dialog.exec() == QDialog::Accepted)
        {
            // L'utilisateur a cliqué sur "Valider"
            QString nom = dialog.getNom();
            QString prenom = dialog.getPrenom();

            qDebug() << "Saisie validée :" << nom.toStdString() << " " << prenom.toStdString();

            // Ici : Appel du service HTTP pour envoyer le JSON au serveur
            // m_apiService->createCollaborateur(nom, prenom);

        }
        else
        {
            // L'utilisateur a cliqué sur "Annuler" ou fermé la fenêtre
            // std::cout << "Saisie annulée" << std::endl;
        }

    }

}
