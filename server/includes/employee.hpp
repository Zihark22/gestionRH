#ifndef EMPLOYEE_HPP
#define EMPLOYEE_HPP

#include "date.hpp"

#include <iostream>
#include <map>

class Employee {
public:
    Employee() = default;
    Employee(const std::string &json);

    // Identifiant unique BDD
    int id() const { return m_id; }
    void set_id(int id) { m_id = id; }

    // Onglet 1 & 2 - Infos de base
    std::string firstname() const { return m_firstname; }
    void set_firstname(const std::string &prenom) { m_firstname = prenom; }

    std::string lastname() const { return m_lastname; }
    void set_lastname(const std::string &nom) { m_lastname = nom; }

    Date birthdate() const { return m_birthdate; }
    void set_birthdate(const Date &date) { m_birthdate = date; }

    std::string job() const { return m_job; }
    void set_job(const std::string &poste) { m_job = poste; }

    int is_executive() const { return m_executive_status; }
    void set_executive_status(int cadre) { m_executive_status = cadre; }

    float position() const { return m_position; }
    void set_position(const float &pos) { m_position = pos; }

    int coefficient() const { return m_coefficient; }
    void set_coefficient(int coeff) { m_coefficient = coeff; }

    Date start_date() const { return m_start_date; }
    void set_start_date(const Date &date) { m_start_date = date; }

    int manager_id() const { return m_manager_id; }
    void set_manager_id(int id) { m_manager_id = id; }

    std::string prev_plan() const { return m_prev_plan; }
    void set_prev_plan(std::string plan) { m_prev_plan = plan; }

    int signed_plan() const { return m_signed_plan; }
    void set_signed_plan(int signe) { m_signed_plan = signe; }

    // Méthodes de sérialisation JSON pour l'API REST
    std::string to_JSON() const;

    static Employee from_sql(const std::map<std::string, std::string> &sql_row);
    void display(void) const;

private:
    int m_id{-1};
    std::string m_firstname;
    std::string m_lastname;
    Date m_birthdate;
    std::string m_job;
    int m_executive_status{0};
    float m_position;
    int m_coefficient{0};
    Date m_start_date;
    int m_manager_id{-1};
    std::string m_prev_plan{"Plan A"};
    int m_signed_plan{0};
};

#endif // EMPLOYEE_HPP
