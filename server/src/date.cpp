#include "../includes/date.hpp"

Date::Date(const string& dateStr) {
    // Implémentez la conversion d'une chaîne de caractères en date
    // Exemple: "dd/mm/yyyy"
    size_t firstSlash = dateStr.find('/');
    if(firstSlash == string::npos)
        firstSlash = dateStr.find('-');

    size_t secondSlash = dateStr.find('/', firstSlash + 1);
    if(secondSlash == string::npos)
        secondSlash = dateStr.find('-', firstSlash + 1);

    if (firstSlash != string::npos && secondSlash != string::npos) {
        this->m_day = stoi(dateStr.substr(0, firstSlash));
        this->m_month = stoi(dateStr.substr(firstSlash + 1, secondSlash - firstSlash - 1));
        this->m_year = stoi(dateStr.substr(secondSlash + 1));
    }
    else {
        this->m_day = 1;
        this->m_month = 1;
        this->m_year = 2000;
    } 
}

string Date::toString() const {
    return to_string(m_day) + "/" + to_string(m_month) + "/" + to_string(m_year);
}




/* ------------------- Opérateurs ------------------- */

///////// COMPARAISONS //////////
bool operator==(Date const& a, Date const& b)
{
    return (a.day() == b.day() && a.month() == b.month() && a.year() == b.year());
}
bool operator!=(Date const& a, Date const& b)
{
    return !(a.day()==b.day() and a.month()==b.month() and a.year()==b.year());
}
bool operator>(Date const& a, Date const& b)
{
    if(a.year() > b.year())      // année plus grande que b
        return true;
    else if(a.year() < b.year()) // année plus petite que b
        return false;
    else {                          // même année que b
        if(a.month() > b.month())   // mois plus grand que b
            return true;
        else if(a.month() < b.month()) // mois plus petit que b
            return false;
        else {                          // même mois que b
            if(a.day() > b.day())   // jour plus grand que b
                return true;
            else if(a.day() < b.day()) // jour plus petit que b
                return false;
            else                            // même jour que b
                return false;
        }
    }
}
bool operator<(Date const& a, Date const& b)
{
    if(a.year() < b.year())      // année plus petite que b
        return true;
    else if(a.year() > b.year()) // année plus grande que b
        return false;
    else {                          // même année que b
        if(a.month() < b.month())   // mois plus petit que b
            return true;
        else if(a.month() > b.month()) // mois plus grand que b
            return false;
        else {                          // même mois que b
            if(a.day() < b.day())   // jour plus petit que b
                return true;
            else if(a.day() > b.day()) // jour plus grand que b
                return false;
            else                            // même jour que b
                return false;
        }
    }
}
bool operator<=(Date const& a, Date const& b) {
    
    if(a.year() < b.year())      // année plus petite que b
        return true;
    else if(a.year() > b.year()) // année plus grande que b
        return false;
    else {                          // même année que b
        if(a.month() < b.month())   // mois plus petit que b
            return true;
        else if(a.month() > b.month()) // mois plus grand que b
            return false;
        else {                          // même mois que b
            if(a.day() < b.day())   // jour plus petit que b
                return true;
            else if(a.day() > b.day()) // jour plus grand que b
                return false;
            else                            // même jour que b
                return true;
        }
    }
}
bool operator>=(Date const& a, Date const& b) {
     if(a.year() > b.year())      // année plus grande que b
        return true;
    else if(a.year() < b.year()) // année plus petite que b
        return false;
    else {                          // même année que b
        if(a.month() > b.month())   // mois plus grand que b
            return true;
        else if(a.month() < b.month()) // mois plus petit que b
            return false;
        else {                          // même mois que b
            if(a.day() > b.day())   // jour plus grand que b
                return true;
            else if(a.day() < b.day()) // jour plus petit que b
                return false;
            else                            // même jour que b
                return true;
        }
    }
}


