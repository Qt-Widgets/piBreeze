/*
	*
	* PBBookmarkInfo.hpp - PBBookmarkInfo.cpp header file
	*
*/

#pragma once

#include "Global.hpp"

class PBBookmarkInfo {

	public:
		QString displayLabel;
		QString displayIcon;
		QString mountPoint;

		static QList<PBBookmarkInfo> allBookmarks();
};

Q_DECLARE_METATYPE(PBBookmarkInfo);
