#include "formwindow.hpp"

QStringList FormWindow::optionsPlan = {"Plan A", "Plan B", "Plan C"};

FormWindow::FormWindow(QWidget *parent)
    : QDialog(parent)
{
    formLayout = new QFormLayout();
    resize(500, 400);

    m_txtLastname = new QLineEdit(this);
    m_txtFirstname = new QLineEdit(this);
    m_jobEdit = new QLineEdit(this);

    m_signedPlanBox = new QCheckBox("Plan signé", this);
    m_statusBox = new QCheckBox("Statut cadre", this);


    m_coefSyntSpinBox = new QSpinBox(this);
    m_coefSyntSpinBox->setRange(90, 270);
    m_coefSyntSpinBox->setSingleStep(5);

    m_posSyntSpinBox = new QDoubleSpinBox(this);
    m_posSyntSpinBox->setRange(1.0, 3.3);
    m_posSyntSpinBox->setDecimals(1);
    m_posSyntSpinBox->setSingleStep(0.1);

    m_startDate = new QDateEdit(QDate::currentDate(),this);
    m_startDate->setDisplayFormat("dd/MM/yyyy");
    m_startDate->setCalendarPopup(true); // Affiche un calendrier au clic
    m_startDate->setMaximumDate(QDate::currentDate());
    m_startDate->setMinimumDate(QDate(2010, 1, 1));

    m_birthDate = new QDateEdit(this);
    m_birthDate->setDisplayFormat("dd/MM/yyyy");
    m_birthDate->setCalendarPopup(true); // Affiche un calendrier au clic
    m_birthDate->setMaximumDate(QDate::currentDate());
    m_birthDate->setMinimumDate(QDate(1950, 1, 1));

    m_planCombo = new QComboBox(this);
    m_planCombo->addItems(FormWindow::optionsPlan);

    m_manager = new QComboBox(this);
    m_id = -1;


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

FormWindow::FormWindow(const QList<QPair<int, QString>> &managers, QWidget *parent)
    : FormWindow(parent)
{
    setWindowTitle("Nouveau collaborateur");

    // Formulaire
    m_txtLastname->setPlaceholderText("Ex: Dupont");
    m_txtFirstname->setPlaceholderText("Ex: Julien");
    m_jobEdit->setPlaceholderText("Ex: responsable RH");
    m_statusBox->setChecked(false);
    m_posSyntSpinBox->setValue(2.1);
    m_coefSyntSpinBox->setValue(100);
    m_signedPlanBox->setChecked(true);

    for (const auto &pair : managers) {
        m_manager->addItem(pair.second, pair.first);
    }
}

FormWindow::FormWindow(const Employee &e, const QList<QPair<int, QString>> &managers, QWidget *parent)
    : FormWindow(parent)
{
    setWindowTitle("Modifier collaborateur");

    m_id = e.id();
    m_txtLastname->setText(QString::fromStdString(e.lastname()));
    m_txtFirstname->setText(QString::fromStdString(e.firstname()));
    m_jobEdit->setText(QString::fromStdString(e.job()));
    m_birthDate->setDate(QDate::fromString(QString::fromStdString(e.birthdate().toString()), "dd'/'MM'/'yyyy"));

    if(e.is_executive())
        m_statusBox->setChecked(true);
    else
        m_statusBox->setChecked(false);

    m_posSyntSpinBox->setValue(e.position());
    m_coefSyntSpinBox->setValue(e.coefficient());
    m_startDate->setDate(QDate::fromString(QString::fromStdString(e.start_date().toString()), "dd'/'MM'/'yyyy"));
    m_planCombo->setCurrentIndex(FormWindow::optionsPlan.indexOf(e.prev_plan()));

    if(e.signed_plan())
        m_signedPlanBox->setChecked(true);
    else
        m_signedPlanBox->setChecked(false);

    for (const auto &pair : managers) {
        m_manager->addItem(pair.second, pair.first);
        if(e.manager_id()==pair.first)
            m_manager->setCurrentIndex(m_manager->count()-1);
    }
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

    e.set_manager_id(m_manager->currentData().toInt());

    e.set_id(m_id);

    return e;
}
