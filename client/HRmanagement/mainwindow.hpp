#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "apiclient.hpp"
#include "employee.hpp"
#include "formwindow.hpp"
#include "configserverwindow.hpp"
#include "configappwindow.hpp"

#include <memory> // pour unique_ptr

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

    // Signal pour ouvrir le formulaire en double cliquant sur une ligne
    void onTableDoubleClicked(int row, int column);

private:

//// Création des onglets (Général et Prévention) ////

    QWidget* createGeneralTab();
    QWidget* createPreventionTab();


/// Remplir les onglets ///

    QTableWidget* fillGeneralTab(QWidget* tab);
    QTableWidget* fillPreventionTab(QWidget* tab);


/// Méthodes de réponse API ///

    void addingEmployee();
    void onEmployeeAdded(int id);
    void onEmployeeModified(const int row, const Employee &e);
    void onConfigModified(const std::string json);


/// Méthodes de gestion BDD locale ///

    void parseMyJson();
    void extractManagers();
    QString get_manager_name(const int &manager_id);


/// Actions ///

    void openConfigServerWindow();
    void openConfigAppWindow();
    void openLogs();


/// MAJ IHM ///

    void updateRows(const int &row, const Employee &e);
    void updateCmpt();
    void reloadData();
    void errorDisplay();


/// Attributs ///

    std::unique_ptr<ApiClient> apiClient;   ///< API features
    QTableWidget* generalTableWidget;       ///< Onglet general
    QTableWidget* preventionTableWidget;    ///< Onglet prevention
    QList<Employee> employees;              ///< Liste des employés et leurs donénes
    QList<QPair<int, QString>> managers;    ///< Liste des noms des managers associés à leur ID d'employé
    QLabel *counterGeneral;                 ///< Label pour le compteur d'employés dans l'onglet général
    QLabel *counterPrevention;              ///< Label pour le compteur d'employés dans l'onglet prévention
};
#endif // MAINWINDOW_HPP
