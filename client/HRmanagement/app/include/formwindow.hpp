#ifndef FORMWINDOW_HPP
#define FORMWINDOW_HPP

#include "employee.hpp"

// Widgets
#include <QDialog>
#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QDateEdit>
#include <QCheckBox>
#include <QLabel>


class FormWindow : public QDialog
{
public:
    explicit FormWindow(QWidget *parent = nullptr);
    explicit FormWindow(const QList<QPair<uint, QString>> &managers, QWidget *parent = nullptr);
    explicit FormWindow(const Employee &e, const QList<QPair<uint, QString>> &managers,QWidget *parent = nullptr);

    // Getters pour récupérer la saisie après validation
    Employee toEmployee();

private:

    static QStringList optionsPlan;

    void initAttributes();
    void configAttributes();


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
    uint mId;

};

#endif // FORMWINDOW_HPP
