#ifndef ERRORWIDGET_H
#define ERRORWIDGET_H

#include "parameters.hpp"

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

/// Error view to display error message
class ErrorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ErrorWidget(const QString &error_message, QWidget *parent = nullptr);

public slots:
    /// Update the displayed error text
    void setErrorMessage(const QString &msg);

signals:
    void requestNavigation(ScreenId targetScreen);

private :
    QLabel *message; ///< Error message label
};

#endif // ERRORWIDGET_H
