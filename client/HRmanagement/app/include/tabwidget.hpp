#ifndef TABWIDGET_H
#define TABWIDGET_H

#include "parameters.hpp"
#include "employee.hpp"
#include "tablewidget.hpp"

#include <QWidget>
#include <QTabWidget>
#include <QList>
#include <QLabel>

/// Tab view with both tables
class TabWidget : public QWidget {
    Q_OBJECT

public:
    explicit TabWidget(QWidget *parent = nullptr);

    /// Refresh the employee tables with the latest employee data
    void employeesUpdate(const QList<Employee> &employees);

signals:

    /// Request a navigation change
    void requestNavigation(ScreenId targetScreen);

    /// Request an employee edit form
    void openEditEmployee(const uint &id, const int &row);

    /// Request the creation form for a new employee
    void addingEmployee();

public slots:
    /// Update the employee data after a modification
    void onEmployeeModified(const int &row, const Employee &e, const QString &manager);
    /// Update the UI after adding a new employee
    void onEmployeeAdded(const QList<Employee> &employees, const QString &manager);
    /// Refresh the row matching the edited employee
    void updateRows(const int &row, const Employee &e, const QString &manager);
    /// Refresh the counters displayed in the tabs
    void updateCmpt(const QList<Employee> &employees);

private slots:

    /// Open the edit form when a row is double-clicked
    void onTableDoubleClicked(int row, int column);

private:
    QTabWidget *mTab{nullptr}; ///< Main tab container
    TableWidget * mGeneralTable{nullptr}; ///< General employee table
    TableWidget * mPreventionTable{nullptr}; ///< Prevention plan table
    QLabel *counterGeneral; ///< Employee count for the general tab
    QLabel *counterPrevention; ///< Employee count for the prevention tab

    /// Create the general employee tab
    QWidget* createGeneralTab();

    /// Create the prevention tab
    QWidget* createPreventionTab();
};


#endif // TABWIDGET_H
