#ifndef DATE_HPP
#define DATE_HPP

#include <string>
#include <stdexcept> // Pour std::invalid_argument
#include <array>

class Date {
public:
    // Constructeur par défaut
    Date() = default;
    
    // Constructeur principal avec validation
    Date(int day, int month, int year);

    // Constructeur délégué : Conversion d'une chaîne de caractères en Date
    Date(const std::string& dateStr);


    /////// Méthodes ///////

    // Convertit la date en chaîne de caractères
    std::string toString() const;

    // Vérifie si l'année est bissextile
    bool isLeapYear(int year);
    
    // Vérifie si la date est valide
    bool isValidDate(int d, int m, int y);


    /////// Getters ///////

    int day() const { return myDay; }
    int month() const { return myMonth; }
    int year() const { return myYear; }


    /////// Setters ///////
    
    void setDay(int day);
    void setMonth(int month) { myMonth = month; }
    void setYear(int year) { myYear = year; }

private:
    int myDay{1};
    int myMonth{1};
    int myYear{2000};
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
