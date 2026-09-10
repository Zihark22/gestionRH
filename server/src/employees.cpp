#include "../includes/employee.hpp"

Employee::Employee(const std::string &jsonStr) {

    // Traitment de la chaîne JSON pour extraire les informations de l'employé
    std::string s = IniParser::trim(jsonStr);

    // Format attendu : [{"firstname":"Marc","lastname":"Dumort"}]
    if (s.size() < 2 || s.front() != '[' || s.back() != ']') {
        return;
    }

    std::string inner = IniParser::trim(s.substr(1, s.size() - 2));

    if (inner.empty()) {
        return;
    }

    if (inner.front() != '{' || inner.back() != '}') {
        return;
    }

    std::string obj = IniParser::trim(inner.substr(1, inner.size() - 2));

    // Extraction des attributs
    std::string firstname = IniParser::getField(obj, "firstname");
    std::string lastname  = IniParser::getField(obj, "lastname");
    std::string birthdate = IniParser::getField(obj, "birthdate");
    std::string job       = IniParser::getField(obj, "job");
    std::string prevPlan  = IniParser::getField(obj, "prev_plan");

    std::string executiveStatus = IniParser::getField(obj, "executive_status");
    std::string signedPlan      = IniParser::getField(obj, "signed_plan");

    std::string positionStr = IniParser::getField(obj, "position");
    std::string coefficientStr = IniParser::getField(obj, "coefficient");
    std::string managerIdStr = IniParser::getField(obj, "manager_id");
    std::string startDate = IniParser::getField(obj, "start_date");
    std::string idStr = IniParser::getField(obj, "id");

    if (!firstname.empty()) m_firstname = firstname;
    if (!lastname.empty())  m_lastname = lastname;
    if (!birthdate.empty()) m_birthdate = Date(birthdate);
    if (!job.empty())      m_job = job;
    if (!prevPlan.empty()) m_prev_plan = prevPlan;

    m_executive_status = (executiveStatus == "true" || executiveStatus == "1");
    m_signed_plan = (signedPlan == "true" || signedPlan == "1");

    if (!positionStr.empty())     m_position = stof(positionStr);
    if (!coefficientStr.empty())  m_coefficient = stoi(coefficientStr);
    if (!managerIdStr.empty())    m_manager_id = stoi(managerIdStr);
    if (!startDate.empty())       m_start_date = Date(startDate);
    if (!idStr.empty())           m_id = stoi(idStr); else m_id = -1; // laisse la base de donnée mettre l'id

} 

std::string Employee::to_JSON() const {
    std::string json = "{";
    json += "\"id\":" + std::to_string(m_id) + ",";
    json += "\"firstname\":\"" + m_firstname + "\",";
    json += "\"lastname\":\"" + m_lastname + "\",";
    json += "\"birthdate\":\"" + m_birthdate.toString() + "\",";
    json += "\"job\":\"" + m_job + "\",";
    json += "\"executive_status\":" + std::string(m_executive_status ? "true" : "false") + ",";
    json += "\"position\":" + std::to_string(m_position) + ",";
    json += "\"coefficient\":" + std::to_string(m_coefficient) + ",";
    json += "\"start_date\":\"" + m_start_date.toString() + "\",";
    json += "\"manager_id\":" + std::to_string(m_manager_id) + ",";
    json += "\"prev_plan\":\"" + m_prev_plan + "\",";
    json += "\"signed_plan\":" + std::string(m_signed_plan ? "true" : "false");
    json += "}";
    return json;
}

void Employee::display(void) const {
    std::cout << "Employee: " << std::endl;
    std::cout << "\tID: " << m_id << std::endl;
    std::cout << "\tFirstname: " << m_firstname << std::endl;
    std::cout << "\tLastname: " << m_lastname << std::endl;
    std::cout << "\tBirthdate: " << m_birthdate.toString() << std::endl;
    std::cout << "\tPoste: " << m_job << std::endl;
    std::cout << "\tIs Cadre: " << (m_executive_status ? "Yes" : "No") << std::endl;
    std::cout << "\tPosition Syntec: " << m_position << std::endl;
    std::cout << "\tCoefficient: " << m_coefficient << std::endl;
    std::cout << "\tStart Date: " << m_start_date.toString() << std::endl;
    std::cout << "\tManager ID: " << m_manager_id << std::endl;
    std::cout << "\tPrev Plan: " << m_prev_plan << std::endl;
    std::cout << "\tSigned Plan: " << (m_signed_plan ? "Yes" : "No") << std::endl;
}

Employee Employee::from_sql(const std::map<std::string, std::string> &sql_row) {

    // Fonction lambda pour convertir une chaîne en entier avec une valeur par défaut
    auto to_int_or_default = [&](const std::string& key, int defaultValue) -> int {
        const auto it = sql_row.find(key);
        if (it == sql_row.end()) {
            return defaultValue;
        }

        const std::string& value = it->second;
        if (value.empty() || value == "NULL" || value == "null") {
            return defaultValue;
        }

        try {
            return stoi(value);
        } catch (const std::exception&) {
            return defaultValue;
        }
    };

    Employee emp;
    emp.set_id(to_int_or_default("id", -1));
    emp.set_firstname(sql_row.at("firstname").empty() ? "" : sql_row.at("firstname"));
    emp.set_lastname(sql_row.at("lastname").empty() ? "" : sql_row.at("lastname"));
    emp.set_birthdate(Date(sql_row.at("birthdate").empty() ? "" : sql_row.at("birthdate")));
    emp.set_job(sql_row.at("job").empty() ? "" : sql_row.at("job"));
    emp.set_executive_status(sql_row.at("executive_status").empty() ? false : sql_row.at("executive_status") == "1");
    emp.set_position(sql_row.at("position").empty() ? 0.0f : stof(sql_row.at("position")));
    emp.set_coefficient(to_int_or_default("coefficient", 0));
    emp.set_start_date(Date(sql_row.at("start_date").empty() ? "" : sql_row.at("start_date")));
    emp.set_manager_id(to_int_or_default("manager_id", -1));
    emp.set_prev_plan(sql_row.at("prev_plan").empty() ? "" : sql_row.at("prev_plan"));
    emp.set_signed_plan(sql_row.at("signed_plan").empty() ? false : sql_row.at("signed_plan") == "1");
    return emp;
}