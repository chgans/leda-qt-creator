include(../../qtcreatorplugin.pri)

HEADERS = \
	ledadocument.h \
	ledaeditor.h \
	ledaeditorconstants.h \
	ledaeditorfactory.cpp \
        ledaeditorplugin.h \
        ledaeditorwidget.h
	
SOURCES = \
	ledadocument.cpp \
	ledaeditor.cpp \
	ledaeditorfactory.cpp \
	ledaeditorplugin.cpp \
	ledaeditorwidget.cpp
	
RESOURCES += leda.qrc

FORMS += ledaeditorwidget.ui
