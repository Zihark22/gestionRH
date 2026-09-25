#ifndef HOMEWIDGET_HPP
#define HOMEWIDGET_HPP

#include "parameters.hpp"

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

/// Home view for welcoming user
class HomeWidget : public QWidget {
    Q_OBJECT

public:
    explicit HomeWidget(QWidget *parent = nullptr);

signals:
    void requestNavigation(ScreenId targetScreen);
};


#endif // HOMEWIDGET_HPP
