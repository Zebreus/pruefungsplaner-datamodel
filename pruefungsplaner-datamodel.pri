INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD

CONFIG += c++17
include($$PWD/libs/qt-json-serialization/qt-json-serialization.pri)

SOURCES += \
    $$PWD/src/day.cpp \
    $$PWD/src/group.cpp \
    $$PWD/src/module.cpp \
    $$PWD/src/plan.cpp \
    $$PWD/src/semester.cpp \
    $$PWD/src/timeslot.cpp \
    $$PWD/src/week.cpp \
    $$PWD/src/plancsvhelper.cpp

HEADERS += \
    $$PWD/include/day.h \
    $$PWD/include/group.h \
    $$PWD/include/module.h \
    $$PWD/include/plan.h \
    $$PWD/include/semester.h \
    $$PWD/include/timeslot.h \
    $$PWD/include/week.h \
    $$PWD/include/plancsvhelper.h

test{
    LIBS *= -lgtest
    INCLUDEPATH *= $$PWD/tests/include

    SOURCES += $$PWD/tests/qthelper.cpp \
            $$PWD/tests/plancsvhelpertest.cpp \
            $$PWD/tests/testdatatest.cpp
    HEADERS += $$PWD/tests/include/testdatahelper.h

    RESOURCES += $$PWD/tests/testdata.qrc
}
