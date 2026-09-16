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

    void initAttributes();
    void configAttributes();

    // Getters pour récupérer la saisie après validation
    Employee toEmployee();

private:
    QFormLayout *formLayout;
    QPushButton *btnValider;
    QPushButton *btnAnnuler;
    QLineEdit *mFirstname;
    QLineEdit *mLastname;
    QDateEdit * mBirthdate;
    QDateEdit *mStartDate;
    QLineEdit *mJobEdit;
    QCheckBox *mSignedPlanBox;
    QCheckBox *mStatusBox;
    QDoubleSpinBox *mPositionBox;
    QSpinBox *mCoefBox;
    QComboBox *mPlanCombo;
    QComboBox *mManager;
    int mId;

};

#endif // FORMWINDOW_HPP
