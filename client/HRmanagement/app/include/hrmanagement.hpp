#ifndef HRMANAGEMENT_HPP
#define HRMANAGEMENT_HPP

#include "apiclient.hpp"
#include "employee.hpp"

#include <QWidget>
#include <QPair>
#include <QString>
#include <QList>

/// Main business logic for API calls and UI coordination
class HRmanagement : public QObject {
    Q_OBJECT

    public:
        explicit HRmanagement(QWidget *parent = nullptr);

        void start();

        // UI actions

        void openConfigServerWindow();
        void openConfigAppWindow();
        void openLogs();


    signals :
        void employeesListUpdated(const QList<Employee> &employees);
        void errorDetected(const QString &msg);
        void onEmployeeAdded(const QList<Employee> &employees, const QString &manager);
        void onEmployeeModified(const int &row, const Employee &e, const QString &manager);

    public slots :
        void loadData(); ///< Load employee data and rebuild the UI state
        void onEmployeeAdd(const uint &id);
        void onEmployModify(const int &row, const Employee &e);
        void openEditEmployeeWindow(const uint &id, const int &row);///< Open the form used to edit an employee
        void addingEmployee(); ///< Open the form used to create a new employee
        void onConfigModified(const std::string json);


    private:
        std::unique_ptr<ApiClient> apiClient; ///< API client instance
        QList<Employee> employees;            ///< Employee list
        QList<QPair<uint, QString>> managers; ///< Manager list mapped to employee IDs


        // Manage data

        void parseMyJson();                                     ///< Save employees list
        void extractManagers();                                 ///< Save managers list
        QString get_manager_name(const uint &manager_id);       ///< Return the manager name for the given employee ID
        Employee get_employee_from_id(const uint &employee_id); ///< Return the employee for the given employee ID
};

#endif // HRMANAGEMENT_HPP
