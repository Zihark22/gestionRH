#ifndef CONFIGAPPWINDOW_H
#define CONFIGAPPWINDOW_H

#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QDialog>

class ConfigAppWindow : public QDialog
{
    public:
        explicit ConfigAppWindow(QWidget *parent=nullptr);
        explicit ConfigAppWindow(const int port, const QString host, QWidget *parent=nullptr);

        int getPort();
        QString getHost();

    private:
        QSpinBox *mPort;
        QLineEdit *mHost;
        QPushButton *btnValider;
        QPushButton *btnAnnuler;

};

#endif // CONFIGAPPWINDOW_H
