#include "include/viewmanager.hpp"

ViewManager::ViewManager(QStackedWidget *stackedWidget, QObject *parent)
    : QObject(parent), mStackedWidget(stackedWidget)
{}

void ViewManager::registerView(ScreenId id, QWidget *view) {
    if (!view || !mStackedWidget) return;

    mViews[id] = view;
    mStackedWidget->addWidget(view);
}

void ViewManager::navigateTo(ScreenId id) {
    auto it = mViews.find(id);
    if (it != mViews.end()) {
        mStackedWidget->setCurrentWidget(it->second);
        emit screenChanged(id);
    }
}
