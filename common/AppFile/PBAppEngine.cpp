/*
	*
	* PBAppEngine.cpp - PiBreeze Application Engine
	*
*/

#include "PBAppEngine.hpp"

inline QString findIn( QString what, QString where ) {

	QRegExp rx( what );
	rx.setMinimal( true );

	if ( rx.indexIn( where ) >= 0 )
		return rx.cap( 1 );

	return QString();
};

PBAppEngine *PBAppEngine::globalInstance = NULL;

PBAppsList PBAppEngine::appsForMimeType( QMimeType mimeType ) {

	PBAppsList appsForMimeList;
	QStringList mimeList = QStringList() << mimeType.name() << mimeType.allAncestors();

	Q_FOREACH( PBAppFile app, appsList.toQList() ) {
		Q_FOREACH( QString mime, mimeList ) {
			if ( app.value( PBAppFile::MimeTypes ).toStringList().contains( mime ) ) {
				if ( ( app.value( PBAppFile::Type ).toString() == "Application" ) and ( not app.value( PBAppFile::NoDisplay ).toBool() ) ) {
					if ( not appsForMimeList.contains( app ) ) {
						appsForMimeList << app;
					}
				}
			}
		}
	}

	QString defaultName = PBXdg::xdgDefaultApp( mimeType.name() );
	for( int i = 0; i < appsForMimeList.count(); i++ ) {
		if ( appsForMimeList.at( i ).desktopFileName() == baseName( defaultName ) ) {
			appsForMimeList.move( i, 0 );
			break;
		}
	}

	return appsForMimeList;
};

QStringList PBAppEngine::mimeTypesForApp( QString desktopName ) {

	QStringList mimeList;

	if ( not desktopName.endsWith( ".desktop" ) )
		desktopName += ".desktop";

	foreach( QString appDir, appsDirs ) {
		if ( QFile::exists( appDir + desktopName ) ) {
			mimeList << PBAppFile( appDir + desktopName ).value( PBAppFile::MimeTypes ).toStringList();
			break;
		}
	}

	return mimeList;
};

PBAppsList PBAppEngine::allDesktops() {

	return appsList;
};

PBAppFile PBAppEngine::application( QString exec ) {

	PBAppsList list;
	Q_FOREACH( PBAppFile app, appsList.toQList() ) {
		if ( app.value( PBAppFile::Exec ).toString().contains( exec, Qt::CaseInsensitive ) )
			list << app;

		else if ( app.value( PBAppFile::Name ).toString().compare( exec, Qt::CaseInsensitive ) == 0 )
			list << app;
	};

	if ( not list.count() )
		return PBAppFile();

	PBAppFile merged = list.toQList().at( 0 );
	Q_FOREACH( PBAppFile other, list.toQList() )
		merged.merge( other );

	return merged;
};

void PBAppEngine::parseDesktops() {

	appsList.clear();
	foreach( QString appDir, appsDirs ) {
		foreach( QString application, listApplications( appDir ) ) {

			appsList << PBAppFile( appDir + application );
		}
	}
};

PBAppEngine* PBAppEngine::instance() {

	if ( PBAppEngine::globalInstance )
		return globalInstance;

	PBAppEngine::globalInstance = new PBAppEngine();
	globalInstance->parseDesktops();

	return PBAppEngine::globalInstance;
};

void PBAppEngine::setApplicationAsDefault( QString appFileName, QString mimetype ) {

	if ( QProcess::execute( "xdg-mime", QStringList() << "default" << appFileName << mimetype ) )
		qDebug() << "Error while setting" << appFileName << "as the default handler for" << mimetype;
};

PBAppEngine::PBAppEngine() {

	appsDirs << QDir::home().filePath( ".local/share/applications/" );
	appsDirs << "/usr/local/share/applications/" << "/usr/share/applications/";
	appsDirs << "/usr/share/applications/kde4/" << "/usr/share/gnome/applications/";
};

PBAppFile PBAppEngine::xdgDefaultApp( QMimeType mimeType ) {

	return appsForMimeType( mimeType ).value( 0 );
};

QStringList PBAppEngine::listApplications( QString appsDir ) {

	QStringList applications;

	DIR *dir;
	struct dirent *ent;
	dir = opendir( appsDir.toLocal8Bit().data() );

	if ( dir != NULL ) {
		while ( ( ent = readdir( dir ) ) != NULL) {
			QString nodeName = QString::fromLocal8Bit( ent->d_name );
			if ( ( nodeName.compare( "." ) == 0 ) or ( nodeName.compare( ".." ) == 0 ) )
				continue;

			if ( nodeName.endsWith( ".desktop" ) )
				applications << nodeName;
		}

		closedir( dir );
	}

	return applications;
};
