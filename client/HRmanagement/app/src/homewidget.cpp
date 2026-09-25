#include "include/homewidget.hpp"

HomeWidget::HomeWidget(QWidget *parent) : QWidget(parent) {
    auto *layout = new QVBoxLayout(this);
    layout->addStretch();
    auto *message = new QLabel("Bienvenu sur l'ERP Scalian",this);
    // message->setAlignment(Qt::AlignCenter);
    message->setStyleSheet("color: purple; font-size: 40px;");
    layout->addWidget(message, 0, Qt::AlignCenter);
    auto *btnOpenTable = new QPushButton("Accéder aux données", this);
    btnOpenTable->setMaximumWidth(200);
    btnOpenTable->setStyleSheet("margin-top: 50px;");
    layout->addWidget(btnOpenTable, 0, Qt::AlignCenter);
    layout->addStretch();


    // Emit the navigation request when the button is clicked
    connect(btnOpenTable, &QPushButton::clicked, this, [this]() {
        emit requestNavigation(ScreenId::TableViewer);
    });
}
