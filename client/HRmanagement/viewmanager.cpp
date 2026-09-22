#include "viewmanager.hpp"

ViewManager::ViewManager(QStackedWidget *stackedWidget, QObject *parent)
    : QObject(parent), m_stackedWidget(stackedWidget)
{}

void ViewManager::registerView(ScreenId id, QWidget *view) {
    if (!view || !m_stackedWidget) return;

    m_views[id] = view;
    m_stackedWidget->addWidget(view);
}

void ViewManager::navigateTo(ScreenId id) {
    auto it = m_views.find(id);
    if (it != m_views.end()) {
        m_stackedWidget->setCurrentWidget(it->second);
        emit screenChanged(id);
    }
}
