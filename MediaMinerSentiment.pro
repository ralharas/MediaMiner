QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += debug

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    sentiment_binding.cpp \
    fastText/fasttext.cc \
    fastText/args.cc \
    fastText/dictionary.cc \
    fastText/matrix.cc \
    fastText/model.cc \
    fastText/utils.cc \
    fastText/vector.cc

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TARGET = MediaMinerSentiment
TEMPLATE = app

INCLUDEPATH += $$PWD/fastText
LIBS += -L$$PWD -lfasttext

# Suppress FastText warnings
QMAKE_CXXFLAGS += -Wno-sign-compare -Wno-defaulted-function-deleted
QMAKE_MACOSX_DEPLOYMENT_TARGET = 15.0
