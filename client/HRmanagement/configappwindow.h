#ifndef CONFIGAPPWINDOW_H
#define CONFIGAPPWINDOW_H

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

class ConfigAppWindow : public QDialog
{
    public:
        explicit ConfigAppWindow(QWidget *parent=nullptr);
        explicit ConfigAppWindow(const int port, const QString host, QWidget *parent=nullptr);

        int getPort();
        QString getHost();

    private:
        QSpinBox *m_port;
        QLineEdit *m_host;
        QPushButton *m_btnValider;
        QPushButton *m_btnAnnuler;

};

#endif // CONFIGAPPWINDOW_H
