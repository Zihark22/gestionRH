#ifndef CONFIGAPPWINDOW_H
#define CONFIGAPPWINDOW_H

#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QDialog>

/// Configuration dialog window to modify app config
class ConfigAppWindow : public QDialog
{
    public:
        explicit ConfigAppWindow(QWidget *parent=nullptr);
        explicit ConfigAppWindow(const int port, const QString host, QWidget *parent=nullptr);

        /// Return the configured application port
        int getPort();
        
        /// Return the configured application host
        QString getHost();

    private:
        QSpinBox *mPort; ///< Application port selector
        QLineEdit *mHost; ///< Application host input
        QPushButton *btnValider; ///< Save button
        QPushButton *btnAnnuler; ///< Cancel button

};

#endif // CONFIGAPPWINDOW_H
