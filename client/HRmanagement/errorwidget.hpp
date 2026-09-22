#ifndef ERRORWIDGET_H
#define ERRORWIDGET_H

#include "parameters.hpp"

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class ErrorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ErrorWidget(const QString &error_message, QWidget *parent = nullptr);

public slots:
    void setErrorMessage(const QString &msg);

signals:
    void requestNavigation(ScreenId targetScreen);

private :
    QLabel *message;
};

#endif // ERRORWIDGET_H
