#ifndef TABWIDGET_H
#define TABWIDGET_H

#include "parameters.hpp"
#include "employee.hpp"
#include "tablewidget.hpp"

#include <QWidget>
#include <QTabWidget>
#include <QList>
#include <QLabel>

class TabWidget : public QWidget {
    Q_OBJECT

public:
    explicit TabWidget(QWidget *parent = nullptr);
    void employeesUpdate(const QList<Employee> &employees);

public slots:
    void onEmployeeModified(const int row, const Employee &e);
    void onEmployeeAdded(const int &id, const QList<Employee> &employees);
    void updateRows(const int &row, const Employee &e);
    void updateCmpt(const QList<Employee> &employees);

signals:
    // Signal écouté par le ViewManager
    void requestNavigation(ScreenId targetScreen);

    // signal pour HRmanagement pour ouvrir une fenêtre de modif
    void openEditEmployee(const int &id, const int &row);

    void addingEmployee();

private slots:
    // Signal pour ouvrir le formulaire en double cliquant sur une ligne
    void onTableDoubleClicked(int row, int column);


private:
    QTabWidget *m_tab{nullptr};
    TableWidget * m_generalTable{nullptr};
    TableWidget * m_preventionTable{nullptr};
    QLabel *counterGeneral;
    QLabel *counterPrevention;

    QWidget* createGeneralTab();
    QWidget* createPreventionTab();
};


#endif // TABWIDGET_H
