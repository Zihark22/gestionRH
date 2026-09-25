# 1. Ajouter le module testlib en plus de core et widgets
QT += core gui widgets testlib

TEMPLATE = app
TARGET = tst_HRmanagement

CONFIG += c++17
# Optionnel : désactive l'ouverture d'une fenêtre console sous Windows
CONFIG += console
CONFIG -= app_bundle

# 2. Permet d'inclure les headers de l'application (#include "Calculatrice.h") sans mettre ../
INCLUDEPATH += ../app/

# 3. Compiler le code de test ET les classes métier de l'application
SOURCES += \
    TestEmployee.cpp \
    ../app/src/employee.cpp

HEADERS += \
    ../app/include/employee.hpp
