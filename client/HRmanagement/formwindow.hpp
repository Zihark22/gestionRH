#ifndef FORMWINDOW_HPP
#define FORMWINDOW_HPP

#include "employee.hpp"

#include <QDialog>
#include <QWidget>

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


class FormWindow : public QDialog
{
public:
    explicit FormWindow(QWidget *parent = nullptr);
    explicit FormWindow(const QList<QPair<int, QString>> &managers, QWidget *parent = nullptr);
    explicit FormWindow(const Employee &e, const QList<QPair<int, QString>> &managers,QWidget *parent = nullptr);
    static QStringList optionsPlan;

    // Getters pour récupérer la saisie après validation
    Employee toEmployee();

private:
    QLineEdit *m_txtFirstname;
    QLineEdit *m_txtLastname;
    QPushButton *m_btnValider;
    QPushButton *m_btnAnnuler;
    QDateEdit * m_birthDate;
    QDateEdit *m_startDate;
    QLineEdit *m_jobEdit;
    QCheckBox *m_signedPlanBox;
    QCheckBox *m_statusBox;
    QDoubleSpinBox *m_posSyntSpinBox;
    QSpinBox *m_coefSyntSpinBox;
    QComboBox *m_planCombo;
    QComboBox *m_manager;
    QFormLayout *formLayout;
    int m_id;

};

#endif // FORMWINDOW_HPP
