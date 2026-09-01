#ifndef DATE_HPP
#define DATE_HPP

#include <string>
#include <stdexcept> // Pour std::invalid_argument
#include <array>

class Date {
public:
    Date() = default;

    // Constructeur principal avec validation
    Date(int day, int month, int year);

    // Constructeur délégué depuis std::string
    Date(const std::string& dateStr);

    // Getters
    int day() const { return m_day; }
    int month() const { return m_month; }
    int year() const { return m_year; }

    // Setters
    void setDay(int day);
    void setMonth(int month) { m_month = month; }
    void setYear(int year) { m_year = year; }

    // Méthode pour convertir la date en chaîne de caractères
    std::string toString() const;
    bool isLeapYear(int year);
    bool isValidDate(int d, int m, int y);


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
