#ifndef DATE_HPP
#define DATE_HPP

#include <string>

using namespace std;

class Date {
public:
    Date() = default;
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
    string toString() const {
        return to_string(m_day) + "/" + to_string(m_month) + "/" + to_string(m_year);
    }
    static Date fromString(const string& dateStr) {
        // Implémentez la conversion d'une chaîne de caractères en date
        // Exemple: "dd/mm/yyyy"
        size_t firstSlash = dateStr.find('/');
        size_t secondSlash = dateStr.find('/', firstSlash + 1);
        if (firstSlash != string::npos && secondSlash != string::npos) {
            return Date(stoi(dateStr.substr(0, firstSlash)), stoi(dateStr.substr(firstSlash + 1, secondSlash - firstSlash - 1)), stoi(dateStr.substr(secondSlash + 1)));
        }
        return Date(); // Retourne une date par défaut si le format est incorrect
    }


private:
    int m_day{1};
    int m_month{1};
    int m_year{2000};
};

#endif // DATE_HPP
