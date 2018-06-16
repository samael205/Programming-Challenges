import qbs

Project {
    minimumQbsVersion: "1.7.1"

    CppApplication {
        Depends { name: "Qt.core" }
        Depends { name: "Qt.quick" }
        property pathList qmlImportPaths: []

        cpp.cxxLanguageVersion: "c++11"

        cpp.defines: [
            "QT_DEPRECATED_WARNINGS",

        ]

        files: [
            "main.cpp",
            "qml.qrc",
        ]

        Group {   
            fileTagsFilter: "application"
            qbs.install: true
        }
    }
}
