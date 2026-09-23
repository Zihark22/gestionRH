#include "../includes/employee.hpp"

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
    std::string firstname = IniParser::getField(obj, "firstname");
    std::string lastname  = IniParser::getField(obj, "lastname");
    std::string birthdate = IniParser::getField(obj, "birthdate");
    std::string job       = IniParser::getField(obj, "job");
    std::string prevPlan  = IniParser::getField(obj, "prevPlan");

    std::string executiveStatus = IniParser::getField(obj, "executive_status");
    std::string signedPlan      = IniParser::getField(obj, "signed_plan");

    std::string positionStr = IniParser::getField(obj, "position");
    std::string coefficientStr = IniParser::getField(obj, "coefficient");
    std::string managerIdStr = IniParser::getField(obj, "manager_id");
    std::string startDate = IniParser::getField(obj, "start_date");
    std::string idStr = IniParser::getField(obj, "id");

    if (!firstname.empty()) 
        mFirstname = firstname;
    if (!lastname.empty())  
        mLastname = lastname;
    if (!birthdate.empty()) 
        mBirthdate = birthdate;
    if (!job.empty()) 
        mJob = job;
    if (!prevPlan.empty()) 
        mPrevPlan = prevPlan;

    mExecutiveStatus = (executiveStatus == "true" || executiveStatus == "1");
    mSignedPlan = (signedPlan == "true" || signedPlan == "1");

    if (!positionStr.empty())     
        mPosition = stof(positionStr);
    if (!coefficientStr.empty())  
        mCoefficient = stoi(coefficientStr);
    if (!managerIdStr.empty())    
        mManagerId = stoi(managerIdStr);
    if (!startDate.empty())       
        mStartDate = startDate;

    if (!idStr.empty())           
        mId = stoi(idStr); 
    else 
        mId = -1; // laisse la base de donnée mettre l'id
} 

std::string Employee::toJson() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << mPosition;
    std::string position2precision = oss.str();

    std::string json = "{";
    json += "\"id\":" + std::to_string(mId) + ",";
    json += "\"firstname\":\"" + mFirstname + "\",";
    json += "\"lastname\":\"" + mLastname + "\",";
    json += "\"birthdate\":\"" + mBirthdate + "\",";
    json += "\"job\":\"" + mJob + "\",";
    json += "\"executive_status\":" + std::string(mExecutiveStatus ? "true" : "false") + ",";
    json += "\"position\":" + position2precision + ",";
    json += "\"coefficient\":" + std::to_string(mCoefficient) + ",";
    json += "\"start_date\":\"" + mStartDate + "\",";
    json += "\"manager_id\":" + std::to_string(mManagerId) + ",";
    json += "\"prevPlan\":\"" + mPrevPlan + "\",";
    json += "\"signed_plan\":" + std::string(mSignedPlan ? "true" : "false");
    json += "}";
    return json;
}

void Employee::display(void) const {
    std::cout << "Employee: " << std::endl;
    std::cout << "\tID: " << mId << std::endl;
    std::cout << "\tFirstname: " << mFirstname << std::endl;
    std::cout << "\tLastname: " << mLastname << std::endl;
    std::cout << "\tBirthdate: " << mBirthdate << std::endl;
    std::cout << "\tPoste: " << mJob << std::endl;
    std::cout << "\tIs Cadre: " << (mExecutiveStatus ? "Yes" : "No") << std::endl;
    std::cout << "\tPosition Syntec: " << mPosition << std::endl;
    std::cout << "\tCoefficient: " << mCoefficient << std::endl;
    std::cout << "\tStart Date: " << mStartDate << std::endl;
    std::cout << "\tManager ID: " << mManagerId << std::endl;
    std::cout << "\tPrev Plan: " << mPrevPlan << std::endl;
    std::cout << "\tSigned Plan: " << (mSignedPlan ? "Yes" : "No") << std::endl;
}

Employee Employee::fromSql(const std::map<std::string, std::string> &sql_row) {

    // Fonction lambda pour convertir une chaîne en entier avec une valeur par défaut
    auto to_int_or_default = [&](const std::string& key, int defaultValue) -> int {
        const auto it = sql_row.find(key);
        if (it == sql_row.end())
            return defaultValue;

        const std::string& value = it->second;
        if (value.empty() || value == "NULL" || value == "null")
            return defaultValue;

        try {
            return stoi(value);
        } catch (const std::exception&) {
            return defaultValue;
        }
    };

    Employee emp;
    emp.setId(to_int_or_default("id", -1));
    emp.setFirstname(sql_row.at("firstname").empty() ? "" : sql_row.at("firstname"));
    emp.setLastname(sql_row.at("lastname").empty() ? "" : sql_row.at("lastname"));
    emp.setJob(sql_row.at("job").empty() ? "" : sql_row.at("job"));
    emp.setBirthdate(sql_row.at("birthdate").empty() ? "" : sql_row.at("birthdate"));
    emp.setStartDate(sql_row.at("start_date").empty() ? "" : sql_row.at("start_date"));
    emp.setExecutiveStatus(sql_row.at("executive_status").empty() ? false : sql_row.at("executive_status") == "1");
    emp.setPosition(sql_row.at("position").empty() ? 0.0f : stof(sql_row.at("position")));
    emp.setCoefficient(to_int_or_default("coefficient", 0));
    emp.setManagerId(to_int_or_default("manager_id", -1));
    emp.setPrevPlan(sql_row.at("prev_plan").empty() ? "" : sql_row.at("prev_plan"));
    emp.setSignedPlan(sql_row.at("signed_plan").empty() ? false : sql_row.at("signed_plan") == "1");
    return emp;
}



/* ------------------- Opérateurs ------------------- */

///////// COMPARAISONS //////////
bool operator==(Employee const& a, Employee const& b) {
    return (a.id() == b.id() && 
            a.firstname() == b.firstname() &&
            a.lastname() == b.lastname() &&
            a.job() == b.job() &&
            a.birthdate() == b.birthdate() &&
            a.startDate() == b.startDate() &&
            a.isExecutive() == b.isExecutive() &&
            a.position() == b.position() &&
            a.coefficient() == b.coefficient() &&
            a.managerId() == b.managerId() &&
            a.prevPlan() == b.prevPlan() &&
            a.signedPlan() == b.signedPlan());
}
bool operator!=(Employee const& a, Employee const& b) {
    return !(a.id() == b.id() && 
            a.firstname() == b.firstname() &&
            a.lastname() == b.lastname() &&
            a.job() == b.job() &&
            a.birthdate() == b.birthdate() &&
            a.startDate() == b.startDate() &&
            a.isExecutive() == b.isExecutive() &&
            a.position() == b.position() &&
            a.coefficient() == b.coefficient() &&
            a.managerId() == b.managerId() &&
            a.prevPlan() == b.prevPlan() &&
            a.signedPlan() == b.signedPlan());
}


///////// FLUX //////////
std::ostream &operator<<(std::ostream &flux, Employee const& e) {
    flux << e.toJson();
    return flux;
} 

