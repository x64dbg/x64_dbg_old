#-------------------------------------------------
#
# Project created by QtCreator 2013-05-20T13:22:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Removes all debug output when defined
#DEFINES += QT_NO_DEBUG_OUTPUT


TARGET = DebuggerX64

DEFINES += BUILD_LIB
TEMPLATE = lib
#TEMPLATE = app



SOURCES += \
    Src/Gui/MainWindow.cpp \
    Src/main.cpp \
    Src/Gui/CPUWidget.cpp \
    Src/Disassembler/QBeaEngine.cpp \
    Src/DisassemblyView/AbstractTableView.cpp \
    Src/DisassemblyView/Disassembly.cpp \
    Src/DisassemblyView/HexDump.cpp \
    Src/Memory/ProcessMemoryMap.cpp \
    Src/Memory/MemoryPage.cpp \
    Src/Bridge/Bridge.cpp \
    Src/Memory/MapViewOfMem.cpp \
    Src/Gui/LineEdit.cpp

HEADERS  += \
    Src/Gui/MainWindow.h \
    Src/Gui/CPUWidget.h \
    Src/Disassembler/QBeaEngine.h \
    Src/DisassemblyView/AbstractTableView.h \
    Src/DisassemblyView/Disassembly.h \
    Src/Global/NewTypes.h \
    Src/DisassemblyView/HexDump.h \
    Src/Memory/ProcessMemoryMap.h \
    Src/Memory/MemoryPage.h \
    Src/Bridge/Bridge.h \
    Src/Exports.h \
    Src/Imports.h \
    Src/main.h \
    Src/Memory/MapViewOfMem.h \
    Src/Gui/LineEdit.h

INCLUDEPATH += \
    Src \
    Src/Gui \
    Src/DisassemblyView \
    Src/Memory \
    Src/BeaEngine/headers \
    Src/Disassembler \
    Src/BeaEngine \
    Src/Global \
    Src/Bridge

FORMS    += \
    Src/Gui/MainWindow.ui \
    Src/Dummy.ui \
    Src/Gui/CPUWidget.ui


LIBS += -L"$$PWD/Src/BeaEngine/" -lBeaEngine_s_64
LIBS += -L"$$PWD/Src/Bridge/" -lx64_bridge

#INCLUDEPATH += $$PWD/Src/BeaEngine_s_64
INCLUDEPATH += $$PWD/Src/Bridge



    !contains(QMAKE_HOST.arch, x86_64) {
        message("x86 build")

        ## Windows x86 (32bit) specific build here

    } else {
        message("x86_64 build")

        ## Windows x64 (64bit) specific build here

    }








