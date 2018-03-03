/*
	*
	* PBXdg.hpp - PBXdg class header
	*
*/

#pragma once

#include "common.hpp"
#include "PBDeviceInfo.hpp"

class PBCOMMON_DLLSPEC PBXdg {
	public:
		enum XdgUserDir {
			XDG_DATA_HOME    = 0x02544878,
			XDG_CONFIG_HOME,
			XDG_CACHE_HOME
		};

		enum XdgSystemDirs {
			XDG_DATA_DIRS    = 0x196BB115,
			XDG_CONFIG_DIRS
		};

		static QString home();
		static QString xdgDefaultApp( QString );
		static QString userDir( PBXdg::XdgUserDir );
		static QStringList systemDirs( PBXdg::XdgSystemDirs );

		static QString trashLocation( QString path );
		static QString homeTrashLocation();
};
