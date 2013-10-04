#-------------------------------------------------
#
# Project created by QtCreator 2013-05-20T13:22:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

#QMAKE_CXXFLAGS += -fpermissive

TARGET = DebuggerX64
#TEMPLATE = app
TEMPLATE = lib

DEFINES += BUILD_LIB

SOURCES += \
    Src/Gui/MainWindow.cpp \
    Src/main.cpp \
    Src/Gui/CPUWidget.cpp \
    Src/Memory/MapViewOfMem.cpp \
    Src/Disassembler/QBeaEngine.cpp \
    Src/DisassemblyView/AbstractTableView.cpp \
    Src/DisassemblyView/Disassembly.cpp \
    Src/DisassemblyView/HexDump.cpp \
    Src/Memory/ProcessMemoryMap.cpp \
    Src/Memory/MemoryPage.cpp \
    Src/Memory/MemoryCache.cpp \
    Src/Bridge/Bridge.cpp \
    Src/Imports.cpp \
    Src/Exports.cpp

HEADERS  += \
    Src/Gui/MainWindow.h \
    Src/Gui/CPUWidget.h \
    Src/Memory/MapViewOfMem.h \
    Src/Disassembler/QBeaEngine.h \
    Src/DisassemblyView/AbstractTableView.h \
    Src/DisassemblyView/Disassembly.h \
    Src/Global/NewTypes.h \
    Src/DisassemblyView/HexDump.h \
    Src/Memory/ProcessMemoryMap.h \
    Src/Memory/MemoryPage.h \
    Src/Memory/MemoryCache.h \
    Src/Bridge/Bridge.h \
    Src/Exports.h \
    Src/Imports.h \
    Src/main.h

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
LIBS += -L"$$PWD/Src/Bridge/" -lbridge.dll

INCLUDEPATH += $$PWD/Src/BeaEngine_s_64
INCLUDEPATH += $$PWD/Src/Bridge












