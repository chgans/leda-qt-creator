import qbs.base 1.0
import "../../QtcTool.qbs" as QtcTool


QtcTool {
    name: "iossim"
    condition: qbs.targetOS.contains("osx")

    Depends { name: "Qt"; submodules: ["widgets"] }
    Depends { name: "app_version_header" }

    files: [
        "main.mm",
        "nsprintf.m",
        "nsstringexpandpath.m",
        "iphonesimulator.m",
        "iphonesimulator.h",
        "nsprintf.h",
        "nsstringexpandpath.h",
        "version.h",
        "iphonesimulatorremoteclient/iphonesimulatorremoteclient.h"
    ]
    cpp.linkerFlags: base.concat(["-sectcreate", "__TEXT", "__info_plist", path + "/Info.plist",
                                  "-fobjc-link-runtime"])
    cpp.frameworks: base.concat(["Foundation", "CoreServices", "ApplicationServices", "IOKit"])
    cpp.frameworkPaths: base.concat("/System/Library/PrivateFrameworks")

    toolInstallDir: project.ide_libexec_path + "/ios"
}
