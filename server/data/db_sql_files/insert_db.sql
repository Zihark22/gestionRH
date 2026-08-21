-- 1. Insertion du Directeur / Manager principal (Pas de manager_id)
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Marc', 'Dupont', '12/05/1995', 'Directeur Technique', 1, 
    '3.2', 210, '10/01/2015', 
    NULL, 'Plan A', 1
);

-- 2. Insertion d'un développeur sous la responsabilité du manager (manager_id = 1)
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Alice', 'Martin', '23/08/1995', 'Développeuse C++', 1, 
    '2.1', 115, '01/09/2001', 
    1, 'Plan B', 1
);

-- 3. Insertion d'un technicien sous la responsabilité du manager (manager_id = 1)
INSERT INTO employees (
    firstname, lastname, birthdate, job, executive_status, 
    position, coefficient, start_date, 
    manager_id, prev_plan, signed_plan
) VALUES (
    'Thomas', 'Bernard', '14/02/2000', 'Technicien Support', 0, 
    '1.2', 100, '15/03/2023', 
    2, 'Plan C', 0
);