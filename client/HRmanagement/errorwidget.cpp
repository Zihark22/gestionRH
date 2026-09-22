#include "errorwidget.hpp"

ErrorWidget::ErrorWidget(const QString &error_message, QWidget *parent) {
    auto *layout = new QVBoxLayout(this);
    QString full_message = "Vous avez rencontré une erreur !\n" + error_message;

    message = new QLabel(full_message,this);
    message->setAlignment(Qt::AlignCenter);
    message->setStyleSheet("color: red; font-size: 20px;");
    layout->addWidget(message);

    auto *btnOpenTable = new QPushButton("⬅ Retour au menu", this);
    layout->addWidget(btnOpenTable);

    // Quand on clique, on émet la demande de navigation
    connect(btnOpenTable, &QPushButton::clicked, this, [this]() {
        emit requestNavigation(ScreenId::Home);
    });
}

void ErrorWidget::setErrorMessage(const QString &msg) {
    message->setText("Vous avez rencontré une erreur !\n"+msg);
}
