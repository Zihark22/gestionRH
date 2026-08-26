#include "../includes/employee.hpp"
#include "../includes/date.hpp"


// Employee::Employee() {}

Employee::Employee(const string &jsonStr) {
    // Créer un Employee à partir du body sous forme JSON lors d'une demande d'ajout à la DB : [{"firstname":"Marc","lastname":"Dumort"}]
      
    auto trim = [](const string &s) -> string {
        size_t start = 0;
        while (start < s.size() && (s[start] == ' ' || s[start] == '\n' || s[start] == '\t' || s[start] == '\r')) {
            ++start;
        }
        size_t end = s.size();
        while (end > start && (s[end - 1] == ' ' || s[end - 1] == '\n' || s[end - 1] == '\t' || s[end - 1] == '\r')) {
            --end;
        }
        return s.substr(start, end - start);
    };

    auto getField = [&](const string &obj, const string &key) -> string {
        string pattern = "\"" + key + "\"";
        size_t pos = obj.find(pattern);
        if (pos == string::npos) {
            return "";
        }

        size_t colon = obj.find(':', pos + pattern.size());
        if (colon == string::npos) {
            return "";
        }

        size_t valueStart = obj.find_first_not_of(" \t\r\n", colon + 1);
        if (valueStart == string::npos) {
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
                if (obj[valueEnd] == '"') {
                    break;
                }
                ++valueEnd;
            }
            return obj.substr(valueStart + 1, valueEnd - valueStart - 1);
        }

        // Cas nombre / bool / null
        size_t valueEnd = valueStart;
        while (valueEnd < obj.size() && obj[valueEnd] != ',' && obj[valueEnd] != '}') {
            ++valueEnd;
        }
        return trim(obj.substr(valueStart, valueEnd - valueStart));
    };

    string s = trim(jsonStr);

    // Format attendu : [{"firstname":"Marc","lastname":"Dumort"}]
    if (s.size() < 2 || s.front() != '[' || s.back() != ']') {
        return;
    }

    string inner = trim(s.substr(1, s.size() - 2));

    if (inner.empty()) {
        return;
    }

    if (inner.front() != '{' || inner.back() != '}') {
        return;
    }

    string obj = trim(inner.substr(1, inner.size() - 2));

    // Extraction des attributs
    string firstname = getField(obj, "firstname");
    string lastname  = getField(obj, "lastname");
    string birthdate = getField(obj, "birthdate");
    string job       = getField(obj, "job");
    string prevPlan  = getField(obj, "prev_plan");

    string executiveStatus = getField(obj, "executive_status");
    string signedPlan      = getField(obj, "signed_plan");

    string positionStr = getField(obj, "position");
    string coefficientStr = getField(obj, "coefficient");
    string managerIdStr = getField(obj, "manager_id");
    string startDate = getField(obj, "start_date");
    string idStr = getField(obj, "id");

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
    
    
    m_id = -1; // laisse la base de donnée mettre l'id


} 


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

Employee Employee::from_sql(const map<string, string> &sql_row) {
    // Implémentez la désérialisation JSON ici
    auto to_int_or_default = [&](const string& key, int defaultValue) -> int {
        const auto it = sql_row.find(key);
        if (it == sql_row.end()) {
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
