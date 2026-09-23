#ifndef DATETABLEWIDGETITEM_HPP
#define DATETABLEWIDGETITEM_HPP

#include <QTableWidgetItem>
#include <QDate>

// Item personnalisé avec tri chronologique
class DateTableWidgetItem : public QTableWidgetItem
{
    public:
        explicit DateTableWidgetItem(const QDate &date);

        bool operator<(const QTableWidgetItem &other) const override;

        QDate date() const;

    private:
        QDate m_date;
};

#endif // DATETABLEWIDGETITEM_HPP
