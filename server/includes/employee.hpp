#ifndef EMPLOYEE_HPP
#define EMPLOYEE_HPP

#include "date.hpp"
#include "iniparser.hpp"


#include <iostream>
#include <map>

class Employee {

public:
// Constructeurs

    Employee() = default;

    /** @brief Crée un objet Employee à partir d'un JSON string (ex: réponse d'une requête API REST)
     *  @param json Une chaîne JSON représentant l'employé.
     */
    Employee(const std::string &json);

// Getters and Setters

    int id() const { return m_id; }
    void set_id(int id) { m_id = id; }

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

// ---------------------------------------------------

    /** @brief Sérialise l'objet Employee en JSON pour l'API REST
     *  @return Chaîne JSON représentant l'employé
     */
    std::string to_JSON() const;

// ---------------------------------------------------

    /** @brief Crée un objet Employee à partir d'une ligne de résultat SQL
     *  @param sql_row Une map représentant une ligne de résultat SQL, où les clés sont les noms des colonnes et les valeurs sont les valeurs correspondantes.
     *  @return Un objet Employee initialisé avec les données de la ligne SQL
     */
    static Employee from_sql(const std::map<std::string, std::string> &sql_row);

// ---------------------------------------------------

    /** @brief Affiche les informations de l'employé dans la sortie standard */
    void display(void) const;

private:
    int m_id{-1};                        //< Identifiant unique de l'employé
    std::string m_firstname;             //< Prénom de l'employé
    std::string m_lastname;              //< Nom de famille de l'employé
    Date m_birthdate;                    //< Date de naissance de l'employé
    std::string m_job;                   //< Poste de l'employé
    int m_executive_status{0};           //< Statut de manager de l'employé
    float m_position;                    //< Position de l'employé
    int m_coefficient{0};                //< Coefficient de l'employé
    Date m_start_date;                   //< Date de début d'activité
    int m_manager_id{-1};                //< Identifiant du manager
    std::string m_prev_plan{"Plan A"};   //< Plan précédent
    int m_signed_plan{0};                //< Plan signé
};

#endif // EMPLOYEE_HPP
