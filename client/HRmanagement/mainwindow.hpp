#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "apiclient.hpp"
#include "employee.hpp"
#include "formwindow.hpp"

// Main
#include <QApplication>
#include <QWidget>
#include <QMainWindow>

// Menus
#include <QMenuBar>
#include <QMenu>
#include <QAction>

// Layouts
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPalette>

// Widgets
#include <QTabWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
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
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QDate>
#include <QDebug>
#include <QAbstractItemView>
#include <QList>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onTableDoubleClicked(int row, int column); // signal pour ouvrir le formulaire en double cliquant sur une ligne

private:
    // Création des onglets (Général et Prévention)
    QWidget* createGeneralTab();
    QWidget* createPreventionTab();

    // Remplir les onglets
    QTableWidget* fillGeneralTab(QWidget* tab);
    QTableWidget* fillPreventionTab(QWidget* tab);

    // Création du formulaire pour ajout
    void addingEmployee();
    void onEmployeeAdded(int id);

    void parseMyJson();
    void extractManagers();
    void updateRows(const int &row, const Employee &e);
    void updateCmpt();
    QString get_manager_name(const int &manager_id);

    // Attributs
    ApiClient *apiClient; // API features
    QTableWidget* generalTableWidget; // onglet general
    QTableWidget* preventionTableWidget; // onglet prevention
    QList<Employee> employees;
    QList<QPair<int, QString>> managers;
    QLabel *counterGeneral;
    QLabel *counterPrevention;
};
#endif // MAINWINDOW_HPP
