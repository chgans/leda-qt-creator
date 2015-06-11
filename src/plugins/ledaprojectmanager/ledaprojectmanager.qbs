import qbs 1.0
import qbs.FileInfo

QtcPlugin {
    name: "LedaProjectManager"

    Depends { name: "Qt.widgets" }
    Depends { name: "Utils" }

    Depends { name: "Core" }
    Depends { name: "TextEditor" }
    Depends { name: "ProjectExplorer" }

    files: [
        "filesselectionwizardpage.cpp",
        "filesselectionwizardpage.h",
        "ledabuildconfiguration.cpp",
        "ledabuildconfiguration.h",
        "ledamakestep.cpp",
        "ledamakestep.h",
        "ledamakestep.ui",
        "ledaproject.cpp",
        "ledaproject.h",
        "ledaproject.qrc",
        "ledaprojectconstants.h",
        "ledaprojectfileseditor.cpp",
        "ledaprojectfileseditor.h",
        "ledaprojectmanager.cpp",
        "ledaprojectmanager.h",
        "ledaprojectnodes.cpp",
        "ledaprojectnodes.h",
        "ledaprojectplugin.cpp",
        "ledaprojectplugin.h",
        "ledaprojectwizard.cpp",
        "ledaprojectwizard.h",
    ]
}
