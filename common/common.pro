TEMPLATE = lib
TARGET = pibreeze

VERSION = 1.0.0

lessThan( QT_MAJOR_VERSION, 5 ) {
	error( "The minimum expected version of Qt to compile this software is 5.0.0" )
}

INCLUDEPATH += . include AppFile DeviceInfo QCryptographicHash5 SimpleCrypt SystemInfo Tools XDG
DEPENDPATH += . include AppFile DeviceInfo QCryptographicHash5 SimpleCrypt SystemInfo Tools XDG

DEFINES += COMMON

# Headers
# =======
HEADERS += include/common.hpp
HEADERS += PBPluginInterface.hpp
HEADERS += include/PBAppEngine.hpp
HEADERS += include/PBAppFile.hpp
HEADERS += include/PBDeviceInfo.hpp
HEADERS += include/PBSystemInfo.hpp
HEADERS += include/PBTools.hpp
HEADERS += include/PBXdg.hpp
HEADERS += include/SimpleCrypt.hpp

# Sources
# =======
SOURCES += AppFile/PBAppEngine.cpp
SOURCES += AppFile/PBAppFile.cpp
SOURCES += DeviceInfo/PBDeviceInfo.cpp
SOURCES += DeviceInfo/PBDeviceInfoUnix.cpp
SOURCES += SimpleCrypt/SimpleCrypt.cpp
SOURCES += SystemInfo/PBSystemInfo.cpp
SOURCES += Tools/PBTools.cpp
SOURCES += XDG/PBXdg.cpp

# Optional Files
# ==============
isEqual(QT_MAJOR_VERSION, 5) {
	lessThan(QT_MINOR_VERSION, 1) {
		# CryptographicHash Sha3 from Qt5 v5.1
		HEADERS += include/QCryptographicHash5.hpp
		SOURCES += QCryptographicHash5/QCryptographicHash5.cpp
	}
}

# Silent compilation
# ==================
CONFIG += silent

# Build location
# ==============
BUILD_PREFIX = $$(PB_BUILD_DIR)
isEmpty( BUILD_PREFIX ) {
	BUILD_PREFIX = ./build
}

MOC_DIR 	= $$BUILD_PREFIX/moc-common5
OBJECTS_DIR = $$BUILD_PREFIX/obj-common5
RCC_DIR		= $$BUILD_PREFIX/qrc-common5
UI_DIR      = $$BUILD_PREFIX/uic-common5

# Build Shared and Static
# =======================
CONFIG += shared_and_static build_all

unix {
	isEmpty(PREFIX) {
		PREFIX = /usr
	}

	INSTALLS += target static header
	target.path = $$PREFIX/lib/pibreeze

	header.path = $$PREFIX/include/pibreeze
	header.files = $$HEADERS include/pibreeze.hpp PBPluginInterface.hpp

	static.path = $$PREFIX/lib/pibreeze
	static.files = libpibreeze.a

	static.path = $$PREFIX/lib/pibreeze
	static.files = libpibreeze.a
}
