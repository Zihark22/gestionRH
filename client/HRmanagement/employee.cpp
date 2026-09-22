#include "employee.hpp"

#include <QDebug>

Employee::Employee(const std::string &jsonStr) {

    std::string obj = cleanJsonString(jsonStr);

    if(obj!="")
        initAttributesFromJsonString(obj);
    else
        throw std::invalid_argument("Impossible de créer un employé car le format du JSON est invalide :" + jsonStr);
}

std::string Employee::cleanJsonString(const std::string &jsonStr) {
    // Traitment de la chaîne JSON pour extraire les informations de l'employé
    std::string s = Employee::trim(jsonStr);

    // Format attendu : [{"firstname":"Marc","lastname":"Dumort"}]
    if (s.size() < 2 || s.front() != '[' || s.back() != ']')
        return "";

    std::string inner = Employee::trim(s.substr(1, s.size() - 2));

    if (inner.empty())
        return "";

    if (inner.front() != '{' || inner.back() != '}')
        return "";

    return Employee::trim(inner.substr(1, inner.size() - 2));
}

void Employee::initAttributesFromJsonString(const std::string &obj) {

    // Extraction des attributs
    QString firstname = Employee::getField(obj, "firstname");
    QString lastname  = Employee::getField(obj, "lastname");
    QString birthdate = Employee::getField(obj, "birthdate");
    QString job       = Employee::getField(obj, "job");
    QString prevPlan  = Employee::getField(obj, "prevPlan");

    QString executiveStatus = Employee::getField(obj, "executive_status");
    QString signedPlan      = Employee::getField(obj, "signed_plan");

    QString positionStr = Employee::getField(obj, "position");
    QString coefficientStr = Employee::getField(obj, "coefficient");
    QString managerIdStr = Employee::getField(obj, "manager_id");
    QString startDate = Employee::getField(obj, "start_date");
    QString idStr = Employee::getField(obj, "id");

    if (!firstname.isEmpty())
        mFirstname = firstname;
    if (!lastname.isEmpty())
        mLastname = lastname;
    if (!birthdate.isEmpty())
        mBirthdate = QDate::fromString(birthdate, "yyyy-MM-dd");
    if (!job.isEmpty())
        mJob = job;
    if (!prevPlan.isEmpty())
        mPrevPlan = prevPlan;

    mExecutiveStatus = (executiveStatus == "true" || executiveStatus == "1");
    mSignedPlan = (signedPlan == "true" || signedPlan == "1");

    if (!positionStr.isEmpty())
        mPosition = positionStr.toFloat();
    if (!coefficientStr.isEmpty())
        mCoefficient = coefficientStr.toInt();
    if (!managerIdStr.isEmpty())
        mManagerId = managerIdStr.toInt();
    if (!startDate.isEmpty())
        mStartDate = QDate::fromString(startDate, "yyyy-MM-dd");
    if (!idStr.isEmpty())
        mId = idStr.toUInt(); else mId = -1; // laisse la base de donnée mettre l'id
}

std::string Employee::toJson() const {
    std::string json = "{";
    json += "\"id\":" + std::to_string(mId) + ",";
    json += "\"firstname\":\"" + mFirstname.toStdString() + "\",";
    json += "\"lastname\":\"" + mLastname.toStdString() + "\",";
    json += "\"birthdate\":\"" + mBirthdate.toString("yyyy-MM-dd").toStdString() + "\",";
    json += "\"job\":\"" + mJob.toStdString() + "\",";
    json += "\"executive_status\":" + std::string(mExecutiveStatus ? "true" : "false") + ",";
    json += "\"position\":" + QString::number(mPosition, 'f', 2).toStdString() + ",";
    json += "\"coefficient\":" + std::to_string(mCoefficient) + ",";
    json += "\"startDate\":\"" + mStartDate.toString("yyyy-MM-dd").toStdString() + "\",";
    json += "\"manager_id\":" + std::to_string(mManagerId) + ",";
    json += "\"prevPlan\":\"" + mPrevPlan.toStdString() + "\",";
    json += "\"signed_plan\":" + std::string(mSignedPlan ? "true" : "false");
    json += "}";
    return json;
}

void Employee::display(void) const {
    qDebug() << "Employee: " ;
    qDebug() << "\tID: " << mId ;
    qDebug() << "\tFirstname: " << mFirstname ;
    qDebug() << "\tLastname: " << mLastname ;
    qDebug() << "\tBirthdate: " << mBirthdate.toString() ;
    qDebug() << "\tPoste: " << mJob ;
    qDebug() << "\tIs Cadre: " << (mExecutiveStatus ? "Yes" : "No") ;
    qDebug() << "\tPosition Syntec: " << mPosition ;
    qDebug() << "\tCoefficient: " << mCoefficient ;
    qDebug() << "\tStart Date: " << mStartDate.toString() ;
    qDebug() << "\tManager ID: " << mManagerId ;
    qDebug() << "\tPrev Plan: " << mPrevPlan ;
    qDebug() << "\tSigned Plan: " << (mSignedPlan ? "Yes" : "No") ;
}


string Employee::trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

QString Employee::getField(const std::string &obj, const std::string &key) {
    std::string pattern = "\"" + key + "\"";
    size_t pos = obj.find(pattern);
    if (pos == std::string::npos)
        return "";

    size_t colon = obj.find(':', pos + pattern.size());
    if (colon == std::string::npos)
        return "";

    size_t valueStart = obj.find_first_not_of(" \t\r\n", colon + 1);
    if (valueStart == std::string::npos) {
        return "";
    }

    // Cas chaîne de caractères
    if (obj[valueStart] == '"') {
        size_t valueEnd = valueStart + 1;
        while (valueEnd < obj.size()) {
            if (obj[valueEnd] == '\\' && valueEnd + 1 < obj.size()) {
                valueEnd += 2;
                continue;
            }
            if (obj[valueEnd] == '"')
                break;

            ++valueEnd;
        }
        return QString::fromStdString(obj.substr(valueStart + 1, valueEnd - valueStart - 1));
    }

    // Cas nombre / bool / null
    size_t valueEnd = valueStart;
    while (valueEnd < obj.size() && obj[valueEnd] != ',' && obj[valueEnd] != '}')
        ++valueEnd;
    return QString::fromStdString(Employee::trim(obj.substr(valueStart, valueEnd - valueStart)));
}
