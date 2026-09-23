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

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(HRmanagement *hr, QWidget *parent = nullptr);
    ~MainWindow() = default;

    // display error inside view
    void errorDisplay(const QString &msg);

signals:
    void reloadData();
    void openConfigServerWindow();
    void openConfigAppWindow();
    void openLogs();


private:

    /// Attributs ///

    QStackedWidget *stack;
    ViewManager *viewManager;
    TabWidget *tabView;
    ErrorWidget *errorView;
};
#endif // MAINWINDOW_HPP
