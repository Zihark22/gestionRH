#include "configappwindow.hpp"

// Constructeur général
ConfigAppWindow::ConfigAppWindow(QWidget *parent)
    : QDialog(parent)
{
    resize(400, 200);
    setWindowTitle("Configuration Application");

    this->m_port = new QSpinBox(this);
    this->m_host = new QLineEdit(this);

    m_port->setValue(80);
    m_port->setSingleStep(1);
    m_port->setRange(1,65000);
    m_host->setPlaceholderText("ex : 127.0.0.1");

    // Ajout des paires Libellé -> Champ au layout de formulaire
    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow("Port :", m_port);
    formLayout->addRow("Hôte :", m_host);
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

// Spécifie les valeurs à préremplir à partir de la config
ConfigAppWindow::ConfigAppWindow(const int port, const QString host, QWidget *parent)
    : ConfigAppWindow(parent)
{
    this->m_host->setText(host);
    this->m_port->setValue(port);
}


int ConfigAppWindow::getPort() {
    return this->m_port->value();
}
QString ConfigAppWindow::getHost() {
    return this->m_host->text();
}
