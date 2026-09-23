#ifndef CONFIGSERVERWINDOW_H
#define CONFIGSERVERWINDOW_H


// Widgets
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>

class ConfigServerWindow : public QDialog
{
    public:
        explicit ConfigServerWindow(QWidget *parent=nullptr);

        QString toJson();


    private:
        QSpinBox *mPort;
        QLineEdit *mHost;
        QLineEdit *mDbPath;
        QPushButton *btnValider;
        QPushButton *btnAnnuler;
};

#endif // CONFIGSERVERWINDOW_H
