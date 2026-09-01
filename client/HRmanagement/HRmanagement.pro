QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    apiclient.cpp \
    date.cpp \
    employee.cpp \
    formwindow.cpp \
    iniparser.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    apiclient.hpp \
    date.hpp \
    employee.hpp \
    formwindow.hpp \
    iniparser.hpp \
    mainwindow.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ressources.qrc
