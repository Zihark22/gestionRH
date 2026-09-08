#include "date.hpp"


// Constructeur avec validation
Date::Date(int day, int month, int year) {
    if (!isValidDate(day, month, year)) {
        throw std::invalid_argument("Date invalide : " + std::to_string(day) + "/"
                                    + std::to_string(month) + "/" + std::to_string(year));
    }
    m_day = day;
    m_month = month;
    m_year = year;
}
Date::Date(const std::string& dateStr) : Date([&]() {
        // Implémentez la conversion d'une chaîne de caractères en date
        // Exemple: "dd/mm/yyyy"
        size_t firstSlash = dateStr.find('/');
        if(firstSlash == std::string::npos)
            firstSlash = dateStr.find('-');

        size_t secondSlash = dateStr.find('/', firstSlash + 1);
        if(secondSlash == std::string::npos)
            secondSlash = dateStr.find('-', firstSlash + 1);

        if (firstSlash != std::string::npos && secondSlash != std::string::npos) {
            int firstPart  = stoi(dateStr.substr(0, firstSlash));
            int secondPart = stoi(dateStr.substr(firstSlash + 1, secondSlash - firstSlash - 1));
            int thirdPart  = stoi(dateStr.substr(secondSlash + 1));

            if(firstPart>1000) // yyyy/MM/dd
            {
                return Date(thirdPart, secondPart, firstPart); // Appelle la validation du constructeur (int, int, int)
            }
            else {
                return Date(firstPart, secondPart, thirdPart);
            }
        }
        else {
            throw std::invalid_argument("Format de la date non conforme (dd/MM/yyyy ou dd-MM-yyyy ou yyyy/MM/dd) : " + dateStr);
        }

    }())
{

        // Le corps est vide, tout a été délégué et validé !
}

bool Date::isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
bool Date::isValidDate(int d, int m, int y) {
    if (y < 1 || m < 1 || m > 12 || d < 1) {
        return false;
    }

    // Nombre de jours par mois
    std::array<int, 12> daysInMonth = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (m == 2 && isLeapYear(y)) {
        daysInMonth[1] = 29; // Février année bissextile
    }

    return d <= daysInMonth[m - 1];
}

// Sécuriser les setters
void Date::setDay(int day) {
    if (!isValidDate(day, m_month, m_year)) {
        throw std::invalid_argument("Jour invalide pour cette date : " + std::to_string(day));
    }
    m_day = day;
}

std::string Date::toString() const {
    std::string format_day, format_month;

    if(m_day<10)
        format_day = "0";
    format_day += std::to_string(m_day);

    if(m_month<10)
        format_month = "0";
    format_month += std::to_string(m_month);

    return format_day + "/" + format_month + "/" + std::to_string(m_year);
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
std::ostream &operator<<(std::ostream &flux, Date const& m_date) {
    // a faire
    flux << m_date.toString();
    return flux;
}
