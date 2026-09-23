#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QWidget>
#include <QStringList>
#include <QTableWidget>

class TableWidget : public QWidget {
    Q_OBJECT

public:
    QTableWidget *m_table{nullptr};
    explicit TableWidget(const QStringList &headers, QWidget *parent = nullptr);
};

#endif // TABLEWIDGET_H
