#include "../includes/employee.hpp"

#include <gtest/gtest.h>
#include <sstream>

// ==========================================
// 1. Tests des Constructeurs et Initialisations
// ==========================================

TEST(EmployeeTest, DefaultConstructor_ShouldInitializeToDefaultEmployee) {
    // Arrange & Act
    Employee e;

    // Assert
    EXPECT_EQ(e.firstname(), "");
    EXPECT_EQ(e.lastname(), "");
    EXPECT_EQ(e.job(), "");
    EXPECT_EQ(e.birthdate(), "2000-01-01");
    EXPECT_EQ(e.startDate(), "2010-01-01");
    EXPECT_EQ(e.isExecutive(), 0);
    EXPECT_EQ(e.coefficient(), 0);
    EXPECT_EQ(e.position(), 0.0);
    EXPECT_EQ(e.id(), -1);
    EXPECT_EQ(e.managerId(), -1);
    EXPECT_EQ(e.prevPlan(), "Plan A");
    EXPECT_EQ(e.signedPlan(), 0);
}

TEST(EmployeeTest, ParameterizedConstructor_ShouldSetCorrectValues) {
    // Arrange & Act
    Employee e("[{\"id\":15,\"firstname\":\"Manon\",\"lastname\":\"Morel\",\"birthdate\":\"14/05/2001\",\
        \"job\":\"Développeuse C++ Junior\",\"executive_status\":true,\"position\":1.20,\"coefficient\":100,\
        \"start_date\":\"01/09/2023\",\"manager_id\":4,\"prevPlan\":\"Plan C\",\"signed_plan\":true}]");

    e.display();
    // Assert
    EXPECT_EQ(e.id(), 15);
    EXPECT_EQ(e.firstname(), "Manon");
    EXPECT_EQ(e.lastname(), "Morel");
    EXPECT_EQ(e.job(), "Développeuse C++ Junior");
    EXPECT_EQ(e.birthdate(), "2001-05-14");
    EXPECT_EQ(e.startDate(), "2023-09-01");
    EXPECT_EQ(e.isExecutive(), 1);
    EXPECT_EQ(e.coefficient(), 100);
    EXPECT_FLOAT_EQ(e.position(), 1.2);
    EXPECT_EQ(e.managerId(), 4);
    EXPECT_EQ(e.prevPlan(), "Plan C");
    EXPECT_EQ(e.signedPlan(), 1);
}

// TEST(EmployeeTest, StringConstructor_ValidFormat_ShouldParseCorrectly) {
//     // Arrange & Act (Exemple: format "DD/MM/YYYY" ou "YYYY-MM-DD" selon votre implémentation)
//     Date d("15/08/2024");

//     // Assert
//     EXPECT_EQ(d.day(), 15);
//     EXPECT_EQ(d.month(), 8);
//     EXPECT_EQ(d.year(), 2024);
// }

// // ==========================================
// // 2. Tests des Setters
// // ==========================================

// TEST(EmployeeTest, Setters_ShouldUpdateValuesIndependently) {
//     Date d;
    
//     d.setDay(25);
//     d.setMonth(12);
//     d.setYear(2025);

//     EXPECT_EQ(d.day(), 25);
//     EXPECT_EQ(d.month(), 12);
//     EXPECT_EQ(d.year(), 2025);
// }

// // ==========================================
// // 3. Tests des Opérateurs de Comparaison
// // ==========================================

// TEST(EmployeeTest, EqualityOperator_SameDates_ShouldReturnTrue) {
//     Date d1(10, 5, 2022);
//     Date d2(10, 5, 2022);

//     EXPECT_TRUE(d1 == d2);
//     EXPECT_FALSE(d1 != d2);
// }

// TEST(EmployeeTest, RelationalOperators_DifferentYears_ShouldOrderCorrectly) {
//     Date past(1, 1, 2020);
//     Date future(1, 1, 2025);

//     EXPECT_LT(past, future);
//     EXPECT_LE(past, future);
//     EXPECT_GT(future, past);
//     EXPECT_GE(future, past);
// }

// TEST(EmployeeTest, RelationalOperators_SameYearDifferentMonth_ShouldOrderCorrectly) {
//     Date earlier(1, 3, 2023);
//     Date later(1, 7, 2023);

//     EXPECT_LT(earlier, later);
// }

// TEST(EmployeeTest, RelationalOperators_SameMonthDifferentDay_ShouldOrderCorrectly) {
//     Date earlier(5, 3, 2023);
//     Date later(20, 3, 2023);

//     EXPECT_LT(earlier, later);
// }

// // ==========================================
// // 4. Tests des Formats et Sorties Flux
// // ==========================================

// TEST(EmployeeTest, ToString_ShouldReturnExpectedString) {
//     Date d(5, 9, 2024);
//     EXPECT_EQ(d.toString(), "05/09/2024"); // Adapter selon votre format
// }

// TEST(EmployeeTest, StreamOperator_ShouldOutputSameAsToString) {
//     Date d(1, 1, 2024);
//     std::ostringstream oss;
//     oss << d;
    
//     EXPECT_EQ(oss.str(), d.toString());
// }


// // Test de dates valides
// TEST(EmployeeTest, ValidDatesDoNotThrow) {
//     EXPECT_NO_THROW(Date(15, 6, 2024));
//     EXPECT_NO_THROW(Date(29, 2, 2024)); // 2024 est bissextile
// }

// // Test de dates invalides
// TEST(EmployeeTest, InvalidDatesThrowException) {
//     // Jour > 31
//     EXPECT_THROW(Date(32, 1, 2024), std::invalid_argument);

//     // Mois invalide
//     EXPECT_THROW(Date(10, 13, 2024), std::invalid_argument);

//     // 29 février sur une année non bissextile
//     EXPECT_THROW(Date(29, 2, 2023), std::invalid_argument);

//     // Jour négatif ou nul
//     EXPECT_THROW(Date(0, 5, 2024), std::invalid_argument);
//     EXPECT_THROW(Date(-5, 5, 2024), std::invalid_argument);
// }