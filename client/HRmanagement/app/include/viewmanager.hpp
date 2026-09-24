#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include "parameters.hpp"

#include <QObject>
#include <QStackedWidget>
#include <unordered_map>

class ViewManager : public QObject {
    Q_OBJECT

public:
    explicit ViewManager(QStackedWidget *stackedWidget, QObject *parent = nullptr);

    // Enregistrement d'une vue
    void registerView(ScreenId id, QWidget *view);

public slots:
    // Slot principal de navigation
    void navigateTo(ScreenId id);

signals:
    void screenChanged(ScreenId id);

private:
    QStackedWidget *mStackedWidget{nullptr};
    std::unordered_map<ScreenId, QWidget*> mViews;
};


#endif // VIEWMANAGER_H
