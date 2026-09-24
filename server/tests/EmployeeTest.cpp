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
    EXPECT_EQ(e.id(), 0);
    EXPECT_EQ(e.managerId(), 0);
    EXPECT_EQ(e.prevPlan(), "Plan A");
    EXPECT_EQ(e.signedPlan(), 0);
}

TEST(EmployeeTest, ParameterizedConstructor_ShouldSetCorrectValues) {
    // Arrange & Act
    Employee e("[{\"id\":15,\"firstname\":\"Manon\",\"lastname\":\"Morel\",\"birthdate\":\"2001-05-14\",\
        \"job\":\"Développeuse C++ Junior\",\"executive_status\":true,\"position\":1.20,\"coefficient\":100,\
        \"start_date\":\"2023-09-01\",\"manager_id\":4,\"prevPlan\":\"Plan C\",\"signed_plan\":true}]");

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

// ==========================================
// 2. Tests des Setters
// ==========================================

TEST(EmployeeTest, Setters_ShouldUpdateValuesIndependently) {
    Employee e;
    
    e.setId(30);
    e.setFirstname("Paul");
    e.setLastname("Dupont");
    e.setJob("Développeur");
    e.setExecutiveStatus(0);
    e.setManagerId(5);
    e.setPrevPlan("Plan A");
    e.setSignedPlan(1);
    e.setCoefficient(215);
    e.setPosition(2.4);
    e.setBirthdate("2015-02-06");
    e.setStartDate("2018-11-26");

    EXPECT_EQ(e.id(), 30);
    EXPECT_EQ(e.firstname(), "Paul");
    EXPECT_EQ(e.lastname(), "Dupont");
    EXPECT_EQ(e.job(), "Développeur");
    EXPECT_EQ(e.isExecutive(), 0);
    EXPECT_EQ(e.managerId(), 5);
    EXPECT_EQ(e.prevPlan(), "Plan A");
    EXPECT_EQ(e.signedPlan(), 1);
    EXPECT_EQ(e.coefficient(), 215);
    EXPECT_FLOAT_EQ(e.position(), 2.4);
    EXPECT_EQ(e.birthdate(), "2015-02-06");
    EXPECT_EQ(e.startDate(), "2018-11-26");

}

// ==========================================
// 3. Tests des Opérateurs de Comparaison
// ==========================================

TEST(EmployeeTest, EqualityOperator_SameEmployee_ShouldReturnTrue) {
    Employee e1("[{\"id\":15,\"firstname\":\"Manon\",\"lastname\":\"Morel\",\"birthdate\":\"2001-05-14\",\
        \"job\":\"Développeuse C++ Junior\",\"executive_status\":true,\"position\":1.20,\"coefficient\":100,\
        \"start_date\":\"2023-09-01\",\"manager_id\":4,\"prevPlan\":\"Plan C\",\"signed_plan\":true}]");

    Employee e2("[{\"id\":15,\"firstname\":\"Manon\",\"lastname\":\"Morel\",\"birthdate\":\"2001-05-14\",\
        \"job\":\"Développeuse C++ Junior\",\"executive_status\":true,\"position\":1.20,\"coefficient\":100,\
        \"start_date\":\"2023-09-01\",\"manager_id\":4,\"prevPlan\":\"Plan C\",\"signed_plan\":true}]");

    EXPECT_TRUE(e1 == e2);
    EXPECT_FALSE(e1 != e2);
}

TEST(EmployeeTest, EqualityOperator_DifferentJobEmployee_ShouldReturnFalse) {
    Employee e1("[{\"id\":15,\"firstname\":\"Manon\",\"lastname\":\"Morel\",\"birthdate\":\"2001-05-14\",\
        \"job\":\"Développeuse C++ Junior\",\"executive_status\":true,\"position\":1.20,\"coefficient\":100,\
        \"start_date\":\"2023-09-01\",\"manager_id\":4,\"prevPlan\":\"Plan C\",\"signed_plan\":true}]");

    Employee e2("[{\"id\":15,\"firstname\":\"Manon\",\"lastname\":\"Morel\",\"birthdate\":\"2001-05-14\",\
        \"job\":\"Développeuse C+ Junior\",\"executive_status\":true,\"position\":1.20,\"coefficient\":100,\
        \"start_date\":\"2023-09-01\",\"manager_id\":4,\"prevPlan\":\"Plan C\",\"signed_plan\":true}]");

    EXPECT_FALSE(e1 == e2);
    EXPECT_TRUE(e1 != e2);
}

// ==========================================
// 4. Tests des Formats et Sorties Flux
// ==========================================

TEST(EmployeeTest, ToJson_ShouldReturnExpectedJson) {
    Employee e("[{\"id\":15,\"firstname\":\"Manon\",\"lastname\":\"Morel\",\"birthdate\":\"2001-05-14\",\
        \"job\":\"Développeuse C++ Junior\",\"executive_status\":true,\"position\":1.2,\"coefficient\":100,\
        \"start_date\":\"2023-09-01\",\"manager_id\":4,\"prevPlan\":\"Plan C\",\"signed_plan\":true}]");

    std::string json = "{\"id\":15,\"firstname\":\"Manon\",\"lastname\":\"Morel\",\"birthdate\":\"2001-05-14\",\
\"job\":\"Développeuse C++ Junior\",\"executive_status\":true,\"position\":1.2,\"coefficient\":100,\
\"start_date\":\"2023-09-01\",\"manager_id\":4,\"prevPlan\":\"Plan C\",\"signed_plan\":true}";

    EXPECT_EQ(e.toJson(), json);
}

TEST(EmployeeTest, StreamOperator_ShouldOutputSameAsToJson) {
    Employee e("[{\"id\":15,\"firstname\":\"Manon\",\"lastname\":\"Morel\",\"birthdate\":\"2001-05-14\",\
        \"job\":\"Développeuse C++ Junior\",\"executive_status\":true,\"position\":1.20,\"coefficient\":100,\
        \"start_date\":\"2023-09-01\",\"manager_id\":4,\"prevPlan\":\"Plan C\",\"signed_plan\":true}]");

    std::ostringstream oss;
    oss << e;
    
    EXPECT_EQ(oss.str(), e.toJson());
}
