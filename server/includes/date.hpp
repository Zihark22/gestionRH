#ifndef DATE_HPP
#define DATE_HPP

#include <string>

class Date {
public:
    Date() = default;
    Date(const std::string& dateStr);
    Date(int day, int month, int year) : m_day(day), m_month(month), m_year(year) {}

    // Getters
    int day() const { return m_day; }
    int month() const { return m_month; }
    int year() const { return m_year; }

    // Setters
    void setDay(int day) { m_day = day; }
    void setMonth(int month) { m_month = month; }
    void setYear(int year) { m_year = year; }

    // Méthode pour convertir la date en chaîne de caractères
    std::string toString() const;


private:
    int m_day{1};
    int m_month{1};
    int m_year{2000};
};

/* ------------------- Opérateurs ------------------- */

// Opérateurs de comparaison (utilisation d'accesseurs)
bool operator==(Date const& a, Date const& b);
bool operator!=(Date const& a, Date const& b);
bool operator<=(Date const& a, Date const& b);
bool operator>=(Date const& a, Date const& b);
bool operator>(Date const& a, Date const& b);
bool operator<(Date const& a, Date const& b);


// Opérateurs de flux
std::ostream &operator<<(std::ostream &flux, Date const& m_date);

#endif // DATE_HPP
