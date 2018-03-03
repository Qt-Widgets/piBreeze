/*
	*
	* PBIconManager.hpp - Icon Manager for PiBreeze
	*
*/

#pragma once

#include "Global.hpp"

typedef QMap<QString, QString> DeviceInfo;
typedef QPair<QString, QString> BookMark;

class PBIconManager {

	public:
		/* Unique single instance */
		static PBIconManager *instance();

		/* Icon string for file type: @mName: Mime Name */
		QStringList iconsForFile( QString mName, QString file = QString() );

		/* Icon string for file type: @mName: Mime Name */
		QStringList icon( QString iName );

		/* Icon string for file type: @mName: Icon Name */
		bool hasIcon( QString iName );

		/* Thumbnail for an image file */
		QString imageThumbnail( QString );

	private:
		/* Private init function */
		PBIconManager();

		/* Generate theme database */
		void generateThemeDatabase();

		/* Global Instance */
		static PBIconManager *iMgr;
		bool init;

		/* Default theme location */
		QString iconThemePath;
};
