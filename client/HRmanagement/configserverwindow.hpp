#ifndef CONFIGSERVERWINDOW_H
#define CONFIGSERVERWINDOW_H

#include <QDialog>

// Widgets
#include <QVBoxLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QDateEdit>
#include <QDate>
#include <QCheckBox>
#include <QLabel>
#include <QMessageBox>

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
