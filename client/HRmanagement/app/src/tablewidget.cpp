#include "include/tablewidget.hpp"

#include <QTableWidget>
#include <QVBoxLayout>
#include <QHeaderView>


TableWidget::TableWidget(const QStringList &headers, QWidget *parent) : QWidget{parent} {
    auto *mainLayout = new QVBoxLayout(this);

    mTable = new QTableWidget(this);

    // Configure the table look and behavior
    
    mTable->clearContents();
    mTable->setRowCount(0); // Reset the table rows
    mTable->setColumnCount(headers.size());
    mTable->setHorizontalHeaderLabels(headers);
    mTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // Make the table read-only
    mTable->setAlternatingRowColors(true);
    mTable->setShowGrid(false); // Keep the table appearance clean
    mTable->setSelectionBehavior(QAbstractItemView::SelectRows); // Select full rows
    mTable->setSelectionMode(QAbstractItemView::SingleSelection); // Allow only one selected row
    mTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch); // Stretch the third column to fill space
    mTable->horizontalHeader()->setMinimumSectionSize(150); // Minimum column width
    mTable->setSortingEnabled(true); // Enable sorting on headers
    mTable->verticalHeader()->setVisible(false); // Hide the row numbers

    mainLayout->addWidget(mTable);
}
