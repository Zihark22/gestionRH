#include "mainwindow.hpp"

#include "parameters.hpp"

#include "viewmanager.hpp"
#include "homewidget.hpp"

#include <QStackedWidget>
#include <QHeaderView>
#include <QMenu>
#include <QMenuBar>
#include <QTableWidgetItem>
#include <QApplication>

MainWindow::MainWindow(HRmanagement *hr, QWidget *parent) : QMainWindow(parent) {

    setWindowTitle("ERP Scalian - RH management");
    resize(900, 800);
    setWindowModality(Qt::ApplicationModal);

    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    viewManager = new ViewManager(stack, this);

    // Instanciation des vues
    auto *homeView = new HomeWidget(this);
    tabView = new TabWidget(this);
    errorView = new ErrorWidget("Erreur à l'initialisation...", this);

    // Enregistrement
    viewManager->registerView(ScreenId::Home, homeView);
    viewManager->registerView(ScreenId::TableViewer, tabView);
    viewManager->registerView(ScreenId::Error, errorView);

    // Câblage des requêtes de navigation vers le manager
    connect(homeView, &HomeWidget::requestNavigation, viewManager, &ViewManager::navigateTo);

    connect(homeView, &HomeWidget::requestNavigation, hr, &HRmanagement::loadData);
    connect(tabView, &TabWidget::requestNavigation, viewManager, &ViewManager::navigateTo);
    connect(errorView, &ErrorWidget::requestNavigation, viewManager, &ViewManager::navigateTo);

    // Câblage des signaux vers la classe métier HRmanagement
    connect(tabView, &TabWidget::openEditEmployee, hr, &HRmanagement::openEditEmployeeWindow);
    connect(tabView, &TabWidget::addingEmployee, hr, &HRmanagement::addingEmployee);
    connect(this, &MainWindow::reloadData, hr, &HRmanagement::loadData);
    connect(this, &MainWindow::openConfigAppWindow, hr, &HRmanagement::openConfigAppWindow);
    connect(this, &MainWindow::openConfigServerWindow, hr, &HRmanagement::openConfigServerWindow);
    connect(this, &MainWindow::openLogs, hr, &HRmanagement::openLogs);
    connect(hr, &HRmanagement::employeesListUpdated, this, &MainWindow::employeesUpdate);
    connect(hr, &HRmanagement::errorDetected, this, &MainWindow::errorDisplay);




    connect(hr, &HRmanagement::onEmployeeAdded, tabView, &TabWidget::onEmployeeAdded);
    connect(hr, &HRmanagement::onEmployeeModified, tabView, &TabWidget::onEmployeeModified);


    // Affichage de la vue de départ
    viewManager->navigateTo(ScreenId::Home);


    // Création de la barre de navigation
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


}


void MainWindow::employeesUpdate(const QList<Employee> &employees) {
    tabView->employeesUpdate(employees);
    viewManager->navigateTo(ScreenId::TableViewer);
}

// display error inside window
void MainWindow::errorDisplay(const QString &msg) {
    errorView->setErrorMessage(msg);
    viewManager->navigateTo(ScreenId::Error);
}

