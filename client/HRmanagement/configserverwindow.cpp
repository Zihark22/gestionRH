#include "configserverwindow.hpp"

ConfigServerWindow::ConfigServerWindow(QWidget *parent)
    : QDialog(parent)
{
    resize(400, 200);
    setWindowTitle("Configuration serveur");

    this->m_port = new QSpinBox(this);
    this->m_host = new QLineEdit(this);
    this->m_db_path = new QLineEdit(this);

    m_port->setValue(80);
    m_port->setSingleStep(1);
    m_port->setRange(1,65000);
    m_host->setPlaceholderText("ex : 127.0.0.1");
    m_db_path->setPlaceholderText("ex: data/mydatabase.db");

    // Ajout des paires Libellé -> Champ au layout de formulaire
    QFormLayout* formLayout = new QFormLayout();
    formLayout->addRow("Port :", m_port);
    formLayout->addRow("Hôte :", m_host);
    formLayout->addRow("Chemin de la BDD :", m_db_path);
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

QString ConfigServerWindow::toJson() {
    QString json = "[{";
    json += "\"host\":\"" + m_host->text() + "\",";
    json += "\"port\":" + QString::fromStdString(std::to_string(m_port->value())) + ",";
    json += "\"db_path\":\"" + m_db_path->text() + "\"";
    json += "}]";
    return json;
}
