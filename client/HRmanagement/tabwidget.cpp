#include "tabwidget.hpp"

#include "tablewidget.hpp"
#include "datetablewidgetitem.hpp"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>


TabWidget::TabWidget(QWidget *parent)
    : QWidget{parent}
{
    auto *mainLayout = new QVBoxLayout(this);

    m_tab = new QTabWidget(this);

    // Ajout des deux onglets
    m_tab->addTab(createGeneralTab(), "Général");
    m_tab->addTab(createPreventionTab(), "Prévention");

    mainLayout->addWidget(m_tab);

    // Bouton de retour
    auto *btnBack = new QPushButton("Retour", this);
    btnBack->setMaximumWidth(200);
    mainLayout->addWidget(btnBack, 0, Qt::AlignLeft);

    // Câblage du bouton retour vers l'émission de la requête de navigation
    connect(btnBack, &QPushButton::clicked, this, [this]() {
        emit requestNavigation(ScreenId::Home);
    });
}

// Création du premier onglet (Tableau)
QWidget* TabWidget::createGeneralTab() {
    auto *tab = new QWidget(this);
    auto *layout = new QVBoxLayout(tab);

    m_generalTable = new TableWidget({"Prénom", "Naissance", "Poste","Statut cadre", "Position (Syntec)","Coefficient (Syntec)", "Début"}, this);

    // Signal lors du double clic d'une ligne
    connect(m_generalTable->m_table, &QTableWidget::cellDoubleClicked, this, &TabWidget::onTableDoubleClicked);


    auto *onLine = new QWidget(this);
    auto *onLineLayout = new QHBoxLayout(onLine);
    auto *addButton = new QPushButton("Ajouter", this);
    connect(addButton, &QPushButton::clicked, this, &TabWidget::addingEmployee);
    auto *exportButton = new QPushButton("Exporter", this);
    auto *cmptLabel = new QLabel("Nombre d'employés : ", this);
    counterGeneral = new QLabel("0", this);

    onLineLayout->addWidget(addButton);
    onLineLayout->addWidget(exportButton);
    onLineLayout->addStretch();
    onLineLayout->addWidget(cmptLabel);
    onLineLayout->addWidget(counterGeneral);

    layout->addWidget(onLine);
    layout->addWidget(m_generalTable);

    return tab;
}


// Création du second onglet (Formulaire)
QWidget* TabWidget::createPreventionTab() {
    auto *tab = new QWidget();
    auto *layout = new QVBoxLayout(tab);

    m_preventionTable = new TableWidget({"Prénom", "Nom", "Poste", "Manager", "Plan de prévention", "Plan signé"}, this);


    auto *onLine = new QWidget(tab);
    auto *onLineLayout = new QHBoxLayout(onLine);
    auto *exportButton = new QPushButton("Exporter", tab);
    auto *cmptLabel = new QLabel("Nombre d'employés : ", tab);
    counterPrevention = new QLabel("0", this);

    onLineLayout->addWidget(exportButton);
    onLineLayout->addStretch();
    onLineLayout->addWidget(cmptLabel);
    onLineLayout->addWidget(counterPrevention);

    layout->addWidget(onLine);
    layout->addWidget(m_preventionTable);
    return tab;
}


// Mise à jour du tableau des employés
void TabWidget::employeesUpdate(const QList<Employee> &employees) {
    int row = 0;
    counterGeneral->setText(tr("%1").arg(employees.size()));
    m_generalTable->m_table->setRowCount(0);
    m_preventionTable->m_table->setRowCount(0);
    counterPrevention->setText(tr("%1").arg(employees.size()));

    // update en local
    m_generalTable->m_table->setSortingEnabled(false); // le sorting peut créer un décalage lors de l'ajout des nouvelles cellules
    m_preventionTable->m_table->setSortingEnabled(false);

    // Parcours de chaque employe et remplir table
    for (const Employee &e : employees) {

        // Insertion d'une nouvelle ligne dans le tableWidget
        m_generalTable->m_table->insertRow(row);

        QTableWidgetItem *firstname_widget = new QTableWidgetItem(e.firstname());
        firstname_widget->setTextAlignment(Qt::AlignCenter);

        // On stocke l'ID unique dans les données cachées du widget
        firstname_widget->setData(Qt::UserRole, e.id());

        // Ajout du widget
        m_generalTable->m_table->setItem(row, 0, firstname_widget);

        QTableWidgetItem *cell = new DateTableWidgetItem(e.birthdate());
        cell->setTextAlignment(Qt::AlignCenter);
        m_generalTable->m_table->setItem(row, 1, cell);

        cell = new QTableWidgetItem(e.job());
        cell->setTextAlignment(Qt::AlignCenter);
        m_generalTable->m_table->setItem(row, 2, cell);


        QString executive_status_str = e.isExecutive() ? "Executive status" : "No executive status";
        cell = new QTableWidgetItem(executive_status_str);
        cell->setTextAlignment(Qt::AlignCenter);
        m_generalTable->m_table->setItem(row, 3, cell);

        cell = new QTableWidgetItem(QString::number(e.position()));
        cell->setTextAlignment(Qt::AlignCenter);
        m_generalTable->m_table->setItem(row, 4, cell);

        cell = new QTableWidgetItem(QString::number(e.coefficient()));
        cell->setTextAlignment(Qt::AlignCenter);
        m_generalTable->m_table->setItem(row, 5, cell);

        cell = new DateTableWidgetItem(e.startDate());
        cell->setTextAlignment(Qt::AlignCenter);
        m_generalTable->m_table->setItem(row, 6, cell);

        ///////////////////////////////////////////////////////////////////


        // Récupération des données
        firstname_widget = new QTableWidgetItem(e.firstname());
        firstname_widget->setTextAlignment(Qt::AlignCenter);

        // On stocke l'ID unique dans les données cachées du widget
        firstname_widget->setData(Qt::UserRole, e.id());

        QString signed_plan_str = e.signedPlan() ? "Oui" : "Non";
        int manager_id = e.managerId();
        QString manager = "";
        // trouver le manager
        for (const Employee &emp : employees) {
            if(emp.id()==manager_id) {
                QString lastname = emp.lastname();
                QString firstname = emp.firstname();
                manager = firstname + " " + lastname;
            }
        }
        if(manager.isEmpty())
            manager = "Aucun renseigné";

        // Insertion d'une nouvelle ligne dans le tableWidget
        m_preventionTable->m_table->insertRow(row);

        // Remplissage des colonnes (Ajustez les indices 0,1,2... selon vos besoins)
        m_preventionTable->m_table->setItem(row, 0, firstname_widget);
        m_preventionTable->m_table->setItem(row, 1, new QTableWidgetItem(e.lastname()));
        m_preventionTable->m_table->setItem(row, 2, new QTableWidgetItem(e.job()));
        m_preventionTable->m_table->setItem(row, 3, new QTableWidgetItem(manager));
        m_preventionTable->m_table->setItem(row, 4, new QTableWidgetItem(e.prevPlan()));
        m_preventionTable->m_table->setItem(row, 5, new QTableWidgetItem(signed_plan_str));


        // Center text in cells
        for (int row = 0; row < m_preventionTable->m_table->rowCount(); ++row) {
            for (int col = 0; col < m_preventionTable->m_table->columnCount(); ++col)
                m_preventionTable->m_table->item(row, col)->setTextAlignment(Qt::AlignCenter);
        }




        row++;
    }

    m_generalTable->m_table->setSortingEnabled(true);
    m_preventionTable->m_table->setSortingEnabled(true);

    emit requestNavigation(ScreenId::TableViewer);
}


// Slot activé lors du clic sur la ligne du premier onglet pour modification via formulaire
void TabWidget::onTableDoubleClicked(int row, int column) {
    Q_UNUSED(column); // On ignore la colonne cliquée car on veut toute la ligne

    // 1. Récupérer l'item de la 1ère colonne de cette ligne
    QTableWidgetItem *firstItem = m_generalTable->m_table->item(row, 0);
    if (!firstItem)
        return;

    // 2. Extraire l'ID qu'on avait caché dedans avec Qt::UserRole
    int id = firstItem->data(Qt::UserRole).toInt();
    // qDebug() << "Collaborateur ID:" << id;

    emit openEditEmployee(id, row);
}



// Actions after sending API requests
void TabWidget::onEmployeeModified(const int row, const Employee &e) {
    // qDebug() << "Employé modifié dans BDD dont id =" << e.id();

    // update en local
    m_generalTable->m_table->setSortingEnabled(false); // le sorting peut créer un décalage lors de l'ajout des nouvelles cellules
    m_preventionTable->m_table->setSortingEnabled(false);

    updateRows(row, e);

    m_generalTable->m_table->setSortingEnabled(true);
    m_preventionTable->m_table->setSortingEnabled(true);
}
void TabWidget::onEmployeeAdded(const int &id, const QList<Employee> &employees) {

    // update en local
    m_generalTable->m_table->setSortingEnabled(false); // le sorting peut créer un décalage lors de l'ajout des nouvelles cellules
    m_preventionTable->m_table->setSortingEnabled(false);

    m_generalTable->m_table->insertRow(employees.size()-1);
    m_preventionTable->m_table->insertRow(employees.size()-1);

    updateRows(employees.size()-1, employees.back());

    m_generalTable->m_table->setSortingEnabled(true);
    m_preventionTable->m_table->setSortingEnabled(true);
    updateCmpt(employees);
}
void TabWidget::updateRows(const int &row, const Employee &e) {

    QString executive_status_str = e.isExecutive() ? "Executive status" : "No executive status";
    QString plan_signed_str = e.signedPlan() ? "Oui" : "Non";

    QTableWidgetItem *firstname_widget = new QTableWidgetItem(e.firstname());
    firstname_widget->setData(Qt::UserRole, e.id()); // ajout de l'ID caché

    ///////////////// update General Tab /////////////////
    m_generalTable->m_table->setItem(row, 0, firstname_widget);
    m_generalTable->m_table->setItem(row, 1, new DateTableWidgetItem(e.birthdate()));
    m_generalTable->m_table->setItem(row, 2, new QTableWidgetItem(e.job()));
    m_generalTable->m_table->setItem(row, 3, new QTableWidgetItem(executive_status_str));
    m_generalTable->m_table->setItem(row, 4, new QTableWidgetItem(QString::number(e.position())));
    m_generalTable->m_table->setItem(row, 5, new QTableWidgetItem(QString::number(e.coefficient())));
    m_generalTable->m_table->setItem(row, 6, new DateTableWidgetItem(e.startDate()));

    for (int col = 0; col < m_generalTable->m_table->columnCount(); ++col) {
        if (auto item = m_generalTable->m_table->item(row, col))
            item->setTextAlignment(Qt::AlignCenter);
    }
    m_generalTable->m_table->selectRow(row);


    ////////////// update Prevention Table /////////////
    // find row of the employee in Prevention tab
    int rowPrevention = m_preventionTable->m_table->rowCount()-1; // initialise à la dernière ligne si ajout d'un collab
    QTableWidgetItem *firstCell;
    for(int iRow=0; iRow<m_preventionTable->m_table->rowCount()-1;iRow++) {
        firstCell = m_preventionTable->m_table->item(iRow, 0);
        if (!firstCell)
            return;

        // Extraire l'ID qu'on avait caché dedans avec Qt::UserRole
        int id = firstCell->data(Qt::UserRole).toInt();

        if(id==e.id()) {
            rowPrevention = iRow;
            break;
        }
    }
    m_preventionTable->m_table->setItem(rowPrevention, 0, firstname_widget->clone());
    m_preventionTable->m_table->setItem(rowPrevention, 1, new QTableWidgetItem(e.lastname()));
    m_preventionTable->m_table->setItem(rowPrevention, 2, new QTableWidgetItem(e.job()));
    m_preventionTable->m_table->setItem(rowPrevention, 3, new QTableWidgetItem("feafea")); //   get_manager_name(e.managerId())
    m_preventionTable->m_table->setItem(rowPrevention, 4, new QTableWidgetItem(e.prevPlan()));
    m_preventionTable->m_table->setItem(rowPrevention, 5, new QTableWidgetItem(plan_signed_str));
    m_preventionTable->m_table->selectRow(rowPrevention);
    for (int col = 0; col < m_preventionTable->m_table->columnCount(); ++col) {
        if (auto item = m_preventionTable->m_table->item(rowPrevention, col))
            item->setTextAlignment(Qt::AlignCenter);
    }
}
void TabWidget::updateCmpt(const QList<Employee> &employees) {
    counterGeneral->setText(QString::fromStdString(to_string(employees.size())));
    counterPrevention->setText(QString::fromStdString(to_string(employees.size())));
}

