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

    // enleve espaces au debut et fin du json
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

    // trouver valeur avec cle
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

