#-------------------------------------------------
#
# Project created by QtCreator 2018-07-31T11:58:36
#
#-------------------------------------------------

QT       -= core gui

TARGET = TestNUKEEModule.dll
TEMPLATE = lib

DEFINES += TESTNUKEEMODULE_LIBRARY GLM_ENABLE_EXPERIMENTAL

INCLUDEPATH += ../NukeEngine-Editor

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        testnukeemodule.cpp

HEADERS += \
        testnukeemodule.h \
    ../NukeEngine-Editor/interface/NUKEEInteface.h

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../NukeEngine/build/debug/release/ -lNukeEngine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../NukeEngine/build/debug/debug/ -lNukeEngine
else:unix: LIBS += -L$$PWD/../NukeEngine/build/debug/ -lNukeEngine

INCLUDEPATH += $$PWD/../NukeEngine
DEPENDPATH += $$PWD/../NukeEngine
