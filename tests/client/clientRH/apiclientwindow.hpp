#ifndef APICLIENTWINDOW_HPP
#define APICLIENTWINDOW_HPP

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>


class ApiClientWindow : public QWidget {
    Q_OBJECT

private:
    QNetworkAccessManager *networkManager;
    QPushButton *fetchButton;
    QTextEdit *resultTextEdit;

public:
    ApiClientWindow(QWidget *parent = nullptr);

private slots:
    void sendGetRequest();
};

#endif // APICLIENTWINDOW_HPP
