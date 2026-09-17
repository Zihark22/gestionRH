-- 1. Insertion du Directeur / Manager principal (Pas de manager_id)
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Marc', 'Dupont', '1995-05-12', 'Directeur Technique', 1, 
    3.2, 210, '2015-01-10', 
    NULL, 'Plan A', 1
);

-- 2. Insertion d'un développeur sous la responsabilité du manager (manager_id = 1)
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Alice', 'Martin', '1995-08-23', 'Développeuse C++', 1, 
    2.1, 115, '2001-09-01', 
    1, 'Plan B', 1
);

-- 3. Insertion d'un technicien sous la responsabilité du développeur (manager_id = 2)
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Thomas', 'Bernard', '2000-02-14', 'Technicien Support', 0, 
    1.2, 100, '2023-03-15', 
    2, 'Plan C', 0
);

-- 4. Tech Lead C++
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Julien', 'Petit', '1988-11-04', 'Tech Lead C++', 1, 
    3.1, 170, '2016-02-01', 
    1, 'Plan A', 1
);

-- 5. Ingénieure Firmware
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Sophie', 'Dubois', '1993-07-19', 'Ingénieure Firmware', 1, 
    2.2, 130, '2018-04-15', 
    4, 'Plan B', 1
);

-- 6. Développeur C++ Linux Embarqué
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Lucas', 'Moreau', '1996-03-28', 'Développeur C++ Linux Embarqué', 1, 
    2.1, 115, '2020-09-01', 
    4, 'Plan B', 0
);

-- 7. Ingénieure QA / Test Logiciel
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Emma', 'Laurent', '1994-12-15', 'Ingénieure QA Logiciel', 1, 
    2.1, 115, '2019-01-10', 
    4, 'Plan C', 1
);

-- 8. Architecte Logiciel Embarqué
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Maxime', 'Simon', '1985-09-03', 'Architecte Logiciel Embarqué', 1, 
    3.2, 210, '2014-06-01', 
    1, 'Plan A', 1
);

-- 9. Développeuse DSP & Algorithmes
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Camille', 'Michel', '1991-01-22', 'Développeuse DSP & Traitement du Signal', 1, 
    2.3, 150, '2017-11-15', 
    8, 'Plan B', 1
);

-- 10. Ingénieur Systèmes Embarqués
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Alexandre', 'Lefebvre', '1992-06-09', 'Ingénieur Systèmes Embarqués', 1, 
    2.2, 130, '2018-03-01', 
    8, 'Plan A', 0
);

-- 11. Scrum Master / PMO
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Léa', 'Leroy', '1989-04-18', 'Scrum Master', 1, 
    2.2, 130, '2019-10-01', 
    1, 'Plan C', 1
);

-- 12. DevOps & CI/CD Embarqué
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Nicolas', 'Roux', '1993-08-05', 'Ingénieur DevOps & CI/CD', 1, 
    2.2, 130, '2021-02-15', 
    4, 'Plan B', 1
);

-- 13. Ingénieure Validation Banc de Test
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Chloé', 'David', '1995-10-30', 'Ingénieure Bancs de Test HIL', 1, 
    2.1, 115, '2021-09-01', 
    8, 'Plan C', 1
);

-- 14. Technicien Banc de Test
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Antoine', 'Bertrand', '1998-02-11', 'Technicien Banc de Test', 0, 
    1.2, 100, '2022-04-01', 
    13, 'Plan C', 0
);

-- 15. Développeuse C++ Junior
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Manon', 'Morel', '2001-05-14', 'Développeuse C++ Junior', 1, 
    1.2, 100, '2023-09-01', 
    4, 'Plan A', 1
);

-- 16. Ingénieur Drivers & BSP
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Hugo', 'Fourcade', '1987-08-25', 'Ingénieur Kernel & Drivers BSP', 1, 
    2.3, 150, '2016-05-15', 
    8, 'Plan B', 1
);

-- 17. Responsable Cybersécurité Embarquée
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Clara', 'Girard', '1986-11-12', 'Lead Cybersécurité Embarquée', 1, 
    3.1, 170, '2017-11-01', 
    1, 'Plan A', 1
);

-- 18. Ingénieur Sécurité Logicielle
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Gabriel', 'Bonnet', '1994-03-07', 'Ingénieur Sécurité Logicielle', 1, 
    2.2, 130, '2020-06-01', 
    17, 'Plan B', 0
);

-- 19. Administrateur Systèmes & Réseaux
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Sarah', 'Francois', '1997-09-29', 'Administratrice Systèmes & Réseau', 0, 
    1.2, 105, '2022-01-15', 
    12, 'Plan C', 1
);

-- 20. Développeur Microcontrôleurs Bare-Metal
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Florian', 'Martinez', '1995-12-16', 'Développeur Bare-Metal C/C++', 1, 
    2.1, 115, '2021-10-01', 
    4, 'Plan B', 1
);

-- 21. Ingénieure FPGA / VHDL
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Juliette', 'Legrand', '1990-07-02', 'Ingénieure Conception FPGA', 1, 
    2.3, 150, '2017-04-01', 
    8, 'Plan A', 1
);

-- 22. Technicien Électronique & Prototypage
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Romain', 'Sanchez', '1999-10-20', 'Technicien Prototypage Électronique', 0, 
    1.1, 95, '2023-09-15', 
    13, 'Plan C', 0
);

-- 23. Développeuse IHM Qt/QML (C++)
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Mathilde', 'Garnier', '1993-04-11', 'Développeuse IHM Qt/C++', 1, 
    2.2, 130, '2019-02-01', 
    4, 'Plan B', 1
);

-- 24. Responsable Produit / Product Owner
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Vincent', 'Chevalier', '1984-01-08', 'Product Owner Systèmes Embarqués', 1, 
    3.1, 170, '2015-03-15', 
    1, 'Plan A', 1
);

-- 25. Ingénieure Recherche & Innovation
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Inès', 'Faure', '1992-06-19', 'Ingénieure R&D Algorithmes', 1, 
    2.3, 150, '2018-09-01', 
    1, 'Plan A', 0
);

-- 26. Développeur C++ / Rust
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Bastien', 'Andre', '1997-05-27', 'Développeur C++ / Rust Embarqué', 1, 
    2.1, 115, '2022-05-01', 
    4, 'Plan B', 1
);

-- 27. Responsable Qualité & Normes (ISO 26262)
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Charlotte', 'Mercier', '1983-08-14', 'Responsable Qualité Logicielle Sûreté', 1, 
    3.1, 170, '2016-01-01', 
    1, 'Plan A', 1
);

-- 28. Ingénieur Intégration Continue
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Paul', 'Guerin', '1996-03-03', 'Ingénieur Intégration Continue & Release', 1, 
    2.1, 115, '2021-10-15', 
    12, 'Plan C', 0
);

-- 29. Chargée de Recrutement Technique
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Laura', 'Boyer', '1994-11-21', 'Chargée de Recrutement Tech', 0, 
    1.2, 105, '2020-06-01', 
    1, 'Plan B', 1
);

-- 30. Technicien Support Embarqué N2
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Adrien', 'Blanc', '1997-09-17', 'Technicien Support Embarqué N2', 0, 
    1.2, 100, '2022-11-01', 
    3, 'Plan C', 1
);

-- 31. Ingénieure RTOS (FreeRTOS / Zephyr)
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Pauline', 'Renaud', '1993-02-24', 'Ingénieure Spécialiste RTOS', 1, 
    2.2, 130, '2019-04-01', 
    8, 'Plan B', 1
);

-- 32. Développeur C++ Stagiaire / Alternant
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Nathan', 'Gauthier', '2002-07-31', 'Alternant Développeur C++', 0, 
    1.1, 85, '2024-09-01', 
    4, 'Plan C', 0
);

-- 33. Office Manager & Assistante de Direction
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Lucie', 'Perrin', '1990-05-10', 'Office Manager', 0, 
    1.2, 100, '2017-02-15', 
    1, 'Plan A', 1
);