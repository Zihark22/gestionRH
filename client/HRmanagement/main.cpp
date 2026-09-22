#include "parameters.hpp"
#include "hrmanagement.hpp"
#include "mainwindow.hpp"

#include <QApplication>
#include <QFile>

void applyStyleSheet(QApplication &app) {
    QFile file(STYLE_FILE_NAME);

    if (file.open(QFile::ReadOnly | QFile::Text)) {
        app.setStyleSheet(QLatin1String(file.readAll()));
        file.close();
    }
    else
        qWarning() << "Impossible de charger la feuille de style QSS :" << file.errorString();
}
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    applyStyleSheet(app); // Appliquer le style globalement

    HRmanagement h;
    MainWindow w(&h); // pour un découplage de l'UI et de la classe métier
    h.start();
    w.show();

    return app.exec();
}
