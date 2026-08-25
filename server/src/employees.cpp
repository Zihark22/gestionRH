#include "../includes/employee.hpp"
#include "../includes/date.hpp"


using namespace std;

// Employee::Employee() {}

string Employee::toJSON() const {
    // Implémentez la sérialisation JSON ici
    // Vous pouvez utiliser une bibliothèque JSON comme nlohmann/json pour faciliter cette tâche
    string json = "{";
    json += "\"id\":" + to_string(m_id) + ",";
    json += "\"firstname\":\"" + m_firstname + "\",";
    json += "\"lastname\":\"" + m_lastname + "\",";
    json += "\"birthdate\":\"" + m_birthdate.toString() + "\",";
    json += "\"job\":\"" + m_job + "\",";
    json += "\"executive_status\":" + string(m_executive_status ? "true" : "false") + ",";
    json += "\"position\":" + to_string(m_position) + ",";
    json += "\"coefficient\":" + to_string(m_coefficient) + ",";
    json += "\"start_date\":\"" + m_start_date.toString() + "\",";
    json += "\"manager_id\":" + to_string(m_manager_id) + ",";
    json += "\"prev_plan\":\"" + m_prev_plan + "\",";
    json += "\"signed_plan\":" + string(m_signed_plan ? "true" : "false");
    json += "}";
    return json;
}

void Employee::display(void) const {
    cout << "Employee: " << endl;
    cout << "\tID: " << m_id << endl;
    cout << "\tFirstname: " << m_firstname << endl;
    cout << "\tLastname: " << m_lastname << endl;
    cout << "\tBirthdate: " << m_birthdate.toString() << endl;
    cout << "\tPoste: " << m_job << endl;
    cout << "\tIs Cadre: " << (m_executive_status ? "Yes" : "No") << endl;
    cout << "\tPosition Syntec: " << m_position << endl;
    cout << "\tCoefficient: " << m_coefficient << endl;
    cout << "\tStart Date: " << m_start_date.toString() << endl;
    cout << "\tManager ID: " << m_manager_id << endl;
    cout << "\tPrev Plan: " << m_prev_plan << endl;
    cout << "\tSigned Plan: " << (m_signed_plan ? "Yes" : "No") << endl;
}

Employee Employee::from_sql(const map<string, string> &json) {
    // Implémentez la désérialisation JSON ici
    auto to_int_or_default = [&](const string& key, int defaultValue) -> int {
        const auto it = json.find(key);
        if (it == json.end()) {
            return defaultValue;
        }

        const string& value = it->second;
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
    emp.set_firstname(json.at("firstname").empty() ? "" : json.at("firstname"));
    emp.set_lastname(json.at("lastname").empty() ? "" : json.at("lastname"));
    emp.set_birthdate(Date(json.at("birthdate").empty() ? "" : json.at("birthdate")));
    emp.set_job(json.at("job").empty() ? "" : json.at("job"));
    emp.set_executive_status(json.at("executive_status").empty() ? false : json.at("executive_status") == "1");
    emp.set_position(json.at("position").empty() ? 0.0f : stof(json.at("position")));
    emp.set_coefficient(to_int_or_default("coefficient", 0));
    emp.set_start_date(Date(json.at("start_date").empty() ? "" : json.at("start_date")));
    emp.set_manager_id(to_int_or_default("manager_id", -1));
    emp.set_prev_plan(json.at("prev_plan").empty() ? "" : json.at("prev_plan"));
    emp.set_signed_plan(json.at("signed_plan").empty() ? false : json.at("signed_plan") == "1");
    return emp;
}
