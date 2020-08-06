QT -= gui

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

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
    src/week.cpp

HEADERS += \
    include/day.h \
    include/group.h \
    include/module.h \
    include/plan.h \
    include/semester.h \
    include/timeslot.h \
    include/week.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
