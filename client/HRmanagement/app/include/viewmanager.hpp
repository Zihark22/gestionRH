#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include "parameters.hpp"

#include <QObject>
#include <QStackedWidget>
#include <unordered_map>

/// Handle different views/widgets to display on app
class ViewManager : public QObject {
    Q_OBJECT

public:
    explicit ViewManager(QStackedWidget *stackedWidget, QObject *parent = nullptr);

    /// Register a view in the stack
    void registerView(ScreenId id, QWidget *view);

public slots:
    /// Navigate to the requested screen
    void navigateTo(ScreenId id);

signals:
    void screenChanged(ScreenId id);

private:
    QStackedWidget *mStackedWidget{nullptr}; ///< View container
    std::unordered_map<ScreenId, QWidget*> mViews; ///< Registered screens
};


#endif // VIEWMANAGER_H
