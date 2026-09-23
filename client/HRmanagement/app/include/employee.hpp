#ifndef EMPLOYEE_HPP
#define EMPLOYEE_HPP

#include <QString>
#include <QDate>

using namespace std;

class Employee {
public:
    // Constructeurs

    Employee() = default;

    /** @brief Crée un objet Employee à partir d'un JSON string (ex: réponse d'une requête API REST)
     *  @param json Une chaîne JSON représentant l'employé.
     */
    Employee(const std::string &json);

    // Getters and Setters

    // Identifiant unique BDD
    int id() const { return mId; }
    void setId(int id) { mId = id; }

    QString firstname() const { return mFirstname; }
    void setFirstname(const QString &prenom) { mFirstname = prenom; }

    QString lastname() const { return mLastname; }
    void setLastname(const QString &nom) { mLastname = nom; }

    QDate birthdate() const { return mBirthdate; }
    void setBirthdate(const QDate &date) { mBirthdate = date; }

    QString job() const { return mJob; }
    void setJob(const QString &poste) { mJob = poste; }

    int isExecutive() const { return mExecutiveStatus; }
    void setExecutiveStatus(int cadre) { mExecutiveStatus = cadre; }

    float position() const { return mPosition; }
    void setPosition(const float &pos) { mPosition = pos; }

    int coefficient() const { return mCoefficient; }
    void setCoefficient(int coeff) { mCoefficient = coeff; }

    QDate startDate() const { return mStartDate; }
    void setStartDate(const QDate &date) { mStartDate = date; }

    int managerId() const { return mManagerId; }
    void setManagerId(int id) { mManagerId = id; }

    QString prevPlan() const { return mPrevPlan; }
    void setPrevPlan(QString plan) { mPrevPlan = plan; }

    int signedPlan() const { return mSignedPlan; }
    void setSignedPlan(int signe) { mSignedPlan = signe; }


    // ---------------------------------------------------

    /** @brief Sérialise l'objet Employee en JSON pour l'API REST
     *  @return Chaîne JSON représentant l'employé
     */
    std::string toJson() const;

    // ---------------------------------------------------

    /** @brief Crée un objet Employee à partir d'une ligne de résultat SQL
     *  @param sql_row Une map représentant une ligne de résultat SQL, où les clés sont les noms des colonnes et les valeurs sont les valeurs correspondantes.
     *  @return Un objet Employee initialisé avec les données de la ligne SQL
     */
    static Employee fromSql(const std::map<std::string, std::string> &sql_row);

    // ---------------------------------------------------

    /** @brief Affiche les informations de l'employé dans la sortie standard */
    void display(void) const;

    // Nettoie les espaces/tabulations inutiles en début et fin de chaîne
    static string trim(const string& str);

    // Fonction pour extraire la valeur d'un champ JSON
    static QString getField(const std::string &obj, const std::string &key);


private:
    int mId{-1};                                                        //< Identifiant unique de l'employé
    QString mFirstname{""};                                             //< Prénom de l'employé
    QString mLastname{""};                                              //< Nom de famille de l'employé
    QDate mBirthdate{QDate::fromString("01/01/2000", "yyyy-MM-dd")};    //< Date de naissance de l'employé
    QString mJob{""};                                                   //< Poste de l'employé
    int mExecutiveStatus{0};                                            //< Statut de manager de l'employé
    float mPosition{0.0};                                               //< Position de l'employé
    int mCoefficient{0};                                                //< Coefficient de l'employé
    QDate mStartDate{QDate::fromString("01/01/2010", "yyyy-MM-dd")};    //< Date de début d'activité
    int mManagerId{-1};                                                 //< Identifiant du manager
    QString mPrevPlan{"Plan A"};                                        //< Plan précédent
    int mSignedPlan{0};                                                 //< Plan signé

    std::string cleanJsonString(const std::string &jsonStr);
    void initAttributesFromJsonString(const std::string &obj);
};
#endif // EMPLOYEE_HPP
