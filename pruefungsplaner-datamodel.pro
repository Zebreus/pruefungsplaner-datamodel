QT -= gui

CONFIG += c++17

INCLUDEPATH += $$PWD/include

include($$PWD/libs/qt-json-serialization/qt-json-serialization.pri)

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    src/day.cpp \
    src/group.cpp \
    src/module.cpp \
    src/plan.cpp \
    src/semester.cpp \
    src/timeslot.cpp \
    src/week.cpp \
    src/plancsvhelper.cpp

HEADERS += \
    include/day.h \
    include/group.h \
    include/module.h \
    include/plan.h \
    include/semester.h \
    include/timeslot.h \
    include/week.h \
    include/plancsvhelper.h

test{
    include(libs/gtest/gtest_dependency.pri)

    QT += testlib
    TEMPLATE = app
    TARGET = pruefungsplaner-datamodel-tests

    CONFIG += thread
    CONFIG -= app_bundle
    LIBS += -lgtest -lgtest_main
    INCLUDEPATH += src

    SOURCES += tests/qthelper.cpp \
            tests/plancsvhelpertest.cpp \
            tests/testdatatest.cpp
    HEADERS += tests/testdatahelper.h

    # Default rules for test deployment.
    qnx: target.path = /tmp/$${TARGET}/bin
    else: unix:!android: target.path = /opt/$${TARGET}/bin
    !isEmpty(target.path): INSTALLS += target
}
else{
    TEMPLATE = lib
    CONFIG += staticlib
    TARGET = pruefungsplaner-datamodel

    # Default rules for library deployment.
    unix {
        target.path = $$[QT_INSTALL_PLUGINS]/generic
    }
    !isEmpty(target.path): INSTALLS += target
}

RESOURCES += \
    tests/testdata.qrc
