TEMPLATE = app
CONFIG += console c++11
CONFIG += app_bundle
CONFIG += qt
QT -= gui
HEADERS += \
    disco.h \
    gram.tab.h \
    union.h
SOURCES += \
    disco.cpp \
    gram.tab.c \
    lex.yy.c \
    union.c


