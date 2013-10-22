#-------------------------------------------------
#
# Project created by QtCreator 2013-05-20T13:22:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Removes all debug output when defined
DEFINES += QT_NO_DEBUG_OUTPUT
DEFINES += QT_NO_DEBUG_STREAM #no debug output in release mode (finally)

#QMAKE_CFLAGS_RELEASE += -O3
#QMAKE_CXXFLAGS_RELEASE += -O3


!contains(QMAKE_HOST.arch, x86_64) {
    TARGET = x32_gui
} else {
    TARGET = x64_gui
}

DEFINES += BUILD_LIB
TEMPLATE = lib
#TEMPLATE = app



SOURCES += \
    Src/main.cpp \
    Src/Gui/MainWindow.cpp \
    Src/Gui/CPUWidget.cpp \
    Src/Gui/CommandLineEdit.cpp \
    Src/BasicView/Disassembly.cpp \
    Src/BasicView/HexDump.cpp \
    Src/BasicView/AbstractTableView.cpp \
    Src/Disassembler/QBeaEngine.cpp \
    Src/Memory/ProcessMemoryMap.cpp \
    Src/Memory/MemoryPage.cpp \
    Src/Memory/MapViewOfMem.cpp \
    Src/Bridge/Bridge.cpp \
    Src/BasicView/StdTable.cpp \
    Src/BasicView/MemoryMapView.cpp \
    Src/BasicView/LogView.cpp \
    Src/Gui/GotoDialog.cpp


HEADERS += \
    Src/main.h \
    Src/Gui/MainWindow.h \
    Src/Gui/CPUWidget.h \
    Src/Gui/CommandLineEdit.h \
    Src/BasicView/Disassembly.h \
    Src/BasicView/HexDump.h \
    Src/BasicView/AbstractTableView.h \
    Src/Disassembler/QBeaEngine.h \
    Src/Memory/ProcessMemoryMap.h \
    Src/Memory/MemoryPage.h \
    Src/Memory/MapViewOfMem.h \
    Src/Bridge/Bridge.h \
    Src/Global/NewTypes.h \
    Src/Exports.h \
    Src/Imports.h \
    Src/BasicView/StdTable.h \
    Src/BasicView/MemoryMapView.h \
    Src/BasicView/LogView.h \
    Src/Gui/GotoDialog.h


INCLUDEPATH += \
    Src \
    Src/Gui \
    Src/BasicView \
    Src/Disassembler \
    Src/BeaEngine \
    Src/ThirdPartyLibs/BeaEngine \
    Src/Memory \
    Src/Bridge \
    Src/Global


FORMS += \
    Src/Gui/MainWindow.ui \
    Src/Dummy.ui \
    Src/Gui/CPUWidget.ui \
    Src/Gui/GotoDialog.ui


#INCLUDEPATH += $$PWD/Src/BeaEngine_s_64
INCLUDEPATH += $$PWD/Src/Bridge



!contains(QMAKE_HOST.arch, x86_64) {
    message("x86 build")
    LIBS += -L"$$PWD/Src/ThirdPartyLibs/BeaEngine/" -lBeaEngine_s
    LIBS += -L"$$PWD/Src/Bridge/" -lx32_bridge
    ## Windows x86 (32bit) specific build here
} else {
    message("x86_64 build")
    LIBS += -L"$$PWD/Src/ThirdPartyLibs/BeaEngine/" -lBeaEngine_s_64
    LIBS += -L"$$PWD/Src/Bridge/" -lx64_bridge
    ## Windows x64 (64bit) specific build here
}

RESOURCES += \
    resource.qrc








