#include "tablewidget.hpp"

#include <QTableWidget>
#include <QVBoxLayout>
#include <QHeaderView>


TableWidget::TableWidget(const QStringList &headers, QWidget *parent)
    : QWidget{parent}
{
    auto *mainLayout = new QVBoxLayout(this);

    m_table = new QTableWidget(this);

    // Options
    m_table->clearContents();
    m_table->setRowCount(0); // Réinitialise les lignes
    m_table->setColumnCount(headers.size());
    m_table->setHorizontalHeaderLabels(headers);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers); // rendre le tableau non editable
    m_table->setAlternatingRowColors(true);
    m_table->setShowGrid(false); // Rend le rendu encore plus moderne et épuré
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows); // selection par ligne
    m_table->setSelectionMode(QAbstractItemView::SingleSelection); // Ne permettre la sélection que d'une seule ligne à la fois
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Étirer automatiquement les colonnes sur toute la largeur disponible
    m_table->setSortingEnabled(true);  // active le trie sur les headers
    m_table->verticalHeader()->setVisible(false);     // Cacher les numéros de ligne

    mainLayout->addWidget(m_table);
}






