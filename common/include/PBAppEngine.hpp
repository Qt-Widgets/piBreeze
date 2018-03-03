/*
	*
	* PBAppEngine.hpp - PBAppEngine.cpp header
	*
*/

#pragma once

#include "common.hpp"
#include "PBAppFile.hpp"
#include "PBXdg.hpp"

class PBCOMMON_DLLSPEC PBAppEngine {
	public:
		// Get a list of applications for a mime type given
		PBAppsList appsForMimeType( QMimeType );

		// Get a list of mimetypes an application handles, given the desktop name
		QStringList mimeTypesForApp( QString );

		// List all the applications
		PBAppsList allDesktops();

		//Get the consolidated/unified application file for a desktop name
		PBAppFile application( QString );

		// List the application locations
		QStringList appsLocations();

		// Add one new application location
		void addAppsLocations( QString );

		// Add multiple applications locations
		void addAppsLocations( QStringList );

		// Parse all desktops
		void parseDesktops();

		static PBAppEngine* instance();
		static void setApplicationAsDefault( QString, QString );

		PBAppFile xdgDefaultApp( QMimeType );

	private:
		// Constructor
		PBAppEngine();

		static PBAppEngine *globalInstance;

		// List files in location @arg 1
		QStringList listApplications( QString );

		QStringList appsDirs;

		PBAppsList appsList;
};
