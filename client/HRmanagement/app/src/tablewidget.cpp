#include "include/tablewidget.hpp"

#include <QTableWidget>
#include <QVBoxLayout>
#include <QHeaderView>


TableWidget::TableWidget(const QStringList &headers, QWidget *parent)
    : QWidget{parent}
{
    auto *mainLayout = new QVBoxLayout(this);

    mTable = new QTableWidget(this);

    // Options
    mTable->clearContents();
    mTable->setRowCount(0); // Réinitialise les lignes
    mTable->setColumnCount(headers.size());
    mTable->setHorizontalHeaderLabels(headers);
    mTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // rendre le tableau non editable
    mTable->setAlternatingRowColors(true);
    mTable->setShowGrid(false); // Rend le rendu encore plus moderne et épuré
    mTable->setSelectionBehavior(QAbstractItemView::SelectRows); // selection par ligne
    mTable->setSelectionMode(QAbstractItemView::SingleSelection); // Ne permettre la sélection que d'une seule ligne à la fois
    mTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch); // Étirer automatiquement la 3eme colonne sur toute la largeur disponible
    mTable->horizontalHeader()->setMinimumSectionSize(150); // Largeur minimale de chaque colonne
    mTable->setSortingEnabled(true);  // active le trie sur les headers
    mTable->verticalHeader()->setVisible(false);     // Cacher les numéros de ligne

    mainLayout->addWidget(mTable);
}
