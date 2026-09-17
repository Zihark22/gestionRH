#include "employee.hpp"


Employee::Employee(const std::string &jsonStr) {

    std::string obj = cleanJsonString(jsonStr);

    if(obj!="")
        initAttributesFromJsonString(obj);
    else
        throw std::invalid_argument("Impossible de créer un employé car le format du JSON est invalide :" + jsonStr);
}

std::string Employee::cleanJsonString(const std::string &jsonStr) {
    // Traitment de la chaîne JSON pour extraire les informations de l'employé
    std::string s = IniParser::trim(jsonStr);

    // Format attendu : [{"firstname":"Marc","lastname":"Dumort"}]
    if (s.size() < 2 || s.front() != '[' || s.back() != ']')
        return "";

    std::string inner = IniParser::trim(s.substr(1, s.size() - 2));

    if (inner.empty())
        return "";

    if (inner.front() != '{' || inner.back() != '}')
        return "";

    return IniParser::trim(inner.substr(1, inner.size() - 2));
}

void Employee::initAttributesFromJsonString(const std::string &obj) {

    // Extraction des attributs
    QString firstname = IniParser::getField(obj, "firstname");
    QString lastname  = IniParser::getField(obj, "lastname");
    QString birthdate = IniParser::getField(obj, "birthdate");
    QString job       = IniParser::getField(obj, "job");
    QString prevPlan  = IniParser::getField(obj, "prevPlan");

    QString executiveStatus = IniParser::getField(obj, "executive_status");
    QString signedPlan      = IniParser::getField(obj, "signed_plan");

    QString positionStr = IniParser::getField(obj, "position");
    QString coefficientStr = IniParser::getField(obj, "coefficient");
    QString managerIdStr = IniParser::getField(obj, "manager_id");
    QString startDate = IniParser::getField(obj, "start_date");
    QString idStr = IniParser::getField(obj, "id");

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
