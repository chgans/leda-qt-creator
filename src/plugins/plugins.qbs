import qbs

Project {
    name: "Plugins"

    references: [
//        "analyzerbase/analyzerbase.qbs",
//        "android/android.qbs",
//        "autotoolsprojectmanager/autotoolsprojectmanager.qbs",
//        "baremetal/baremetal.qbs",
//        "bazaar/bazaar.qbs",
//        "beautifier/beautifier.qbs",
        "bineditor/bineditor.qbs",
//        "bookmarks/bookmarks.qbs",
//        "clangcodemodel/clangcodemodel.qbs",
//        "classview/classview.qbs",
//        "clearcase/clearcase.qbs",
//        "cmakeprojectmanager/cmakeprojectmanager.qbs",
        "coreplugin/coreplugin.qbs",
        "coreplugin/images/logo/logo.qbs",
//        "cpaster/cpaster.qbs",
//        "cpaster/frontend/frontend.qbs",
//        "cppeditor/cppeditor.qbs",
//        "cpptools/cpptools.qbs",
//        "cvs/cvs.qbs",
//        "debugger/debugger.qbs",
//        "debugger/ptracepreload.qbs",
//        "designer/designer.qbs",
//        "diffeditor/diffeditor.qbs",
//        "fakevim/fakevim.qbs",
//        "emacskeys/emacskeys.qbs",
//        "genericprojectmanager/genericprojectmanager.qbs",
//        "git/git.qbs",
//        "glsleditor/glsleditor.qbs",
//        "helloworld/helloworld.qbs",
//        "help/help.qbs",
        "imageviewer/imageviewer.qbs",
//        "ios/ios.qbs",
        "leda/leda.qbs",
//        "macros/macros.qbs",
//        "mercurial/mercurial.qbs",
//        "perforce/perforce.qbs",
        "projectexplorer/projectexplorer.qbs",
//        "qbsprojectmanager/qbsprojectmanager.qbs",
//        "qmakeandroidsupport",
//        "pythoneditor/pythoneditor.qbs",
//        "qmldesigner/qmldesigner.qbs",
//        "qmljseditor/qmljseditor.qbs",
//        "qmljstools/qmljstools.qbs",
//        "qmlprofiler/qmlprofiler.qbs",
//        "qmlprojectmanager/qmlprojectmanager.qbs",
//        "qnx/qnx.qbs",
//        "qmakeprojectmanager/qmakeprojectmanager.qbs",
//        "qtsupport/qtsupport.qbs",
//        "remotelinux/remotelinux.qbs",
//        "resourceeditor/resourceeditor.qbs",
//        "subversion/subversion.qbs",
        "tasklist/tasklist.qbs",
        "texteditor/texteditor.qbs",
//        "todo/todo.qbs",
        "updateinfo/updateinfo.qbs",
//        "valgrind/valgrind.qbs",
//        "vcsbase/vcsbase.qbs",
//        "welcome/welcome.qbs",
//        "winrt/winrt.qbs"
    ].concat(project.additionalPlugins)
}
