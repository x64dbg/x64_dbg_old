#-------------------------------------------------
#
# Project created by QtCreator 2013-05-20T13:22:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DebuggerX64
TEMPLATE = app


SOURCES += \
    Src/Gui/MainWindow.cpp \
    Src/main.cpp \
    Src/Gui/CPUSubWindow.cpp \
    Src/Gui/CPUWidget.cpp \
    Src/Gui/ColumnWidget.cpp \
    Src/Gui/CustomTableView.cpp \
    Src/Gui/CustomTreeWidget.cpp \
    Src/DisassemblyView/DisassemblyView.cpp \
    Src/Memory/MapViewOfMem.cpp \
    Src/Memory/test.cpp \
    Src/Disassembler/QBeaEngine.cpp

HEADERS  += \
    Src/Gui/MainWindow.h \
    Src/Gui/CPUSubWindow.h \
    Src/Gui/CPUWidget.h \
    Src/Gui/ColumnWidget.h \
    Src/Gui/CustomTableView.h \
    Src/Gui/CustomTreeWidget.h \
    Src/DisassemblyView/DisassemblyView.h \
    Src/Memory/MapViewOfMem.h \
    Src/Memory/test.h \
    Src/Disassembler/QBeaEngine.h

INCLUDEPATH += \
    Src/Gui \
    Src/DisassemblyView \
    Src/Memory \
    Src/BeaEngine/headers \
    Src/Disassembler \
    Src/BeaEngine

FORMS    += \
    Src/Gui/MainWindow.ui \
    Src/Dummy.ui \
    Src/Gui/CPUWidget.ui


LIBS += -L"$$PWD/Src/BeaEngine/" -lBeaEngine_s

INCLUDEPATH += $$PWD/Src/BeaEngine_s
