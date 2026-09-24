#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QWidget>
#include <QStringList>
#include <QTableWidget>

class TableWidget : public QWidget {
    Q_OBJECT

public:
    explicit TableWidget(const QStringList &headers, QWidget *parent = nullptr);

    QTableWidget* getTable() { return mTable; }

private :
    QTableWidget *mTable{nullptr};
};

#endif // TABLEWIDGET_H
