#include "include/errorwidget.hpp"

ErrorWidget::ErrorWidget(const QString &error_message, QWidget *parent) : QWidget(parent) {
    auto *layout = new QVBoxLayout(this);
    QString full_message = "Vous avez rencontré une erreur ! <br/> " + error_message;

    message = new QLabel(full_message,this);
    message->setTextFormat(Qt::RichText);
    message->setAlignment(Qt::AlignCenter);
    message->setStyleSheet("color: red; font-size: 20px;");
    layout->addWidget(message);

    auto *btnOpenTable = new QPushButton("⬅ Retour au menu", this);
    layout->addWidget(btnOpenTable);

    // Emit the navigation request when the button is clicked
    connect(btnOpenTable, &QPushButton::clicked, this, [this]() {
        emit requestNavigation(ScreenId::Home);
    });
}

void ErrorWidget::setErrorMessage(const QString &msg) {
    QString newmsg = "Vous avez rencontré une erreur ! <br/> "+msg;
    message->setText(newmsg);
    emit requestNavigation(ScreenId::Error);
}
