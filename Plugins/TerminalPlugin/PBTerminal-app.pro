TEMPLATE = app
TARGET = PBTermPlugin

# Qt5 QtWidgets
# =============
QT += widgets

# Plugin Mode
# ===========
# CONFIG += plugin

DEPENDPATH += . QTermWidget/lib
INCLUDEPATH += . QTermWidget/lib

LIBS += -lutil -lX11
DEFINES += STR_VERSION=\\\"1.0\\\"

# PBTerminal Sources
SOURCES += Main.cpp
SOURCES += PBTerminal.cpp
SOURCES += PBTermWidget.cpp
SOURCES += PBTSettingsDialog.cpp
SOURCES += PBTTools.cpp
SOURCES += TtyTabWidget.cpp

# PBTerminal Headers
HEADERS += Global.hpp
HEADERS += PBTerminal.hpp
HEADERS += PBTermWidget.hpp
HEADERS += PBTSettingsDialog.hpp
HEADERS += PBTTools.hpp
HEADERS += TtyTabWidget.hpp

# QTermWidget Sources
SOURCES += QTermWidget/lib/BlockArray.cpp
SOURCES += QTermWidget/lib/ColorScheme.cpp
SOURCES += QTermWidget/lib/Emulation.cpp
SOURCES += QTermWidget/lib/Filter.cpp
SOURCES += QTermWidget/lib/History.cpp
SOURCES += QTermWidget/lib/HistorySearch.cpp
SOURCES += QTermWidget/lib/KeyboardTranslator.cpp
SOURCES += QTermWidget/lib/konsole_wcwidth.cpp
SOURCES += QTermWidget/lib/kprocess.cpp
SOURCES += QTermWidget/lib/kpty.cpp
SOURCES += QTermWidget/lib/kptydevice.cpp
SOURCES += QTermWidget/lib/kptyprocess.cpp
SOURCES += QTermWidget/lib/Pty.cpp
SOURCES += QTermWidget/lib/qtermwidget.cpp
SOURCES += QTermWidget/lib/Screen.cpp
SOURCES += QTermWidget/lib/ScreenWindow.cpp
SOURCES += QTermWidget/lib/Session.cpp
SOURCES += QTermWidget/lib/ShellCommand.cpp
SOURCES += QTermWidget/lib/TerminalCharacterDecoder.cpp
SOURCES += QTermWidget/lib/TerminalDisplay.cpp
SOURCES += QTermWidget/lib/tools.cpp
SOURCES += QTermWidget/lib/Vt102Emulation.cpp

# QTermWidget Headers
HEADERS += QTermWidget/lib/BlockArray.h
HEADERS += QTermWidget/lib/CharacterColor.h
HEADERS += QTermWidget/lib/Character.h
HEADERS += QTermWidget/lib/ColorScheme.h
HEADERS += QTermWidget/lib/ColorTables.h
HEADERS += QTermWidget/lib/DefaultTranslatorText.h
HEADERS += QTermWidget/lib/Emulation.h
HEADERS += QTermWidget/lib/ExtendedDefaultTranslator.h
HEADERS += QTermWidget/lib/Filter.h
HEADERS += QTermWidget/lib/History.h
HEADERS += QTermWidget/lib/HistorySearch.h
HEADERS += QTermWidget/lib/KeyboardTranslator.h
HEADERS += QTermWidget/lib/konsole_wcwidth.h
HEADERS += QTermWidget/lib/kprocess.h
HEADERS += QTermWidget/lib/kptydevice.h
HEADERS += QTermWidget/lib/kpty.h
HEADERS += QTermWidget/lib/kpty_p.h
HEADERS += QTermWidget/lib/kptyprocess.h
HEADERS += QTermWidget/lib/LineFont.h
HEADERS += QTermWidget/lib/Pty.h
HEADERS += QTermWidget/lib/qtermwidget.h
HEADERS += QTermWidget/lib/Screen.h
HEADERS += QTermWidget/lib/ScreenWindow.h
HEADERS += QTermWidget/lib/Session.h
HEADERS += QTermWidget/lib/ShellCommand.h
HEADERS += QTermWidget/lib/TerminalCharacterDecoder.h
HEADERS += QTermWidget/lib/TerminalDisplay.h
HEADERS += QTermWidget/lib/tools.h
HEADERS += QTermWidget/lib/Vt102Emulation.h

# Build Location
# ==============
MOC_DIR 	= build/moc
OBJECTS_DIR = build/objs
RCC_DIR		= build/qrc
UI_DIR      = build/uic

# C++11 Support
# =============
QMAKE_CFLAGS += -std=c99
QMAKE_CXXFLAGS += -std=c++11

DEFINES += "HAVE_POSIX_OPENPT"
DEFINES += "HAVE_SYS_TIME_H"

RESOURCES += PBTerminal.qrc
CONFIG += silent
