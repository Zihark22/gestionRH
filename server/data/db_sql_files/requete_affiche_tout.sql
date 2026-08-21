SELECT 
    c.id,
    c.firstname,
    c.lastname,
    c.birthdate,
    c.job,
    c.executive_status,
    c.position,
    c.coefficient,
    c.start_date,
    c.prev_plan,
    c.signed_plan,
    c.manager_id,
    COALESCE(m.firstname || ' ' || m.lastname, 'Aucun') AS nom_manager
FROM employees c
LEFT JOIN employees m ON c.manager_id = m.id;