		
		-- Enable foreign key support in SQLite (required).
		PRAGMA foreign_keys = ON;

		-- Drop the table if it already exists (useful for tests).
		DROP TABLE IF EXISTS employees;

		-- Create the employees table.
		CREATE TABLE employees (
			id                  INTEGER PRIMARY KEY AUTOINCREMENT, -- ID assigned by the database.
			firstname           TEXT NOT NULL, -- Example: 'Bob'.
			lastname            TEXT NOT NULL, -- Example: 'Martin'.
			birthdate		    TEXT NOT NULL, -- Format: YYYY/MM/DD
			job             	TEXT NOT NULL, -- Ex: 'Consultant'
			executive_status    INTEGER NOT NULL CHECK (executive_status IN (0, 1)), -- 1 = executive, 0 = non-executive.
			position     		REAL NOT NULL, -- Ex: 1.1
			coefficient  		INTEGER NOT NULL, -- Ex: 95
			start_date       	TEXT NOT NULL, -- Format: YYYY/MM/DD
			
			-- Management and career planning.
			manager_id          INTEGER DEFAULT NULL, -- Reference to an employee ID.
			prev_plan 			TEXT CHECK (prev_plan IN ('Plan A', 'Plan B', 'Plan C', NULL)), -- Limited selection.
			signed_plan         INTEGER NOT NULL DEFAULT 0 CHECK (signed_plan IN (0, 1)), -- 1 = yes, 0 = no.

			-- Self-referencing foreign key for the manager.
			FOREIGN KEY (manager_id) REFERENCES employees(id) ON DELETE SET NULL -- Link between manager_id and id.
		);
		
		
		