#include "mainwindow.hpp"

#include <QApplication>
#include <QFile>
#include <QApplication>

void applyStyleSheet(QApplication &app) {
    QFile file(":/style.qss");

    if (file.open(QFile::ReadOnly | QFile::Text)) {
        app.setStyleSheet(QLatin1String(file.readAll()));
        file.close();
    }
    else {
        qWarning() << "Impossible de charger la feuille de style QSS :" << file.errorString();
    }
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    applyStyleSheet(a); // Appliquer le style globalement

    MainWindow w;
    w.show();
    return a.exec();
}
