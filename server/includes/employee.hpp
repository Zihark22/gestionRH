#ifndef EMPLOYEE_HPP
#define EMPLOYEE_HPP

#include "iniparser.hpp"

#include <iostream>
#include <map>
#include <chrono>
#include <stdexcept> // For std::invalid_argument.
#include <sstream>
#include <iomanip> // For setprecision.



using namespace std::chrono;

class Employee {
public:
// Constructors.

    Employee() = default;

    /** @brief Create an Employee from a JSON string, such as a REST API response.
     *  @param json JSON representation of the employee.
     */
    Employee(const std::string &json);


// Operators.

    // Comparison operators (using accessors).
    bool operator==(const Employee &other);
    bool operator!=(const Employee &other);

    // Stream operators.
    friend std::ostream &operator<<(std::ostream &flux, Employee const& e);

// Getters and setters.

    int id() const { return mId; }
    void setId(int id) { mId = id; }

    std::string firstname() const { return mFirstname; }
    void setFirstname(const std::string &prenom) { mFirstname = prenom; }

    std::string lastname() const { return mLastname; }
    void setLastname(const std::string &nom) { mLastname = nom; }

    std::string job() const { return mJob; }
    void setJob(const std::string &poste) { mJob = poste; }

    bool isExecutive() const { return mExecutiveStatus; }
    void setExecutiveStatus(bool cadre) { mExecutiveStatus = cadre; }

    float position() const { return mPosition; }
    void setPosition(const float &pos) { mPosition = pos; }

    uint coefficient() const { return mCoefficient; }
    void setCoefficient(uint coeff) { mCoefficient = coeff; }

    std::string startDate() const { return mStartDate; }
    void setStartDate(const std::string &date) { mStartDate = date; }

    std::string birthdate() const { return mBirthdate; }
    void setBirthdate(const std::string &date) { mBirthdate = date; }

    uint managerId() const { return mManagerId; }
    void setManagerId(uint id) { mManagerId = id; }

    std::string prevPlan() const { return mPrevPlan; }
    void setPrevPlan(std::string plan) { mPrevPlan = plan; }

    bool signedPlan() const { return mSignedPlan; }
    void setSignedPlan(bool signe) { mSignedPlan = signe; }

// ---------------------------------------------------

    /** @brief Serialize the Employee as JSON for the REST API.
     *  @return JSON representation of the employee.
     */
    std::string toJson() const;

// ---------------------------------------------------

    /** @brief Create an Employee from a SQL result row.
     *  @param sql_row Map of column names to values from a SQL result row.
     *  @return Employee initialized from the SQL row.
     */
    static Employee fromSql(const std::map<std::string, std::string> &sql_row);

// ---------------------------------------------------

    /** @brief Print employee information to standard output. */
    void display(void) const;

private:
    uint mId{0};                                //< Unique employee ID.
    std::string mFirstname{""};             //< Employee first name.
    std::string mLastname{""};              //< Employee last name.
    std::string mBirthdate{"2000-01-01"};   //< Employee birth date.
    std::string mJob{""};                   //< Employee position.
    bool mExecutiveStatus{false};            //< Whether the employee is an executive.
    float mPosition{0.0};                    //< Syntec position.
    uint mCoefficient{0};                    //< Syntec coefficient.
    std::string mStartDate{"2010-01-01"};   //< Employment start date.
    uint mManagerId{0};                      //< Manager ID.
    std::string mPrevPlan{"Plan A"};        //< Previous plan.
    bool mSignedPlan{false};                 //< Whether the plan is signed.

    std::string cleanJsonString(const std::string &jsonStr);
    void initAttributesFromJsonString(const std::string &obj);
};

#endif // EMPLOYEE_HPP
