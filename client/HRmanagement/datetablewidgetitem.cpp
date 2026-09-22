#include "datetablewidgetitem.hpp"

DateTableWidgetItem::DateTableWidgetItem(const QDate &date)
    : QTableWidgetItem(date.toString("dd/MM/yyyy")), m_date(date) {
    setTextAlignment(Qt::AlignCenter);
}

bool DateTableWidgetItem::operator<(const QTableWidgetItem &other) const {
    const auto *otherDateItem = dynamic_cast<const DateTableWidgetItem*>(&other);
    if (otherDateItem) {
        return this->m_date < otherDateItem->m_date;
    }
    return QTableWidgetItem::operator<(other);
}

QDate DateTableWidgetItem::date() const {
    return m_date;
}
