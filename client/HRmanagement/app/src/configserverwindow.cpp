#include "include/configserverwindow.hpp"

#include <QVBoxLayout>
#include <QFormLayout>

ConfigServerWindow::ConfigServerWindow(QWidget *parent) : QDialog(parent) {
    resize(400, 200);
    setWindowTitle("Configuration serveur");

    this->mPort = new QSpinBox(this);
    this->mHost = new QLineEdit(this);
    this->mDbPath = new QLineEdit(this);

    mPort->setValue(80);
    mPort->setSingleStep(1);
    mPort->setRange(1,65000);
    mHost->setPlaceholderText("ex : 127.0.0.1");
    mDbPath->setPlaceholderText("ex: data/mydatabase.db");

    // Add label-field pairs to the form layout
    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow("Port :", mPort);
    formLayout->addRow("Hôte :", mHost);
    formLayout->addRow("Chemin de la BDD :", mDbPath);
    formLayout->setItem(formLayout->rowCount(), QFormLayout::SpanningRole,
                        new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding)); // stretch space line

    // Create the validation buttons
    btnValider = new QPushButton("Valider", this);
    btnAnnuler = new QPushButton("Annuler", this);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnValider);
    btnLayout->addWidget(btnAnnuler);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(btnLayout);

    // Connect the buttons to QDialog's built-in slots
    connect(btnValider, &QPushButton::clicked, this, &QDialog::accept);
    connect(btnAnnuler, &QPushButton::clicked, this, &QDialog::reject);
}

QString ConfigServerWindow::toJson() {
    QString json = "[{";
    json += "\"host\":\"" + mHost->text() + "\",";
    json += "\"port\":" + QString::fromStdString(std::to_string(mPort->value())) + ",";
    json += "\"db_path\":\"" + mDbPath->text() + "\"";
    json += "}]";
    return json;
}
