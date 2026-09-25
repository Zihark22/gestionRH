#include "include/mainwindow.hpp"

#include "include/parameters.hpp"

#include "include/viewmanager.hpp"
#include "include/homewidget.hpp"

#include <QStackedWidget>
#include <QHeaderView>
#include <QMenu>
#include <QMenuBar>
#include <QTableWidgetItem>
#include <QApplication>

MainWindow::MainWindow(HRmanagement *hr, QWidget *parent) : QMainWindow(parent) {

    setWindowTitle("ERP Scalian - RH management");
    resize(1200, 700);
    move(300, 200);
    setWindowModality(Qt::ApplicationModal);

    // Create the main navigation bar
    QMenuBar *bar = menuBar();

    QMenu *appMenu = bar->addMenu("Application");
    QAction *reloadAction = appMenu->addAction(tr("Recharger"));
    reloadAction->setShortcut(QKeySequence(tr("Ctrl+R")));
    connect(reloadAction, &QAction::triggered, this, &MainWindow::reloadData);
    QAction *quitAction = appMenu->addAction(tr("&Quitter"));
    quitAction->setShortcuts(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    QMenu *paramMenu = bar->addMenu("&Paramètres");
    QAction *logsAction = paramMenu->addAction(tr("Ouvrir &Logs"));
    logsAction->setShortcut(QKeySequence(tr("Ctrl+L")));
    connect(logsAction, &QAction::triggered, this, &MainWindow::openLogs);
    QAction *configAppAction = paramMenu->addAction(tr("Config &App"));
    configAppAction->setShortcut(QKeySequence(tr("Ctrl+A")));
    connect(configAppAction, &QAction::triggered, this, &MainWindow::openConfigAppWindow);
    QAction *configServAction = paramMenu->addAction(tr("Config &Server"));
    configServAction->setShortcut(QKeySequence(tr("Ctrl+S")));
    connect(configServAction, &QAction::triggered, this, &MainWindow::openConfigServerWindow);


    // Manage the stacked views
    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    viewManager = new ViewManager(stack, this);

    // Instantiate the views
    auto *homeView = new HomeWidget(this);
    tabView = new TabWidget(this);
    errorView = new ErrorWidget("Erreur à l'initialisation...", this);

    // Register the available screens
    viewManager->registerView(ScreenId::Home, homeView);
    viewManager->registerView(ScreenId::TableViewer, tabView);
    viewManager->registerView(ScreenId::Error, errorView);

    // Connect navigation requests to the manager
    connect(homeView, &HomeWidget::requestNavigation, viewManager, &ViewManager::navigateTo);
    connect(homeView, &HomeWidget::requestNavigation, hr, &HRmanagement::loadData); // request when the home button is clicked to load data
    connect(tabView, &TabWidget::requestNavigation, viewManager, &ViewManager::navigateTo);
    connect(errorView, &ErrorWidget::requestNavigation, viewManager, &ViewManager::navigateTo);

    // Connect main-window signals to the business layer
    connect(tabView, &TabWidget::openEditEmployee, hr, &HRmanagement::openEditEmployeeWindow);
    connect(tabView, &TabWidget::addingEmployee, hr, &HRmanagement::addingEmployee);
    connect(this, &MainWindow::reloadData, hr, &HRmanagement::loadData);
    connect(this, &MainWindow::openConfigAppWindow, hr, &HRmanagement::openConfigAppWindow);
    connect(this, &MainWindow::openConfigServerWindow, hr, &HRmanagement::openConfigServerWindow);
    connect(this, &MainWindow::openLogs, hr, &HRmanagement::openLogs);

    // Connect business signals to the table view
    connect(hr, &HRmanagement::employeesListUpdated, tabView, &TabWidget::employeesUpdate);
    connect(hr, &HRmanagement::errorDetected, errorView, &ErrorWidget::setErrorMessage);
    connect(hr, &HRmanagement::onEmployeeAdded, tabView, &TabWidget::onEmployeeAdded);
    connect(hr, &HRmanagement::onEmployeeModified, tabView, &TabWidget::onEmployeeModified);

    // Display the starting screen
    viewManager->navigateTo(ScreenId::Home);
}

