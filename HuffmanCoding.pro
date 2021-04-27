TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

HEADERS +=

DISTFILES +=

macx: {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15
}
