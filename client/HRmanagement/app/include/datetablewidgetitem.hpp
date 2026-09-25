#ifndef DATETABLEWIDGETITEM_HPP
#define DATETABLEWIDGETITEM_HPP

#include <QTableWidgetItem>
#include <QDate>

/// Table item with chronological sorting support
class DateTableWidgetItem : public QTableWidgetItem
{
    public:
        explicit DateTableWidgetItem(const QDate &date);

        bool operator<(const QTableWidgetItem &other) const override;

        /// Return the stored date
        QDate date() const;

    private:
        QDate mDate; ///< Stored date value
};

#endif // DATETABLEWIDGETITEM_HPP
