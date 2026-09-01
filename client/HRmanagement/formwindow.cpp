#include "formwindow.hpp"

FormWindow::FormWindow()
    : QDialog()
{
    setWindowTitle("Nouveau collaborateur");

    // Formulaire
    QFormLayout *formLayout = new QFormLayout();
    m_txtNom = new QLineEdit(this);
    m_txtNom->setPlaceholderText("Ex: Dupont");
    m_txtPrenom = new QLineEdit(this);
    m_txtPrenom->setPlaceholderText("Ex: Julien");

    auto *birthDate = new QDateEdit(this);
    birthDate->setDisplayFormat("dd/MM/yyyy");
    birthDate->setCalendarPopup(true); // Affiche un calendrier au clic

    auto *jobEdit = new QLineEdit(this);
    jobEdit->setPlaceholderText("Ex: responsable RH");

    auto *statusBox = new QCheckBox("Statut cadre", this);
    statusBox->setChecked(false);

    auto *posSyntSpinBox = new QDoubleSpinBox(this);
    posSyntSpinBox->setRange(0.0, 5.0);
    posSyntSpinBox->setDecimals(1);
    posSyntSpinBox->setSingleStep(0.1);
    posSyntSpinBox->setValue(2.1);

    auto *coefSyntSpinBox = new QSpinBox(this);
    coefSyntSpinBox->setRange(1, 500);
    coefSyntSpinBox->setValue(130);

    auto *startDate = new QDateEdit(QDate::currentDate(),this);
    startDate->setDisplayFormat("dd/MM/yyyy");
    startDate->setCalendarPopup(true); // Affiche un calendrier au clic

    auto *planCombo = new QComboBox(this);
    planCombo->addItems({"", "Plan A", "Plan B", "Plan C"});

    auto *signedPlanBox = new QCheckBox("Plan signé", this);
    signedPlanBox->setChecked(true);


    // Ajout des paires Libellé -> Champ au layout de formulaire
    formLayout->addRow("Nom :", m_txtNom);
    formLayout->addRow("Prénom :", m_txtPrenom);
    formLayout->addRow("Naissance :", birthDate);
    formLayout->addRow("Poste :", jobEdit);
    formLayout->addRow("Statut cadre :", statusBox);
    formLayout->addRow("Position (Syntec) :", posSyntSpinBox);
    formLayout->addRow("Coefficient (Syntec) :", coefSyntSpinBox);
    formLayout->addRow("Début:", startDate);
    formLayout->addRow("Plan de prévention :", planCombo);
    formLayout->addRow("Plan signé :", signedPlanBox);
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

FormWindow::FormWindow(const Employee &e)
    : QDialog()
{
    setWindowTitle("Modifier collaborateur");

    // Formulaire
    QFormLayout *formLayout = new QFormLayout();
    m_txtNom = new QLineEdit(QString::fromStdString(e.lastname()), this);
    m_txtPrenom = new QLineEdit(QString::fromStdString(e.firstname()), this);

    auto *birthDate = new QDateEdit(this);
    birthDate->setDisplayFormat("dd/MM/yyyy");
    birthDate->setCalendarPopup(true); // Affiche un calendrier au clic

    auto *jobEdit = new QLineEdit(this);
    jobEdit->setPlaceholderText("Ex: responsable RH");

    auto *statusBox = new QCheckBox("Statut cadre", this);
    statusBox->setChecked(false);

    auto *posSyntSpinBox = new QDoubleSpinBox(this);
    posSyntSpinBox->setRange(0.0, 5.0);
    posSyntSpinBox->setDecimals(1);
    posSyntSpinBox->setSingleStep(0.1);
    posSyntSpinBox->setValue(2.1);

    auto *coefSyntSpinBox = new QSpinBox(this);
    coefSyntSpinBox->setRange(1, 500);
    coefSyntSpinBox->setValue(130);

    auto *startDate = new QDateEdit(QDate::currentDate(),this);
    startDate->setDisplayFormat("dd/MM/yyyy");
    startDate->setCalendarPopup(true); // Affiche un calendrier au clic

    auto *planCombo = new QComboBox(this);
    planCombo->addItems({"", "Plan A", "Plan B", "Plan C"});

    auto *signedPlanBox = new QCheckBox("Plan signé", this);
    signedPlanBox->setChecked(true);


    // Ajout des paires Libellé -> Champ au layout de formulaire
    formLayout->addRow("Nom :", m_txtNom);
    formLayout->addRow("Prénom :", m_txtPrenom);
    formLayout->addRow("Naissance :", birthDate);
    formLayout->addRow("Poste :", jobEdit);
    formLayout->addRow("Statut cadre :", statusBox);
    formLayout->addRow("Position (Syntec) :", posSyntSpinBox);
    formLayout->addRow("Coefficient (Syntec) :", coefSyntSpinBox);
    formLayout->addRow("Début:", startDate);
    formLayout->addRow("Plan de prévention :", planCombo);
    formLayout->addRow("Plan signé :", signedPlanBox);
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



QString FormWindow::getNom() const { return m_txtNom->text(); }
QString FormWindow::getPrenom() const { return m_txtPrenom->text(); }
