#include "employee.hpp"

Employee::Employee() {
    this->m_birthdate = Date("01/01/2000");
    this->m_start_date = Date("01/01/2000");
    this->m_coefficient = 100;
    this->m_position = 2.1;
    this->m_firstname = "";
    this->m_lastname = "";
    this->m_executive_status = 0;
    this->m_signed_plan = 0;
    this->m_manager_id = -1;
    this->m_id = -1;
    this->m_prev_plan = "";
    this->m_job = "";
}

Employee::Employee(const string &jsonStr) {
    // Créer un Employee à partir du body sous forme JSON lors d'une demande d'ajout à la DB : [{"firstname":"Marc","lastname":"Dumort"}]

    string s = IniParser::trim(jsonStr);

    // Format attendu : [{"firstname":"Marc","lastname":"Dumort"}]
    if (s.size() < 2 || s.front() != '[' || s.back() != ']') {
        return;
    }

    string inner = IniParser::trim(s.substr(1, s.size() - 2));

    if (inner.empty()) {
        return;
    }

    if (inner.front() != '{' || inner.back() != '}') {
        return;
    }

    string obj = IniParser::trim(inner.substr(1, inner.size() - 2));

    // Extraction des attributs
    string firstname = IniParser::getField(obj, "firstname");
    string lastname  = IniParser::getField(obj, "lastname");
    string birthdate = IniParser::getField(obj, "birthdate");
    string job       = IniParser::getField(obj, "job");
    string prevPlan  = IniParser::getField(obj, "prev_plan");

    string executiveStatus = IniParser::getField(obj, "executive_status");
    string signedPlan      = IniParser::getField(obj, "signed_plan");

    string positionStr = IniParser::getField(obj, "position");
    string coefficientStr = IniParser::getField(obj, "coefficient");
    string managerIdStr = IniParser::getField(obj, "manager_id");
    string startDate = IniParser::getField(obj, "start_date");
    string idStr = IniParser::getField(obj, "id");

    if (!firstname.empty()) m_firstname = firstname;
    if (!lastname.empty())  m_lastname = lastname;
    if (!birthdate.empty()) m_birthdate = Date(birthdate);
    if (!job.empty())      m_job = job;
    if (!prevPlan.empty()) m_prev_plan = prevPlan;

    m_executive_status = (executiveStatus == "true" || executiveStatus == "1");
    m_signed_plan = (signedPlan == "true" || signedPlan == "1");

    if (!positionStr.empty())     m_position = QString::fromStdString(positionStr).toFloat();
    if (!coefficientStr.empty())  m_coefficient = QString::fromStdString(coefficientStr).toInt();
    if (!managerIdStr.empty())    m_manager_id = QString::fromStdString(managerIdStr).toInt();
    if (!startDate.empty())       m_start_date = Date(startDate);
    if (!idStr.empty())           m_id = QString::fromStdString(idStr).toInt(); else m_id = -1; // laisse la base de donnée mettre l'id
}

string Employee::to_JSON() const {
    // Implémentez la sérialisation JSON ici
    // Vous pouvez utiliser une bibliothèque JSON comme nlohmann/json pour faciliter cette tâche
    string json = "{";
    json += "\"id\":" + to_string(m_id) + ",";
    json += "\"firstname\":\"" + m_firstname + "\",";
    json += "\"lastname\":\"" + m_lastname + "\",";
    json += "\"birthdate\":\"" + m_birthdate.toString() + "\",";
    json += "\"job\":\"" + m_job + "\",";
    json += "\"executive_status\":" + string(m_executive_status ? "true" : "false") + ",";
    json += "\"position\":" + QString::number(m_position, 'f', 2).toStdString() + ",";
    json += "\"coefficient\":" + to_string(m_coefficient) + ",";
    json += "\"start_date\":\"" + m_start_date.toString() + "\",";
    json += "\"manager_id\":" + to_string(m_manager_id) + ",";
    json += "\"prev_plan\":\"" + m_prev_plan + "\",";
    json += "\"signed_plan\":" + string(m_signed_plan ? "true" : "false");
    json += "}";
    return json;
}

void Employee::display(void) const {
    qDebug() << "Employee: " ;
    qDebug() << "\tID: " << m_id ;
    qDebug() << "\tFirstname: " << m_firstname ;
    qDebug() << "\tLastname: " << m_lastname ;
    qDebug() << "\tBirthdate: " << m_birthdate.toString() ;
    qDebug() << "\tPoste: " << m_job ;
    qDebug() << "\tIs Cadre: " << (m_executive_status ? "Yes" : "No") ;
    qDebug() << "\tPosition Syntec: " << QString::number(m_position, 'f', 2).toStdString() ;
    qDebug() << "\tCoefficient: " << m_coefficient ;
    qDebug() << "\tStart Date: " << m_start_date.toString() ;
    qDebug() << "\tManager ID: " << m_manager_id ;
    qDebug() << "\tPrev Plan: " << m_prev_plan ;
    qDebug() << "\tSigned Plan: " << (m_signed_plan ? "Yes" : "No") ;
}

