#include "formwindow.hpp"

QStringList FormWindow::optionsPlan = {"Plan A", "Plan B", "Plan C"};

FormWindow::FormWindow(QWidget *parent) : QDialog(parent) {
    formLayout = new QFormLayout();
    resize(500, 400);

    mLastname = new QLineEdit(this);
    mFirstname = new QLineEdit(this);
    mJobEdit = new QLineEdit(this);

    mSignedPlanBox = new QCheckBox("Plan signé", this);
    mStatusBox = new QCheckBox("Statut cadre", this);


    mCoefBox = new QSpinBox(this);
    mCoefBox->setRange(90, 270);
    mCoefBox->setSingleStep(5);

    mPositionBox = new QDoubleSpinBox(this);
    mPositionBox->setRange(1.0, 3.3);
    mPositionBox->setDecimals(1);
    mPositionBox->setSingleStep(0.1);

    mStartDate = new QDateEdit(QDate::currentDate(),this);
    mStartDate->setDisplayFormat("dd/MM/yyyy");
    mStartDate->setCalendarPopup(true); // Affiche un calendrier au clic
    mStartDate->setMaximumDate(QDate::currentDate());
    mStartDate->setMinimumDate(QDate(2010, 1, 1));

    mBirthdate = new QDateEdit(this);
    mBirthdate->setDisplayFormat("dd/MM/yyyy");
    mBirthdate->setCalendarPopup(true); // Affiche un calendrier au clic
    mBirthdate->setMaximumDate(QDate::currentDate());
    mBirthdate->setMinimumDate(QDate(1950, 1, 1));

    mPlanCombo = new QComboBox(this);
    mPlanCombo->addItems(FormWindow::optionsPlan);

    mManager = new QComboBox(this);
    mId = -1;


    // Ajout des paires Libellé -> Champ au layout de formulaire
    formLayout->addRow("Nom :", mLastname);
    formLayout->addRow("Prénom :", mFirstname);
    formLayout->addRow("Naissance :", mBirthdate);
    formLayout->addRow("Poste :", mJobEdit);
    formLayout->addRow("Statut cadre :", mStatusBox);
    formLayout->addRow("Manager :", mManager);
    formLayout->addRow("Position (Syntec) :", mPositionBox);
    formLayout->addRow("Coefficient (Syntec) :", mCoefBox);
    formLayout->addRow("Début:", mStartDate);
    formLayout->addRow("Plan de prévention :", mPlanCombo);
    formLayout->addRow("Plan signé :", mSignedPlanBox);
    formLayout->setItem(formLayout->rowCount(), QFormLayout::SpanningRole,
                        new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding)); // stretch space line


    // Boutons de validation
    btnValider = new QPushButton("Valider", this);
    btnAnnuler = new QPushButton("Annuler", this);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnValider);
    btnLayout->addWidget(btnAnnuler);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(btnLayout);

    // Relier les boutons aux slots intégrés de QDialog (accept/reject)
    connect(btnValider, &QPushButton::clicked, this, &QDialog::accept);
    connect(btnAnnuler, &QPushButton::clicked, this, &QDialog::reject);
}

FormWindow::FormWindow(const QList<QPair<int, QString>> &managers, QWidget *parent) : FormWindow(parent) {
    setWindowTitle("Nouveau collaborateur");

    // Formulaire
    mLastname->setPlaceholderText("Ex: Dupont");
    mFirstname->setPlaceholderText("Ex: Julien");
    mJobEdit->setPlaceholderText("Ex: responsable RH");
    mStatusBox->setChecked(false);
    mPositionBox->setValue(2.1);
    mCoefBox->setValue(100);
    mSignedPlanBox->setChecked(true);

    for (const auto &pair : managers)
        mManager->addItem(pair.second, pair.first);
}

FormWindow::FormWindow(const Employee &e, const QList<QPair<int, QString>> &managers, QWidget *parent) : FormWindow(parent) {
    setWindowTitle("Modifier collaborateur");

    mId = e.id();
    mLastname->setText(QString::fromStdString(e.lastname()));
    mFirstname->setText(QString::fromStdString(e.firstname()));
    mJobEdit->setText(QString::fromStdString(e.job()));
    mBirthdate->setDate(QDate::fromString(QString::fromStdString(e.birthdate().toString()), "dd'/'MM'/'yyyy"));

    if(e.isExecutive())
        mStatusBox->setChecked(true);
    else
        mStatusBox->setChecked(false);

    mPositionBox->setValue(e.position());
    mCoefBox->setValue(e.coefficient());
    mStartDate->setDate(QDate::fromString(QString::fromStdString(e.startDate().toString()), "dd'/'MM'/'yyyy"));
    mPlanCombo->setCurrentIndex(FormWindow::optionsPlan.indexOf(e.prevPlan()));

    if(e.signedPlan())
        mSignedPlanBox->setChecked(true);
    else
        mSignedPlanBox->setChecked(false);

    for (const auto &pair : managers) {
        mManager->addItem(pair.second, pair.first);
        if(e.managerId()==pair.first)
            mManager->setCurrentIndex(mManager->count()-1);
    }
}

Employee FormWindow::toEmployee() {
    Employee e = Employee();
    e.setLastname(mLastname->text().toStdString());
    e.setFirstname(mFirstname->text().toStdString());

    e.setCoefficient(mCoefBox->value());
    e.setPosition(mPositionBox->value());

    e.setBirthdate(mBirthdate->text().toStdString());
    e.setStartDate(mStartDate->text().toStdString());

    if(mSignedPlanBox->isChecked())
        e.setSignedPlan(1);
    else
        e.setSignedPlan(0);

    if(mStatusBox->isChecked())
        e.setExecutiveStatus(1);
    else
        e.setExecutiveStatus(0);

    e.setPrevPlan(mPlanCombo->currentText().toStdString());

    e.setJob(mJobEdit->text().toStdString());

    e.setManagerId(mManager->currentData().toInt());

    e.setId(mId);

    return e;
}
