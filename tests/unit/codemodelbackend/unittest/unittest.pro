QT       += core network testlib
QT       -= gui

TARGET = codemodelbackendtest
CONFIG   += console c++14 testcase
CONFIG   -= app_bundle

TEMPLATE = app

GMOCK_DIR = $$(GMOCK_DIR)
GTEST_DIR = $$GMOCK_DIR/gtest

requires(exists($$GMOCK_DIR))
!exists($$GMOCK_DIR):message("No gmock is found! To enabe unit tests set GMOCK_DIR")

INCLUDEPATH += $$GTEST_DIR $$GTEST_DIR/include $$GMOCK_DIR $$GMOCK_DIR/include

include(../../../../src/libs/sqlite/sqlite-lib.pri)
include(../../../../src/libs/codemodelbackendipc/codemodelbackendipc-lib.pri)
include(../../../../src/tools/codemodelbackend/ipcsource/codemodelbackendclangipc-source.pri)
include(../../../../src/shared/clang/clang_installation.pri)
include(../../../../src/plugins/clangcodemodel/clangcodemodelunittestfiles.pri)

INCLUDEPATH += $$PWD/../../../../src/libs $$PWD/../../../../src/plugins

requires(!isEmpty(LLVM_LIBS))

LIBS += $$LLVM_LIBS
INCLUDEPATH += $$LLVM_INCLUDEPATH
INCLUDEPATH += ../../../../src/libs/utils

osx:QMAKE_CXXFLAGS = -stdlib=libc++

SOURCES += main.cpp \
    $$GTEST_DIR/src/gtest-all.cc \
    $$GMOCK_DIR/src/gmock-all.cc \
    utf8test.cpp \
    sqlstatementbuildertest.cpp \
    createtablesqlstatementbuildertest.cpp \
    sqlitecolumntest.cpp \
    sqlitestatementtest.cpp \
    sqlitetabletest.cpp \
    spydummy.cpp \
    sqlitedatabasetest.cpp \
    sqlitedatabasebackendtest.cpp \
    readandwritecommandblocktest.cpp \
    clientserverinprocesstest.cpp \
    clientserveroutsideprocess.cpp \
    gtest-qt-printing.cpp \
    codecompletiontest.cpp \
    ../../../../src/libs/utils/qtcassert.cpp \
    clangstringtest.cpp \
    translationunittest.cpp \
    clangcodecompleteresultstest.cpp \
    codecompletionsextractortest.cpp \
    unsavedfilestest.cpp \
    projecttest.cpp \
    clangipcservertest.cpp \
    translationunitstest.cpp \
    completionchunkstotextconvertertest.cpp

HEADERS += \
    gtest-qt-printing.h \
    spydummy.h \
    ../../../../src/libs/utils/qtcassert.h \
    mockipclient.h \
    mockipcserver.h

OTHER_FILES += data/complete_testfile_1.cpp \
               data/complete_completer.cpp \
               data/complete_completer_unsaved.cpp \
               data/complete_extractor_function.cpp \
               data/complete_extractor_function_unsaved.cpp \
               data/complete_extractor_function_unsaved_2.cpp \
               data/complete_extractor_variable.cpp \
               data/complete_extractor_class.cpp \
               data/complete_extractor_namespace.cpp \
               data/complete_extractor_enumeration.cpp \
               data/complete_extractor_constructor.cpp \
               data/complete_translationunit_parse_error.cpp

DEFINES += QT_NO_CAST_FROM_ASCII
DEFINES += CODEMODELBACKEND_TESTS
DEFINES += DONT_CHECK_COMMAND_COUNTER
DEFINES += GTEST_HAS_STD_INITIALIZER_LIST_ GTEST_LANG_CXX11

DEFINES += TESTDATA_DIR=\"R\\\"xxx($$PWD/data)xxx\\\"\"
win32:DEFINES += ECHOSERVER=\"R\\\"xxx($$OUT_PWD/../echo)xxx\\\"\"
unix:DEFINES += ECHOSERVER=\"R\\\"xxx($$OUT_PWD/../echoserver/echo)xxx\\\"\"
