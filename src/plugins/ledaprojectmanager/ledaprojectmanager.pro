include(../../qtcreatorplugin.pri)

HEADERS = ledaproject.h \
    ledaprojectplugin.h \
    ledaprojectmanager.h \
    ledaprojectconstants.h \
    ledaprojectnodes.h \
    ledaprojectfileseditor.h
SOURCES = ledaproject.cpp \
    ledaprojectplugin.cpp \
    ledaprojectmanager.cpp \
    ledaprojectnodes.cpp \
    ledaprojectfileseditor.cpp
RESOURCES += ledaproject.qrc
FORMS += ledamakestep.ui

# equals(TEST, 1) {
#     SOURCES += ledaprojectplugin_test.cpp
#     DEFINES += SRCDIR=\\\"$$PWD\\\"
# }
