#ifndef CONFIGSERVERWINDOW_H
#define CONFIGSERVERWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>

/// Configuration dialog window to modify server
class ConfigServerWindow : public QDialog
{
    public:
        explicit ConfigServerWindow(QWidget *parent=nullptr);

        /// Serialize the server configuration into JSON
        QString toJson();


    private:
        QSpinBox *mPort; ///< Server port field
        QLineEdit *mHost; ///< Server host field
        QLineEdit *mDbPath; ///< Database path field
        QPushButton *btnValider; ///< Save button
        QPushButton *btnAnnuler; ///< Cancel button
};

#endif // CONFIGSERVERWINDOW_H
