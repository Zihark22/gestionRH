#include "mainwindow.hpp"
#include "apiclientwindow.hpp"

#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    MainWindow window; // MainWindow or ApiClientWindow
    window.show();

    return app.exec(); //  app.exec() or QApplication::exec()
}
