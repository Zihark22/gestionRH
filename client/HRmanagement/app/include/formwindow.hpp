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

/// Dialog window containing form to fill employee data
class FormWindow : public QDialog
{
public:
    explicit FormWindow(QWidget *parent = nullptr);
    explicit FormWindow(const QList<QPair<uint, QString>> &managers, QWidget *parent = nullptr);
    explicit FormWindow(const Employee &e, const QList<QPair<uint, QString>> &managers,QWidget *parent = nullptr);

    /// Build an employee from the entered form values
    Employee toEmployee();

private:

    static QStringList optionsPlan; ///< Available prevention plan choices

    /// Initialize the form fields
    void initAttributes();

    /// Configure the widgets and labels
    void configAttributes();


    QFormLayout *formLayout; ///< Main form layout
    QPushButton *btnValider; ///< Save button
    QPushButton *btnAnnuler; ///< Cancel button
    QLineEdit *mFirstname; ///< First name field
    QLineEdit *mLastname; ///< Last name field
    QDateEdit * mBirthdate; ///< Birth date field
    QDateEdit *mStartDate; ///< Start date field
    QLineEdit *mJobEdit; ///< Job title field
    QCheckBox *mSignedPlanBox; ///< Signed plan checkbox
    QCheckBox *mStatusBox; ///< Executive status checkbox
    QDoubleSpinBox *mPositionBox; ///< Syntec position field
    QSpinBox *mCoefBox; ///< Syntec coefficient field
    QComboBox *mPlanCombo; ///< Prevention plan selector
    QComboBox *mManager; ///< Manager selector
    uint mId; ///< Employee identifier for updates

};

#endif // FORMWINDOW_HPP
