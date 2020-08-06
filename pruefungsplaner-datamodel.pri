INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD

CONFIG += c++11
include($$PWD/qt-json-serialization/qt-json-serialization.pri)

SOURCES += \
    $$PWD/src/day.cpp \
    $$PWD/src/group.cpp \
    $$PWD/src/module.cpp \
    $$PWD/src/plan.cpp \
    $$PWD/src/semester.cpp \
    $$PWD/src/timeslot.cpp \
    $$PWD/src/week.cpp

HEADERS += \
    $$PWD/include/day.h \
    $$PWD/include/group.h \
    $$PWD/include/module.h \
    $$PWD/include/plan.h \
    $$PWD/include/semester.h \
    $$PWD/include/timeslot.h \
    $$PWD/include/week.h
