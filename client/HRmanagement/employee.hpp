#ifndef EMPLOYEE_HPP
#define EMPLOYEE_HPP

#include "date.hpp"
#include "iniparser.hpp"

#include <QString>
#include <QDebug>

using namespace std;

class Employee {
public:
    // Constructeurs

    Employee() = default;

    /** @brief Crée un objet Employee à partir d'un JSON string (ex: réponse d'une requête API REST)
     *  @param json Une chaîne JSON représentant l'employé.
     */
    Employee(const std::string &json);

    // Getters and Setters

    // Identifiant unique BDD
    int id() const { return mId; }
    void setId(int id) { mId = id; }

    std::string firstname() const { return mFirstname; }
    void setFirstname(const std::string &prenom) { mFirstname = prenom; }

    std::string lastname() const { return mLastname; }
    void setLastname(const std::string &nom) { mLastname = nom; }

    Date birthdate() const { return mBirthdate; }
    void setBirthdate(const Date &date) { mBirthdate = date; }

    std::string job() const { return mJob; }
    void setJob(const std::string &poste) { mJob = poste; }

    int isExecutive() const { return mExecutiveStatus; }
    void setExecutiveStatus(int cadre) { mExecutiveStatus = cadre; }

    float position() const { return mPosition; }
    void setPosition(const float &pos) { mPosition = pos; }

    int coefficient() const { return mCoefficient; }
    void setCoefficient(int coeff) { mCoefficient = coeff; }

    Date startDate() const { return mStartDate; }
    void setStartDate(const Date &date) { mStartDate = date; }

    int managerId() const { return mManagerId; }
    void setManagerId(int id) { mManagerId = id; }

    std::string prevPlan() const { return mPrevPlan; }
    void setPrevPlan(std::string plan) { mPrevPlan = plan; }

    int signedPlan() const { return mSignedPlan; }
    void setSignedPlan(int signe) { mSignedPlan = signe; }


    // ---------------------------------------------------

    /** @brief Sérialise l'objet Employee en JSON pour l'API REST
     *  @return Chaîne JSON représentant l'employé
     */
    std::string toJson() const;

    // ---------------------------------------------------

    /** @brief Crée un objet Employee à partir d'une ligne de résultat SQL
     *  @param sql_row Une map représentant une ligne de résultat SQL, où les clés sont les noms des colonnes et les valeurs sont les valeurs correspondantes.
     *  @return Un objet Employee initialisé avec les données de la ligne SQL
     */
    static Employee fromSql(const std::map<std::string, std::string> &sql_row);

    // ---------------------------------------------------

    /** @brief Affiche les informations de l'employé dans la sortie standard */
    void display(void) const;

private:
    int mId{-1};                        //< Identifiant unique de l'employé
    std::string mFirstname;             //< Prénom de l'employé
    std::string mLastname;              //< Nom de famille de l'employé
    Date mBirthdate;                    //< Date de naissance de l'employé
    std::string mJob;                   //< Poste de l'employé
    int mExecutiveStatus{0};           //< Statut de manager de l'employé
    float mPosition;                    //< Position de l'employé
    int mCoefficient{0};                //< Coefficient de l'employé
    Date mStartDate;                   //< Date de début d'activité
    int mManagerId{-1};                //< Identifiant du manager
    std::string mPrevPlan{"Plan A"};   //< Plan précédent
    int mSignedPlan{0};                //< Plan signé
};
#endif // EMPLOYEE_HPP
