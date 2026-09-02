#include "formwindow.hpp"

FormWindow::FormWindow(const QStringList &managers)
    : QDialog()
{
    setWindowTitle("Nouveau collaborateur");
    resize(500, 400);

    // Formulaire
    QFormLayout *formLayout = new QFormLayout();
    m_txtLastname = new QLineEdit(this);
    m_txtLastname->setPlaceholderText("Ex: Dupont");
    m_txtFirstname = new QLineEdit(this);
    m_txtFirstname->setPlaceholderText("Ex: Julien");

    m_birthDate = new QDateEdit(this);
    m_birthDate->setDisplayFormat("dd/MM/yyyy");
    m_birthDate->setCalendarPopup(true); // Affiche un calendrier au clic

    m_jobEdit = new QLineEdit(this);
    m_jobEdit->setPlaceholderText("Ex: responsable RH");

    m_statusBox = new QCheckBox("Statut cadre", this);
    m_statusBox->setChecked(false);

    m_posSyntSpinBox = new QDoubleSpinBox(this);
    m_posSyntSpinBox->setRange(0.0, 5.0);
    m_posSyntSpinBox->setDecimals(1);
    m_posSyntSpinBox->setSingleStep(0.1);
    m_posSyntSpinBox->setValue(2.1);

    m_coefSyntSpinBox = new QSpinBox(this);
    m_coefSyntSpinBox->setRange(1, 500);
    m_coefSyntSpinBox->setValue(130);

    m_startDate = new QDateEdit(QDate::currentDate(),this);
    m_startDate->setDisplayFormat("dd/MM/yyyy");
    m_startDate->setCalendarPopup(true); // Affiche un calendrier au clic

    m_planCombo = new QComboBox(this);
    m_planCombo->addItems({"", "Plan A", "Plan B", "Plan C"});

    m_manager = new QComboBox(this);
    m_manager->addItems(managers);

    m_manager->addItems(managers);

    m_signedPlanBox = new QCheckBox("Plan signé", this);
    m_signedPlanBox->setChecked(true);


    // Ajout des paires Libellé -> Champ au layout de formulaire
    formLayout->addRow("Nom :", m_txtLastname);
    formLayout->addRow("Prénom :", m_txtFirstname);
    formLayout->addRow("Naissance :", m_birthDate);
    formLayout->addRow("Poste :", m_jobEdit);
    formLayout->addRow("Statut cadre :", m_statusBox);
    formLayout->addRow("Manager :", m_manager);
    formLayout->addRow("Position (Syntec) :", m_posSyntSpinBox);
    formLayout->addRow("Coefficient (Syntec) :", m_coefSyntSpinBox);
    formLayout->addRow("Début:", m_startDate);
    formLayout->addRow("Plan de prévention :", m_planCombo);
    formLayout->addRow("Plan signé :", m_signedPlanBox);
    formLayout->setItem(formLayout->rowCount(), QFormLayout::SpanningRole,
                        new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding)); // stretch space line


    // Boutons de validation
    m_btnValider = new QPushButton("Valider", this);
    m_btnAnnuler = new QPushButton("Annuler", this);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_btnValider);
    btnLayout->addWidget(m_btnAnnuler);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(btnLayout);

    // Relier les boutons aux slots intégrés de QDialog (accept/reject)
    connect(m_btnValider, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_btnAnnuler, &QPushButton::clicked, this, &QDialog::reject);
}

FormWindow::FormWindow(const Employee &e, const QStringList &managers)
    : QDialog()
{
    setWindowTitle("Modifier collaborateur");
    resize(500, 400);

    // Formulaire
    QFormLayout *formLayout = new QFormLayout();
    m_txtLastname = new QLineEdit(QString::fromStdString(e.lastname()), this);
    m_txtFirstname = new QLineEdit(QString::fromStdString(e.firstname()), this);
    m_jobEdit = new QLineEdit(QString::fromStdString(e.job()), this);

    m_birthDate = new QDateEdit(QDate::fromString(QString::fromStdString(e.birthdate().toString()), "dd'/'MM'/'yyyy"),this);
    m_birthDate->setDisplayFormat("dd/MM/yyyy");
    m_birthDate->setCalendarPopup(true); // Affiche un calendrier au clic

    m_statusBox = new QCheckBox("Statut cadre", this);
    if(e.is_executive())
        m_statusBox->setChecked(true);
    else
        m_statusBox->setChecked(false);

    m_posSyntSpinBox = new QDoubleSpinBox(this);
    m_posSyntSpinBox->setRange(0.0, 5.0);
    m_posSyntSpinBox->setDecimals(1);
    m_posSyntSpinBox->setSingleStep(0.1);
    m_posSyntSpinBox->setValue(e.position());

    m_coefSyntSpinBox = new QSpinBox(this);
    m_coefSyntSpinBox->setRange(1, 500);
    m_coefSyntSpinBox->setValue(e.coefficient());

    m_startDate = new QDateEdit(QDate::fromString(QString::fromStdString(e.start_date().toString()), "dd'/'MM'/'yyyy"),this);
    m_startDate->setDisplayFormat("dd/MM/yyyy");
    m_startDate->setCalendarPopup(true); // Affiche un calendrier au clic

    m_planCombo = new QComboBox(this);
    m_planCombo->addItems({"", "Plan A", "Plan B", "Plan C"});
    int ind = 0;
    if(e.prev_plan()=="Plan A")
        ind = 1;
    else if(e.prev_plan()=="Plan B")
            ind = 2;
    else if(e.prev_plan()=="Plan C")
        ind = 3;
    m_planCombo->setCurrentIndex(ind);

    m_signedPlanBox = new QCheckBox("Plan signé", this);
    if(e.signed_plan())
        m_signedPlanBox->setChecked(true);
    else
        m_signedPlanBox->setChecked(false);

    m_manager = new QComboBox(this);
    m_manager->addItems(managers);

    // Ajout des paires Libellé -> Champ au layout de formulaire
    formLayout->addRow("Nom :", m_txtLastname);
    formLayout->addRow("Prénom :", m_txtFirstname);
    formLayout->addRow("Naissance :", m_birthDate);
    formLayout->addRow("Poste :", m_jobEdit);
    formLayout->addRow("Statut cadre :", m_statusBox);
    formLayout->addRow("Manager :", m_manager);
    formLayout->addRow("Position (Syntec) :", m_posSyntSpinBox);
    formLayout->addRow("Coefficient (Syntec) :", m_coefSyntSpinBox);
    formLayout->addRow("Début:", m_startDate);
    formLayout->addRow("Plan de prévention :", m_planCombo);
    formLayout->addRow("Plan signé :", m_signedPlanBox);
    formLayout->setItem(formLayout->rowCount(), QFormLayout::SpanningRole,
                        new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding)); // stretch space line


    // Boutons de validation
    m_btnValider = new QPushButton("Valider", this);
    m_btnAnnuler = new QPushButton("Annuler", this);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_btnValider);
    btnLayout->addWidget(m_btnAnnuler);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(btnLayout);

    // Relier les boutons aux slots intégrés de QDialog (accept/reject)
    connect(m_btnValider, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_btnAnnuler, &QPushButton::clicked, this, &QDialog::reject);

    // qDebug() << "Depuis formulaire\n" << QString::fromUtf8(parent->getApi()->getResponseData()).toStdString();

}

Employee FormWindow::toEmployee() {
    Employee e = Employee();
    e.set_lastname(m_txtLastname->text().toStdString());
    e.set_firstname(m_txtFirstname->text().toStdString());

    e.set_coefficient(m_coefSyntSpinBox->value());
    e.set_position(m_posSyntSpinBox->value());

    e.set_birthdate(m_birthDate->text().toStdString());
    e.set_start_date(m_startDate->text().toStdString());

    if(m_signedPlanBox->isChecked())
        e.set_signed_plan(1);
    else
        e.set_signed_plan(0);

    if(m_statusBox->isChecked())
        e.set_executive_status(1);
    else
        e.set_executive_status(0);

    e.set_prev_plan(m_planCombo->currentText().toStdString());

    e.set_job(m_jobEdit->text().toStdString());

    return e;
}
