#ifndef HRMANAGEMENT_HPP
#define HRMANAGEMENT_HPP

#include "apiclient.hpp"
#include "employee.hpp"

#include <QWidget>
#include <QPair>
#include <QString>
#include <QList>

/**
 * @brief Classe métier pour la gestion de l'API et le contrôle de l'IHM
 */
class HRmanagement : public QObject {
    Q_OBJECT

    public:
        explicit HRmanagement(QWidget *parent = nullptr);

        void start();

        /// Actions ///

        void openConfigServerWindow();
        void openConfigAppWindow();
        void openLogs();


    signals :
        void employeesListUpdated(const QList<Employee> &employees);
        void errorDetected(const QString & msg);


        /// Signaux de réponse API ///

        void onEmployeeAdded(const QList<Employee> &employees, const QString &manager);
        void onEmployeeModified(const int &row, const Employee &e, const QString &manager);

    public slots :
        void loadData();
        void onEmployeeAdd(const uint &id);
        void onEmployModify(const int &row, const Employee &e);

        void openEditEmployeeWindow(const uint id, const int row);
        void addingEmployee();
        void onConfigModified(const std::string json);


    private:

        /// Attributs ///

        std::unique_ptr<ApiClient> apiClient;   ///< API features
        QList<Employee> employees;              ///< Liste des employés et leurs donénes
        QList<QPair<uint, QString>> managers;    ///< Liste des noms des managers associés à leur ID d'employé


        /// Méthodes de gestion BDD locale ///

        void parseMyJson();
        void extractManagers();
        QString get_manager_name(const uint &manager_id);
        Employee get_employee_from_id(const uint &employee_id);


        /// MAJ IHM ///

        void updateRows(const int &row, const Employee &e);
        void updateCmpt();

};

#endif // HRMANAGEMENT_HPP
