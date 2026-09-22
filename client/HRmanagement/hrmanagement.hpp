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


        /// Actions ///

        void openConfigServerWindow();
        void openConfigAppWindow();
        void openLogs();

        void start();

    signals :
        void employeesListUpdated(const QList<Employee> &employees);
        void errorDetected(const QString & msg);


        /// Méthodes de réponse API ///

        void onEmployeeAdded(const int &id, const QList<Employee> &employees);
        void onEmployeeModified(const int row, const Employee &e);

    public slots :
        void loadData();
        void onEmployeeAdd(const int &id);

        void openEditEmployeeWindow(const int id, const int row);
        void addingEmployee();
        void onConfigModified(const std::string json);


    private:

        /// Attributs ///

        std::unique_ptr<ApiClient> apiClient;   ///< API features
        QList<Employee> employees;              ///< Liste des employés et leurs donénes
        QList<QPair<int, QString>> managers;    ///< Liste des noms des managers associés à leur ID d'employé


        /// Méthodes de gestion BDD locale ///

        void parseMyJson();
        void extractManagers();
        QString get_manager_name(const int &manager_id);


        /// MAJ IHM ///

        void updateRows(const int &row, const Employee &e);
        void updateCmpt();



};

#endif // HRMANAGEMENT_HPP
