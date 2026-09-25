#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "hrmanagement.hpp"
#include "employee.hpp"
#include "viewmanager.hpp"
#include "tabwidget.hpp"
#include "errorwidget.hpp"

// Main
#include <QWidget>
#include <QMainWindow>
#include <QStackedWidget>

// Widgets
#include <QTabWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QList>

// App main window for HMI
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(HRmanagement *hr, QWidget *parent = nullptr);
    ~MainWindow() = default;

signals:
    void reloadData();
    void openConfigServerWindow();
    void openConfigAppWindow();
    void openLogs();

private:
    QStackedWidget *stack; ///< Screen container
    ViewManager *viewManager; ///< Screen navigation manager
    TabWidget *tabView; ///< Main employee table view
    ErrorWidget *errorView; ///< Error view
};
#endif // MAINWINDOW_HPP
