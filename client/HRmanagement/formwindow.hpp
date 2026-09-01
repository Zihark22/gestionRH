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
#include <QCheckBox>
#include <QLabel>
#include <QMessageBox>


class FormWindow : public QDialog
{
public:
    FormWindow();
    FormWindow(const Employee &e);

    // Getters pour récupérer la saisie après validation
    QString getNom() const;
    QString getPrenom() const;

private:
    QLineEdit *m_txtNom;
    QLineEdit *m_txtPrenom;
    QPushButton *m_btnValider;
    QPushButton *m_btnAnnuler;
};

#endif // FORMWINDOW_HPP
