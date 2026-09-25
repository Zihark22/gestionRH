#ifndef EMPLOYEE_HPP
#define EMPLOYEE_HPP

#include <QString>
#include <QDate>

using namespace std;

/// Store employee data
class Employee {
public:
    /// Create a default employee instance
    Employee() = default;

    /// Create an employee instance from a JSON payload
    Employee(const std::string &json);

    /// Getters and setters

    uint id() const { return mId; }
    void setId(uint id) { mId = id; }

    QString firstname() const { return mFirstname; }
    void setFirstname(const QString &prenom) { mFirstname = prenom; }

    QString lastname() const { return mLastname; }
    void setLastname(const QString &nom) { mLastname = nom; }

    QDate birthdate() const { return mBirthdate; }
    void setBirthdate(const QDate &date) { mBirthdate = date; }

    QString job() const { return mJob; }
    void setJob(const QString &poste) { mJob = poste; }

    bool isExecutive() const { return mExecutiveStatus; }
    void setExecutiveStatus(bool cadre) { mExecutiveStatus = cadre; }

    float position() const { return mPosition; }
    void setPosition(const float &pos) { mPosition = pos; }

    uint coefficient() const { return mCoefficient; }
    void setCoefficient(uint coeff) { mCoefficient = coeff; }

    QDate startDate() const { return mStartDate; }
    void setStartDate(const QDate &date) { mStartDate = date; }

    uint managerId() const { return mManagerId; }
    void setManagerId(uint id) { mManagerId = id; }

    QString prevPlan() const { return mPrevPlan; }
    void setPrevPlan(QString plan) { mPrevPlan = plan; }

    bool signedPlan() const { return mSignedPlan; }
    void setSignedPlan(bool signe) { mSignedPlan = signe; }


    /// Convert the employee to a JSON document
    std::string toJson() const;

    /// Create an employee from a database result row
    static Employee fromSql(const std::map<std::string, std::string> &sql_row);

    /// Print employee data to the console
    void display(void) const;

    /// Remove leading and trailing whitespace from a string
    static string trim(const string& str);

    /// Extract a JSON field value by key
    static QString getField(const std::string &obj, const std::string &key);


private:
    uint mId{0}; ///< Unique employee identifier
    QString mFirstname{""}; ///< Employee first name
    QString mLastname{""}; ///< Employee last name
    QDate mBirthdate{QDate::fromString("01/01/2000", "yyyy-MM-dd")}; ///< Employee birth date
    QString mJob{""}; ///< Job title
    bool mExecutiveStatus{0}; ///< Executive status flag
    float mPosition{0.0}; ///< Syntec position value
    uint mCoefficient{0}; ///< Syntec coefficient
    QDate mStartDate{QDate::fromString("01/01/2010", "yyyy-MM-dd")}; ///< Employment start date
    uint mManagerId{0}; ///< Manager identifier
    QString mPrevPlan{"Plan A"}; ///< Previous prevention plan
    bool mSignedPlan{0}; ///< Signed plan flag

    std::string cleanJsonString(const std::string &jsonStr);
    void initAttributesFromJsonString(const std::string &obj);
};
#endif // EMPLOYEE_HPP
