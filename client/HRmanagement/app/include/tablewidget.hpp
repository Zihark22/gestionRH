#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QWidget>
#include <QStringList>
#include <QTableWidget>

/// Table to display employee data
class TableWidget : public QWidget {
    Q_OBJECT

public:
    explicit TableWidget(const QStringList &headers, QWidget *parent = nullptr);

    /// Return the underlying table widget
    QTableWidget* getTable() { return mTable; }

private :
    QTableWidget *mTable{nullptr}; ///< Table widget instance
};

#endif // TABLEWIDGET_H
