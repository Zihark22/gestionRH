-- 1. Insertion du Directeur / Manager principal (Pas de manager_id)
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Marc', 'Dupont', '12/05/1995', 'Directeur Technique', 1, 
    3.2, 210, '10/01/2015', 
    NULL, 'Plan A', 1
);

-- 2. Insertion d'un développeur sous la responsabilité du manager (manager_id = 1)
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Alice', 'Martin', '23/08/1995', 'Développeuse C++', 1, 
    2.1, 115, '01/09/2001', 
    1, 'Plan B', 1
);

-- 3. Insertion d'un technicien sous la responsabilité du développeur (manager_id = 2)
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Thomas', 'Bernard', '14/02/2000', 'Technicien Support', 0, 
    1.2, 100, '15/03/2023', 
    2, 'Plan C', 0
);

-- 4. Tech Lead C++
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Julien', 'Petit', '04/11/1988', 'Tech Lead C++', 1, 
    3.1, 170, '01/02/2016', 
    1, 'Plan A', 1
);

-- 5. Ingénieure Firmware
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Sophie', 'Dubois', '19/07/1993', 'Ingénieure Firmware', 1, 
    2.2, 130, '15/04/2018', 
    4, 'Plan B', 1
);

-- 6. Développeur C++ Linux Embarqué
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Lucas', 'Moreau', '28/03/1996', 'Développeur C++ Linux Embarqué', 1, 
    2.1, 115, '01/09/2020', 
    4, 'Plan B', 0
);

-- 7. Ingénieure QA / Test Logiciel
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Emma', 'Laurent', '15/12/1994', 'Ingénieure QA Logiciel', 1, 
    2.1, 115, '10/01/2019', 
    4, 'Plan C', 1
);

-- 8. Architecte Logiciel Embarqué
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Maxime', 'Simon', '03/09/1985', 'Architecte Logiciel Embarqué', 1, 
    3.2, 210, '01/06/2014', 
    1, 'Plan A', 1
);

-- 9. Développeuse DSP & Algorithmes
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Camille', 'Michel', '22/01/1991', 'Développeuse DSP & Traitement du Signal', 1, 
    2.3, 150, '15/11/2017', 
    8, 'Plan B', 1
);

-- 10. Ingénieur Systèmes Embarqués
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Alexandre', 'Lefebvre', '09/06/1992', 'Ingénieur Systèmes Embarqués', 1, 
    2.2, 130, '01/03/2018', 
    8, 'Plan A', 0
);

-- 11. Scrum Master / PMO
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Léa', 'Leroy', '18/04/1989', 'Scrum Master', 1, 
    2.2, 130, '01/10/2019', 
    1, 'Plan C', 1
);

-- 12. DevOps & CI/CD Embarqué
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Nicolas', 'Roux', '05/08/1993', 'Ingénieur DevOps & CI/CD', 1, 
    2.2, 130, '15/02/2021', 
    4, 'Plan B', 1
);

-- 13. Ingénieure Validation Banc de Test
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Chloé', 'David', '30/10/1995', 'Ingénieure Bancs de Test HIL', 1, 
    2.1, 115, '01/09/2021', 
    8, 'Plan C', 1
);

-- 14. Technicien Banc de Test
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Antoine', 'Bertrand', '11/02/1998', 'Technicien Banc de Test', 0, 
    1.2, 100, '01/04/2022', 
    13, 'Plan C', 0
);

-- 15. Développeuse C++ Junior
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Manon', 'Morel', '14/05/2001', 'Développeuse C++ Junior', 1, 
    1.2, 100, '01/09/2023', 
    4, 'Plan A', 1
);

-- 16. Ingénieur Drivers & BSP
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Hugo', 'Fourcade', '25/08/1987', 'Ingénieur Kernel & Drivers BSP', 1, 
    2.3, 150, '15/05/2016', 
    8, 'Plan B', 1
);

-- 17. Responsable Cybersécurité Embarquée
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Clara', 'Girard', '12/11/1986', 'Lead Cybersécurité Embarquée', 1, 
    3.1, 170, '01/11/2017', 
    1, 'Plan A', 1
);

-- 18. Ingénieur Sécurité Logicielle
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Gabriel', 'Bonnet', '07/03/1994', 'Ingénieur Sécurité Logicielle', 1, 
    2.2, 130, '01/06/2020', 
    17, 'Plan B', 0
);

-- 19. Administrateur Systèmes & Réseaux
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Sarah', 'Francois', '29/09/1997', 'Administratrice Systèmes & Réseau', 0, 
    1.2, 105, '15/01/2022', 
    12, 'Plan C', 1
);

-- 20. Développeur Microcontrôleurs Bare-Metal
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Florian', 'Martinez', '16/12/1995', 'Développeur Bare-Metal C/C++', 1, 
    2.1, 115, '01/10/2021', 
    4, 'Plan B', 1
);

-- 21. Ingénieure FPGA / VHDL
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Juliette', 'Legrand', '02/07/1990', 'Ingénieure Conception FPGA', 1, 
    2.3, 150, '01/04/2017', 
    8, 'Plan A', 1
);

-- 22. Technicien Électronique & Prototypage
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Romain', 'Sanchez', '20/10/1999', 'Technicien Prototypage Électronique', 0, 
    1.1, 95, '15/09/2023', 
    13, 'Plan C', 0
);

-- 23. Développeuse IHM Qt/QML (C++)
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Mathilde', 'Garnier', '11/04/1993', 'Développeuse IHM Qt/C++', 1, 
    2.2, 130, '01/02/2019', 
    4, 'Plan B', 1
);

-- 24. Responsable Produit / Product Owner
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Vincent', 'Chevalier', '08/01/1984', 'Product Owner Systèmes Embarqués', 1, 
    3.1, 170, '15/03/2015', 
    1, 'Plan A', 1
);

-- 25. Ingénieure Recherche & Innovation
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Inès', 'Faure', '19/06/1992', 'Ingénieure R&D Algorithmes', 1, 
    2.3, 150, '01/09/2018', 
    1, 'Plan A', 0
);

-- 26. Développeur C++ / Rust
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Bastien', 'Andre', '27/05/1997', 'Développeur C++ / Rust Embarqué', 1, 
    2.1, 115, '01/05/2022', 
    4, 'Plan B', 1
);

-- 27. Responsable Qualité & Normes (ISO 26262)
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Charlotte', 'Mercier', '14/08/1983', 'Responsable Qualité Logicielle Sûreté', 1, 
    3.1, 170, '01/01/2016', 
    1, 'Plan A', 1
);

-- 28. Ingénieur Intégration Continue
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Paul', 'Guerin', '03/03/1996', 'Ingénieur Intégration Continue & Release', 1, 
    2.1, 115, '15/10/2021', 
    12, 'Plan C', 0
);

-- 29. Chargée de Recrutement Technique
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Laura', 'Boyer', '21/11/1994', 'Chargée de Recrutement Tech', 0, 
    1.2, 105, '01/06/2020', 
    1, 'Plan B', 1
);

-- 30. Technicien Support Embarqué N2
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Adrien', 'Blanc', '17/09/1997', 'Technicien Support Embarqué N2', 0, 
    1.2, 100, '01/11/2022', 
    3, 'Plan C', 1
);

-- 31. Ingénieure RTOS (FreeRTOS / Zephyr)
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Pauline', 'Renaud', '24/02/1993', 'Ingénieure Spécialiste RTOS', 1, 
    2.2, 130, '01/04/2019', 
    8, 'Plan B', 1
);

-- 32. Développeur C++ Stagiaire / Alternant
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Nathan', 'Gauthier', '31/07/2002', 'Alternant Développeur C++', 0, 
    1.1, 85, '01/09/2024', 
    4, 'Plan C', 0
);

-- 33. Office Manager & Assistante de Direction
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Lucie', 'Perrin', '10/05/1990', 'Office Manager', 0, 
    1.2, 100, '15/02/2017', 
    1, 'Plan A', 1
);