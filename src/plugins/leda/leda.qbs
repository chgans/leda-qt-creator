import qbs 1.0

QtcPlugin {
    name: "LedaEditor"

    Depends { name: "Qt.widgets" }
    Depends { name: "Aggregation" }
    Depends { name: "Utils" }

    Depends { name: "Core" }

    files: [
        "leda.qrc",
        "ledadocument.cpp",
        "ledadocument.h",
        "ledaeditor.cpp",
        "ledaeditor.h",
        "ledaeditorconstants.h",
        "ledaeditorfactory.cpp",
        "ledaeditorfactory.h",
        "ledaeditorplugin.cpp",
        "ledaeditorplugin.h",
        "ledaeditorwidget.cpp",
        "ledaeditorwidget.h",
        "ledaeditorwidget.ui",
    ]
}

