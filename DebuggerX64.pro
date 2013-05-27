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
    Src/Gui/CustomTreeWidget.cpp

HEADERS  += \
    Src/Gui/MainWindow.h \
    Src/Gui/CPUSubWindow.h \
    Src/Gui/CPUWidget.h \
    Src/Gui/ColumnWidget.h \
    Src/Gui/CustomTableView.h \
    Src/Gui/CustomTreeWidget.h

INCLUDEPATH += \
    Src/Gui \

FORMS    += \
    Src/Gui/MainWindow.ui \
    Src/Dummy.ui \
    Src/Gui/CPUWidget.ui
