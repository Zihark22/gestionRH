QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

CONFIG += c++17

TEMPLATE = app
TARGET = HRmanagement

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# 1. Permet de trouver les headers dans include/
INCLUDEPATH += include

SOURCES += \
    src/apiclient.cpp \
    src/configappwindow.cpp \
    src/configserverwindow.cpp \
    src/datetablewidgetitem.cpp \
    src/employee.cpp \
    src/errorwidget.cpp \
    src/formwindow.cpp \
    src/homewidget.cpp \
    src/hrmanagement.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/tablewidget.cpp \
    src/tabwidget.cpp \
    src/viewmanager.cpp

HEADERS += \
    include/apiclient.hpp \
    include/configappwindow.hpp \
    include/configserverwindow.hpp \
    include/datetablewidgetitem.hpp \
    include/employee.hpp \
    include/errorwidget.hpp \
    include/formwindow.hpp \
    include/homewidget.hpp \
    include/hrmanagement.hpp \
    include/mainwindow.hpp \
    include/parameters.hpp \
    include/tablewidget.hpp \
    include/tabwidget.hpp \
    include/viewmanager.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ressources.qrc
