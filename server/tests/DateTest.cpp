#include <gtest/gtest.h>
#include "../includes/date.hpp"
#include <sstream>

// ==========================================
// 1. Tests des Constructeurs et Initialisations
// ==========================================

TEST(DateTest, DefaultConstructor_ShouldInitializeToDefaultDate) {
    // Arrange & Act
    Date d;

    // Assert
    EXPECT_EQ(d.day(), 1);
    EXPECT_EQ(d.month(), 1);
    EXPECT_EQ(d.year(), 2000);
}

TEST(DateTest, ParameterizedConstructor_ShouldSetCorrectValues) {
    // Arrange & Act
    Date d(15, 8, 2024);

    // Assert
    EXPECT_EQ(d.day(), 15);
    EXPECT_EQ(d.month(), 8);
    EXPECT_EQ(d.year(), 2024);
}

TEST(DateTest, StringConstructor_ValidFormat_ShouldParseCorrectly) {
    // Arrange & Act (Exemple: format "DD/MM/YYYY" ou "YYYY-MM-DD" selon votre implémentation)
    Date d("15/08/2024");

    // Assert
    EXPECT_EQ(d.day(), 15);
    EXPECT_EQ(d.month(), 8);
    EXPECT_EQ(d.year(), 2024);
}

// ==========================================
// 2. Tests des Setters
// ==========================================

TEST(DateTest, Setters_ShouldUpdateValuesIndependently) {
    Date d;
    
    d.setDay(25);
    d.setMonth(12);
    d.setYear(2025);

    EXPECT_EQ(d.day(), 25);
    EXPECT_EQ(d.month(), 12);
    EXPECT_EQ(d.year(), 2025);
}

// ==========================================
// 3. Tests des Opérateurs de Comparaison
// ==========================================

TEST(DateTest, EqualityOperator_SameDates_ShouldReturnTrue) {
    Date d1(10, 5, 2022);
    Date d2(10, 5, 2022);

    EXPECT_TRUE(d1 == d2);
    EXPECT_FALSE(d1 != d2);
}

TEST(DateTest, RelationalOperators_DifferentYears_ShouldOrderCorrectly) {
    Date past(1, 1, 2020);
    Date future(1, 1, 2025);

    EXPECT_LT(past, future);
    EXPECT_LE(past, future);
    EXPECT_GT(future, past);
    EXPECT_GE(future, past);
}

TEST(DateTest, RelationalOperators_SameYearDifferentMonth_ShouldOrderCorrectly) {
    Date earlier(1, 3, 2023);
    Date later(1, 7, 2023);

    EXPECT_LT(earlier, later);
}

TEST(DateTest, RelationalOperators_SameMonthDifferentDay_ShouldOrderCorrectly) {
    Date earlier(5, 3, 2023);
    Date later(20, 3, 2023);

    EXPECT_LT(earlier, later);
}

// ==========================================
// 4. Tests des Formats et Sorties Flux
// ==========================================

TEST(DateTest, ToString_ShouldReturnExpectedString) {
    Date d(5, 9, 2024);
    EXPECT_EQ(d.toString(), "05/09/2024"); // Adapter selon votre format
}

TEST(DateTest, StreamOperator_ShouldOutputSameAsToString) {
    Date d(1, 1, 2024);
    std::ostringstream oss;
    oss << d;
    
    EXPECT_EQ(oss.str(), d.toString());
}