#-------------------------------------------------
#
# Project created by QtCreator 2018-12-07T22:57:25
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Fiszki
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

SOURCES += \
        main.cpp \
        fiszkimainwindow.cpp \
    userlistwindow.cpp \
    dbmanager.cpp \
    addnewuserwindow.cpp \
    errorcreatinguserwindow.cpp \
    aboutwindow.cpp \
    question.cpp \
    session.cpp \
    user.cpp \
    addquestionwindow.cpp \
    mistakewindow.cpp \
    endfortoday.cpp

HEADERS += \
        fiszkimainwindow.h \
    userlistwindow.h \
    dbmanager.h \
    addnewuserwindow.h \
    errorcreatinguserwindow.h \
    aboutwindow.h \
    question.h \
    session.h \
    enums.h \
    user.h \
    addquestionwindow.h \
    mistakewindow.h \
    endfortoday.h

FORMS += \
        fiszkimainwindow.ui \
    userlistwindow.ui \
    addnewuserwindow.ui \
    errorcreatinguserwindow.ui \
    aboutwindow.ui \
    addquestionwindow.ui \
    mistakewindow.ui \
    endfortoday.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
