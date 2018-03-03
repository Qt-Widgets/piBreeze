/*
	*
	* PBFileInfoGatherer.hpp - PiBreeze FileInfoGatherer Class Header
	*
*/

#pragma once

#include "Global.hpp"

class PBQuickFileInfoGatherer {

	public:
		PBQuickFileInfoGatherer();
		QVariantList getQuickFileInfo( QString path );

	private:
		QString permStr( mode_t fileMode );
		QString formatSize( qint64 num );

		QSettings *fileTypes;
};
