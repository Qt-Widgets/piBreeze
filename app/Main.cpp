/*
	*
	* Copyright 2018 Britanicus <marcusbritanicus@gmail.com>
	*

	*
	* This program is free software; you can redistribute it and/or modify
	* it under the terms of the GNU General Public License as published by
	* the Free Software Foundation; either version 3 of the License, or
	* (at your option) any later version.
	*

	*
	* This program is distributed in the hope that it will be useful,
	* but WITHOUT ANY WARRANTY; without even the implied warranty of
	* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	* GNU General Public License for more details.
	*

	*
	* You should have received a copy of the GNU General Public License
	* along with this program; if not, write to the Free Software
	* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
	* MA 02110-1301, USA.
	*
*/

#include "Global.hpp"

#include "PiBreeze.hpp"

#include "PBMessageDialog.hpp"
#include "AboutPB.hpp"
#include "PBSettingsManager.hpp"
#include "PiBreezeTray.hpp"

#include "PBLogger.hpp"
#include "PBCLParser.hpp"

QString lockFile = QString( "/tmp/PiBreeze-%1.lock" ).arg( geteuid() );

void killApp() {

	int pid = -1;
	bool procRunning = true;

	// check for the lock file and see if its stale.
	if ( QFileInfo( lockFile ).exists() ) {

		// OK, lock exists. Now, check if it is stale!
		QFile pidFile( lockFile );
		pidFile.open( QFile::ReadOnly );
		pid = pidFile.readAll().toInt();
		pidFile.close();
	}

	kill( pid, SIGKILL );
};

bool isLocked() {

	int pid = -1;
	bool procRunning = true;

	// check for the lock file and see if its stale.
	if ( QFileInfo( lockFile ).exists() ) {

		// OK, lock exists. Now, check if it is stale!
		QFile pidFile( lockFile );
		pidFile.open( QFile::ReadOnly );
		pid = pidFile.readAll().toInt();
		pidFile.close();

		// if /proc/pid doesnot exist, we have a stale lock
		if( not QFileInfo( QString( "/proc/%1" ).arg( pid ) ).exists() ) {
			procRunning = false;
			QFile::remove( lockFile );
		}
	}

	else {

		procRunning = false;
	}

	if ( procRunning )
		return true;

	return false;
};

void lockApp() {
	/*
		*
		* lockApp() -> None
		*
		* Lock app to one instance
		*
		* @return None
		*
	*/

	QFile pidFile( lockFile );
	pidFile.open( QFile::WriteOnly );
	pidFile.write( qPrintable( QString::number( qApp->applicationPid() ) ) );
	pidFile.close();
};

int main( int argc, char **argv ) {

	qInstallMessageHandler( PBMessageOutput5 );

	qRegisterMetaType<PBDeviceInfo>( "PBDeviceInfo" );

	/* Necessary when using QWebEngine from plugins */
	#if QT_VERSION >= 0x050500
		QCoreApplication::setAttribute( Qt::AA_ShareOpenGLContexts );
	#endif

	QApplication app( argc, argv );

	/* About Application */
	app.setOrganizationName( "PiBreeze" );
	app.setApplicationName( "PiBreeze" );

	/* Startup */
	PBStartup();

	if ( Settings->General.TrayIcon )
		app.setQuitOnLastWindowClosed( false );

	else
		app.setQuitOnLastWindowClosed( true );

	/* Arg Parser */
	switch( PBArgParser( argc, argv ) ) {

		/* We want only the server running in the tray */
		case SYSTRAY : {
			/* If an instance is already running, inform the user about it. */
			if ( isLocked() ) {
				qDebug() << "Found a running instance. Exiting";
				PBMessageDialog::information(
					NULL,
					"PiBreeze - Open in tray",
					"PiBreeze is now open in SystemTray. Left click to show/hide all the open windows, Double click to open a new window, middle click to quit."
				);
				return 0;
			}

			/* Else, start an instance. */
			else {
				qDebug() << "No running instance of the PiBreeze found";

				/* Here we do not request the tray icon as we directly open it */
				PiBreeze *Gui = new PiBreeze( QString() );

				PBTrayIcon* trayIcon = new PBTrayIcon();
				trayIcon->show();

				QObject::connect( trayIcon, SIGNAL( newWindow() ), Gui, SLOT( newWindow() ) );

				return app.exec();
			}

			break;
		}

		/* We want only the settings */
		case SETTINGS : {
			qDebug() << "Showing Settings Dialog";
			PBSettingsManager *settingsMgr = new PBSettingsManager();
			settingsMgr->exec();

			return 0;
		}

		/* We want only the about dialog */
		case ABOUTPB : {
			qDebug() << "Showing About PB dialog";
			AboutPB *aboutPB = new AboutPB();
			aboutPB->exec();

			return 0;
		}

		/* We want only the about dialog */
		case LICENSE : {
			qDebug() << "Showing PiBreeze License";
			License *license = new License();
			license->exec();

			return 0;
		}

		/* A forced opening */
		case FORCENEW : {
			if ( isLocked() ) {
				qCritical( "Killing the existing server" );
				qWarning( "This may have caused data loss. Be warned." );

				killApp();
			}

			lockApp();
			PiBreeze *Gui;
			if ( app.arguments().count() >= 3 )
				Gui = new PiBreeze( app.arguments().at( 2 ) );

			else
				Gui = new PiBreeze( QString() );

			if ( Settings->Session.Maximized )
				Gui->showMaximized();

			else
				Gui->showNormal();

			if ( Settings->General.TrayIcon ) {
				PBTrayIcon* trayIcon = new PBTrayIcon();
				trayIcon->show();

				QObject::connect( trayIcon, SIGNAL( newWindow() ), Gui, SLOT( newWindow() ) );
			}

			return app.exec();
		}

		/* A normal opening */
		case NORMAL : {
			/* If the server is running */
			if ( isLocked() ) {
				qDebug() << "Found a running instance. Exiting";
				PBMessageDialog::information(
					NULL,
					"PiBreeze - Open in tray",
					"PiBreeze is now open in SystemTray. Left click to show/hide all the open windows, Double click to open a new window, middle click to quit."
				);

				return 0;
			}

			/* Server not running */
			else {
				lockApp();

				PiBreeze *Gui;
				if ( app.arguments().count() >= 2 )
					Gui = new PiBreeze( app.arguments().at( 1 ) );

				else
					Gui = new PiBreeze( QString() );

				if ( Settings->Session.Maximized )
					Gui->showMaximized();

				else
					Gui->showNormal();

				if ( Settings->General.TrayIcon ) {
					PBTrayIcon* trayIcon = new PBTrayIcon();
					trayIcon->show();

					QObject::connect( trayIcon, SIGNAL( newWindow() ), Gui, SLOT( newWindow() ) );
				}

				return app.exec();
			}
		}
	}
};
