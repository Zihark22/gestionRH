-- Activer le support des clés étrangères dans SQLite (impératif)
PRAGMA foreign_keys = ON;

-- Suppression de la table si elle existe déjà (utile pour les tests)
DROP TABLE IF EXISTS employees;

-- Création de la table employees avec les colonnes et contraintes spécifiées
CREATE TABLE employees (
    id                  INTEGER PRIMARY KEY AUTOINCREMENT,
    firstname           TEXT NOT NULL,
    lastname            TEXT NOT NULL,
    birthdate		    TEXT NOT NULL, -- Format recommandé : YYYY-MM-DD
    job             	TEXT NOT NULL,
    executive_status    INTEGER NOT NULL CHECK (executive_status IN (0, 1)), -- 1 = Cadre, 0 = Non cadre
    position     		TEXT NOT NULL, -- Ex: '1.1', '2.1', '3.1'
    coefficient  		INTEGER NOT NULL, -- Ex: 95, 115, 170...
    start_date       	TEXT NOT NULL, -- Format recommandé : YYYY-MM-DD
    
    -- Prévention et Management
    manager_id          INTEGER DEFAULT NULL,
    prev_plan 			TEXT CHECK (prev_plan IN ('Plan A', 'Plan B', 'Plan C', NULL)),
    signed_plan         INTEGER NOT NULL DEFAULT 0 CHECK (signed_plan IN (0, 1)), -- 1 = Oui, 0 = Non

    -- Clé étrangère réflexive (Auto-référence pour le manager)
    FOREIGN KEY (manager_id) REFERENCES employees(id) ON DELETE SET NULL
);