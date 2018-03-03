TEMPLATE = app
TARGET = PBVault

QT += widgets

INCLUDEPATH += . ../../common/include EncFS KeyDialog Salsa20

Qt += widgets
LIBS += -L../../common/ -lpibreeze

# Input
HEADERS += PBCircle.hpp
HEADERS += PBVault.hpp
HEADERS += PBVaultDatabase.hpp
HEADERS += EncFS/PBEncFS.hpp
HEADERS += KeyDialog/PBPasswordDialog.hpp
HEADERS += KeyDialog/PBPasswordEdit.hpp
HEADERS += KeyDialog/PBPasswordInput.hpp
HEADERS += KeyDialog/PBPatternPad.hpp
HEADERS += Salsa20/PBSalsa20.hpp
HEADERS += Salsa20/Salsa20.hpp

SOURCES += PBCircle.cpp
SOURCES += Main.cpp
SOURCES += PBVault.cpp
SOURCES += PBVaultDatabase.cpp
SOURCES += KeyDialog/PBPasswordDialog.cpp
SOURCES += KeyDialog/PBPasswordEdit.cpp
SOURCES += KeyDialog/PBPasswordInput.cpp
SOURCES += KeyDialog/PBPatternPad.cpp
SOURCES += EncFS/PBEncFS.cpp
SOURCES += Salsa20/PBSalsa20.cpp

CONFIG += silent

unix {
	isEmpty(PREFIX) {
		PREFIX = /usr
	}
	BINDIR = $$PREFIX/bin

	INSTALLS += target
	target.path = $$BINDIR

	QMAKE_RPATHDIR += $$PREFIX/lib/pibreeze/
}

# Build location

BUILD_PREFIX = $$(PB_BUILD_DIR)

isEmpty( BUILD_PREFIX ) {
	BUILD_PREFIX = ./build
}

MOC_DIR 	= $$BUILD_PREFIX/moc-vault5
OBJECTS_DIR = $$BUILD_PREFIX/obj-vault5
RCC_DIR		= $$BUILD_PREFIX/qrc-vault5
UI_DIR      = $$BUILD_PREFIX/uic-vault5

DEFINES += STANDALONE
