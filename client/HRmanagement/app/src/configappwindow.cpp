#include "include/configappwindow.hpp"

#include <QVBoxLayout>
#include <QFormLayout>

ConfigAppWindow::ConfigAppWindow(QWidget *parent) : QDialog(parent) {
    resize(400, 200);
    setWindowTitle("Configuration Application");

    this->mPort = new QSpinBox(this);
    this->mHost = new QLineEdit(this);

    mPort->setValue(80);
    mPort->setSingleStep(1);
    mPort->setRange(1,65000);
    mHost->setPlaceholderText("ex : 127.0.0.1");

    // Add label-field pairs to the form layout
    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow("Port :", mPort);
    formLayout->addRow("Hôte :", mHost);
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


ConfigAppWindow::ConfigAppWindow(const int port, const QString host, QWidget *parent) : ConfigAppWindow(parent) {
    this->mHost->setText(host);
    this->mPort->setValue(port);
}


/********* Getters *********/

int ConfigAppWindow::getPort() {
    return this->mPort->value();
}

QString ConfigAppWindow::getHost() {
    return this->mHost->text();
}
