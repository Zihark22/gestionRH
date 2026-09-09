		
		-- Activer le support des clés étrangères dans SQLite (impératif)
		PRAGMA foreign_keys = ON;

		-- Suppression de la table si elle existe déjà (utile pour les tests)
		DROP TABLE IF EXISTS employees;

		-- Création de la table employees
		CREATE TABLE employees (
			id                  INTEGER PRIMARY KEY AUTOINCREMENT, -- id donné par la DB en autoincrémentant un compteur à chaque ajout
			firstname           TEXT NOT NULL, -- Ex : 'Bob'
			lastname            TEXT NOT NULL, -- Ex: 'Martin'
			birthdate		    TEXT NOT NULL, -- Format: DD/MM/YYYY
			job             	TEXT NOT NULL, -- Ex: 'Consultant'
			executive_status    INTEGER NOT NULL CHECK (executive_status IN (0, 1)), -- 1 = Cadre, 0 = Non cadre
			position     		REAL NOT NULL, -- Ex: 1.1
			coefficient  		INTEGER NOT NULL, -- Ex: 95
			start_date       	TEXT NOT NULL, -- Format: DD/MM/YYYY
			
			-- Prévention et Management
			manager_id          INTEGER DEFAULT NULL, -- ref à id
			prev_plan 			TEXT CHECK (prev_plan IN ('Plan A', 'Plan B', 'Plan C', NULL)), -- Choix limité
			signed_plan         INTEGER NOT NULL DEFAULT 0 CHECK (signed_plan IN (0, 1)), -- 1 = Oui, 0 = Non

			-- Clé étrangère réflexive (Auto-référence pour le manager)
			FOREIGN KEY (manager_id) REFERENCES employees(id) ON DELETE SET NULL -- le lien entre manager_id et id
		);
		
		
		