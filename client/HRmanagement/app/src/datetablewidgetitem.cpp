#include "include/datetablewidgetitem.hpp"

DateTableWidgetItem::DateTableWidgetItem(const QDate &date)
    : QTableWidgetItem(date.toString("dd/MM/yyyy")), mDate(date) {
    setTextAlignment(Qt::AlignCenter);
}

bool DateTableWidgetItem::operator<(const QTableWidgetItem &other) const {
    const auto *otherDateItem = dynamic_cast<const DateTableWidgetItem*>(&other);
    if (otherDateItem) {
        return this->mDate < otherDateItem->mDate;
    }
    return QTableWidgetItem::operator<(other);
}

QDate DateTableWidgetItem::date() const {
    return mDate;
}
