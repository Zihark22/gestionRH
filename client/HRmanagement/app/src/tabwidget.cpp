#include "include/tabwidget.hpp"

#include "include/tablewidget.hpp"
#include "include/datetablewidgetitem.hpp"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>


TabWidget::TabWidget(QWidget *parent)
    : QWidget{parent}
{
    auto *mainLayout = new QVBoxLayout(this);

    mTab = new QTabWidget(this);

    // Ajout des deux onglets
    mTab->addTab(createGeneralTab(), "Général");
    mTab->addTab(createPreventionTab(), "Prévention");

    mainLayout->addWidget(mTab);

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

    mGeneralTable = new TableWidget({"Prénom", "Naissance", "Poste","Statut cadre", "Position (Syntec)","Coefficient (Syntec)", "Début"}, this);

    // Signal lors du double clic d'une ligne
    connect(mGeneralTable->getTable(), &QTableWidget::cellDoubleClicked, this, &TabWidget::onTableDoubleClicked);


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
    layout->addWidget(mGeneralTable);

    return tab;
}


// Création du second onglet (Formulaire)
QWidget* TabWidget::createPreventionTab() {
    auto *tab = new QWidget();
    auto *layout = new QVBoxLayout(tab);

    mPreventionTable = new TableWidget({"Prénom", "Nom", "Poste", "Manager", "Plan de prévention", "Plan signé"}, this);


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
    layout->addWidget(mPreventionTable);
    return tab;
}


// Mise à jour du tableau des employés
void TabWidget::employeesUpdate(const QList<Employee> &employees) {
    int row = 0;
    counterGeneral->setText(tr("%1").arg(employees.size()));
    mGeneralTable->getTable()->setRowCount(0);
    mPreventionTable->getTable()->setRowCount(0);
    counterPrevention->setText(tr("%1").arg(employees.size()));

    // update en local
    mGeneralTable->getTable()->setSortingEnabled(false); // le sorting peut créer un décalage lors de l'ajout des nouvelles cellules
    mPreventionTable->getTable()->setSortingEnabled(false);

    // Parcours de chaque employe et remplir table
    for (const Employee &e : employees) {

        // Insertion d'une nouvelle ligne dans le tableWidget
        mGeneralTable->getTable()->insertRow(row);

        QTableWidgetItem *firstname_widget = new QTableWidgetItem(e.firstname());
        firstname_widget->setTextAlignment(Qt::AlignCenter);

        // On stocke l'ID unique dans les données cachées du widget
        firstname_widget->setData(Qt::UserRole, e.id());

        // Ajout du widget
        mGeneralTable->getTable()->setItem(row, 0, firstname_widget);

        QTableWidgetItem *cell = new DateTableWidgetItem(e.birthdate());
        cell->setTextAlignment(Qt::AlignCenter);
        mGeneralTable->getTable()->setItem(row, 1, cell);

        cell = new QTableWidgetItem(e.job());
        cell->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        mGeneralTable->getTable()->setItem(row, 2, cell);


        QString executive_status_str = e.isExecutive() ? "✔" : "X";
        cell = new QTableWidgetItem(executive_status_str);
        cell->setTextAlignment(Qt::AlignCenter);
        mGeneralTable->getTable()->setItem(row, 3, cell);

        cell = new QTableWidgetItem(QString::number(e.position()));
        cell->setTextAlignment(Qt::AlignCenter);
        mGeneralTable->getTable()->setItem(row, 4, cell);

        cell = new QTableWidgetItem(QString::number(e.coefficient()));
        cell->setTextAlignment(Qt::AlignCenter);
        mGeneralTable->getTable()->setItem(row, 5, cell);

        cell = new DateTableWidgetItem(e.startDate());
        cell->setTextAlignment(Qt::AlignCenter);
        mGeneralTable->getTable()->setItem(row, 6, cell);

        ///////////////////////////////////////////////////////////////////


        // Récupération des données
        firstname_widget = new QTableWidgetItem(e.firstname());
        firstname_widget->setTextAlignment(Qt::AlignCenter);

        // On stocke l'ID unique dans les données cachées du widget
        firstname_widget->setData(Qt::UserRole, e.id());

        QString signed_plan_str = e.signedPlan() ? "✔" : "X";
        uint manager_id = e.managerId();
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
            manager = "Aucun";

        // Insertion d'une nouvelle ligne dans le tableWidget
        mPreventionTable->getTable()->insertRow(row);

        // Remplissage des colonnes (Ajustez les indices 0,1,2... selon vos besoins)
        mPreventionTable->getTable()->setItem(row, 0, firstname_widget);
        mPreventionTable->getTable()->setItem(row, 1, new QTableWidgetItem(e.lastname()));
        mPreventionTable->getTable()->setItem(row, 2, new QTableWidgetItem(e.job()));
        mPreventionTable->getTable()->setItem(row, 3, new QTableWidgetItem(manager));
        mPreventionTable->getTable()->setItem(row, 4, new QTableWidgetItem(e.prevPlan()));
        mPreventionTable->getTable()->setItem(row, 5, new QTableWidgetItem(signed_plan_str));
        if(!e.signedPlan()) {
            QColor lightRed(255, 220, 220);
            for (int col = 0; col < mPreventionTable->getTable()->columnCount(); ++col) {
                mPreventionTable->getTable()->item(row, col)->setBackground(lightRed);
            }
        }

        // Center text in cells
        for (int row = 0; row < mPreventionTable->getTable()->rowCount(); ++row) {
            for (int col = 0; col < mPreventionTable->getTable()->columnCount(); ++col)
                mPreventionTable->getTable()->item(row, col)->setTextAlignment(Qt::AlignCenter);
            mPreventionTable->getTable()->item(row, 2)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter); // align job left
        }

        row++;
    }

    mGeneralTable->getTable()->setSortingEnabled(true);
    mPreventionTable->getTable()->setSortingEnabled(true);

    emit requestNavigation(ScreenId::TableViewer);
}


void TabWidget::onTableDoubleClicked(int row, int column) {
    Q_UNUSED(column); // On ignore la colonne cliquée car on veut toute la ligne

    // 1. Récupérer l'item de la 1ère colonne de cette ligne
    QTableWidgetItem *firstItem = mGeneralTable->getTable()->item(row, 0);
    if (!firstItem)
        return;

    // 2. Extraire l'ID qu'on avait caché dedans avec Qt::UserRole
    uint id = firstItem->data(Qt::UserRole).toUInt();
    // qDebug() << "Collaborateur ID:" << id;

    emit openEditEmployee(id, row);
}

void TabWidget::onEmployeeModified(const int &row, const Employee &e, const QString &manager) {
    // qDebug() << "Employé modifié dans BDD dont id =" << e.id();

    // update en local
    mGeneralTable->getTable()->setSortingEnabled(false); // le sorting peut créer un décalage lors de l'ajout des nouvelles cellules
    mPreventionTable->getTable()->setSortingEnabled(false);

    updateRows(row, e, manager);

    mGeneralTable->getTable()->setSortingEnabled(true);
    mPreventionTable->getTable()->setSortingEnabled(true);
}
void TabWidget::onEmployeeAdded(const QList<Employee> &employees, const QString &manager) {

    // update en local
    mGeneralTable->getTable()->setSortingEnabled(false); // le sorting peut créer un décalage lors de l'ajout des nouvelles cellules
    mPreventionTable->getTable()->setSortingEnabled(false);

    mGeneralTable->getTable()->insertRow(employees.size()-1);
    mPreventionTable->getTable()->insertRow(employees.size()-1);

    updateRows(employees.size()-1, employees.back(), manager);

    mGeneralTable->getTable()->setSortingEnabled(true);
    mPreventionTable->getTable()->setSortingEnabled(true);
    updateCmpt(employees);
}
void TabWidget::updateRows(const int &row, const Employee &e, const QString &manager) {

    QString executive_status_str = e.isExecutive() ? "✔" : "X";
    QString plan_signed_str = e.signedPlan() ? "✔" : "X";

    QTableWidgetItem *firstname_widget = new QTableWidgetItem(e.firstname());
    firstname_widget->setData(Qt::UserRole, e.id()); // ajout de l'ID caché

    ///////////////// update General Tab /////////////////
    mGeneralTable->getTable()->setItem(row, 0, firstname_widget);
    mGeneralTable->getTable()->setItem(row, 1, new DateTableWidgetItem(e.birthdate()));
    mGeneralTable->getTable()->setItem(row, 2, new QTableWidgetItem(e.job()));
    mGeneralTable->getTable()->setItem(row, 3, new QTableWidgetItem(executive_status_str));
    mGeneralTable->getTable()->setItem(row, 4, new QTableWidgetItem(QString::number(e.position())));
    mGeneralTable->getTable()->setItem(row, 5, new QTableWidgetItem(QString::number(e.coefficient())));
    mGeneralTable->getTable()->setItem(row, 6, new DateTableWidgetItem(e.startDate()));

    for (int col = 0; col < mGeneralTable->getTable()->columnCount(); ++col) {
        if (auto item = mGeneralTable->getTable()->item(row, col))
            item->setTextAlignment(Qt::AlignCenter);
    }
    mGeneralTable->getTable()->item(row, 2)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter); // align job left
    mGeneralTable->getTable()->selectRow(row);


    ////////////// update Prevention Table /////////////
    // find row of the employee in Prevention tab
    int rowPrevention = mPreventionTable->getTable()->rowCount()-1; // initialise à la dernière ligne si ajout d'un collab
    QTableWidgetItem *firstCell;
    for(int iRow=0; iRow<mPreventionTable->getTable()->rowCount()-1;iRow++) {
        firstCell = mPreventionTable->getTable()->item(iRow, 0);
        if (!firstCell)
            return;

        // Extraire l'ID qu'on avait caché dedans avec Qt::UserRole
        uint id = firstCell->data(Qt::UserRole).toUInt();

        if(id==e.id()) {
            rowPrevention = iRow;
            break;
        }
    }
    mPreventionTable->getTable()->setItem(rowPrevention, 0, firstname_widget->clone());
    mPreventionTable->getTable()->setItem(rowPrevention, 1, new QTableWidgetItem(e.lastname()));
    mPreventionTable->getTable()->setItem(rowPrevention, 2, new QTableWidgetItem(e.job()));
    mPreventionTable->getTable()->setItem(rowPrevention, 3, new QTableWidgetItem(manager));
    mPreventionTable->getTable()->setItem(rowPrevention, 4, new QTableWidgetItem(e.prevPlan()));
    mPreventionTable->getTable()->setItem(rowPrevention, 5, new QTableWidgetItem(plan_signed_str));
    mPreventionTable->getTable()->selectRow(rowPrevention);
    QColor lightRed(255, 220, 220);
    for (int col = 0; col < mPreventionTable->getTable()->columnCount(); ++col) {
        if(!e.signedPlan())
            mPreventionTable->getTable()->item(row, col)->setBackground(lightRed);
        if (auto item = mPreventionTable->getTable()->item(rowPrevention, col))
            item->setTextAlignment(Qt::AlignCenter);
    }
    mPreventionTable->getTable()->item(row, 2)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter); // align job left
}
void TabWidget::updateCmpt(const QList<Employee> &employees) {
    counterGeneral->setText(QString::fromStdString(to_string(employees.size())));
    counterPrevention->setText(QString::fromStdString(to_string(employees.size())));
}

