/*
	*
	* PBStartup.cpp - PiBreeze Startup Configurator
	*
*/

#include "Global.hpp"
#include "PBPluginManager.hpp"
#include "PBMessageDialog.hpp"
#include "PBBugReporter.hpp"

void PBStartup() {

	/*
		*
		* Set the Icon Theme
		*
	*/
	QIcon::setThemeName( Settings->General.IconTheme );

	/*
		*
		* Application style
		*
	*/
	qApp->setStyle( QStyleFactory::create( Settings->General.Style ) );

	/*
		*
		* Set hard coded CustomActions: Open as SU, Open in terminal.
		*
	*/
	if ( actionSettings.value( "OpenAsSuperUser/Name" ).toString().isEmpty() ) {
		actionSettings.setValue( "OpenAsSuperUser/Name", "Open as SuperUser" );
		actionSettings.setValue( "OpenAsSuperUser/Exec", "sudo xdg-open %f" );
		actionSettings.setValue( "OpenAsSuperUser/Glob", QStringList() << "*" );
	}

	if ( actionSettings.value( "OpenInTerminal" ).toString().isEmpty() ) {
		actionSettings.setValue( "OpenInTerminal/Name", "Open In Terminal" );
		actionSettings.setValue( "OpenInTerminal/Exec", "%term" );
		actionSettings.setValue( "OpenInTerminal/Glob", QStringList() << "folder" );
	}

	/*
		*
		* Detect and save the terminals available
		*
	*/

	QSettings settings( "PiBreeze", "PiBreeze" );
	if ( settings.value( "Terminals/nbterminal" ).toString().isEmpty() ) {
		QStringList paths = QString( getenv( "PATH" ) ).split( ":" );
		foreach( QString path, paths ) {
			if ( QFileInfo( path + "/nbterminal" ).exists() ) {
				QStringList termInfo = QStringList() << "nbterminal" << "--workdir" << "%1" << "-e" << "%2";
				settings.setValue( "Terminals/nbterminal", QVariant( termInfo ) );
				break;
			}
		}
	}

	if ( settings.value( "Terminals/qterminal" ).toString().isEmpty() ) {
		QStringList paths = QString( getenv( "PATH" ) ).split( ":" );
		foreach( QString path, paths ) {
			if ( QFileInfo( path + "/qterminal" ).exists() ) {
				QStringList termInfo = QStringList() << "qterminal" << "--workdir" << "%1" << "-e" << "%2";
				settings.setValue( "Terminals/qterminal", QVariant( termInfo ) );
				break;
			}
		}
	}

	if ( settings.value( "Terminals/konsole" ).toString().isEmpty() ) {
		QStringList paths = QString( getenv( "PATH" ) ).split( ":" );
		foreach( QString path, paths ) {
			if ( QFileInfo( path + "/konsole" ).exists() ) {
				QStringList termInfo = QStringList() << "konsole" << "--workdir" << "%1" << "-e" << "%2";
				settings.setValue( "Terminals/konsole", QVariant( termInfo ) );
				break;
			}
		}
	}

	if ( settings.value( "Terminals/gnome-terminal" ).toString().isEmpty() ) {
		QStringList paths = QString( getenv( "PATH" ) ).split( ":" );
		foreach( QString path, paths ) {
			if ( QFileInfo( path + "/gnome-terminal" ).exists() ) {
				QStringList termInfo = QStringList() << "gnome-terminal" << "--working-directory" << "%1" << "-x" << "%2";
				settings.setValue( "Terminals/gnome-terminal", QVariant( termInfo ) );
				break;
			}
		}
	}

	if ( settings.value( "Terminals/mate-terminal" ).toString().isEmpty() ) {
		QStringList paths = QString( getenv( "PATH" ) ).split( ":" );
		foreach( QString path, paths ) {
			if ( QFileInfo( path + "/mate-terminal" ).exists() ) {
				QStringList termInfo = QStringList() << "mate-terminal" << "--working-directory" << "%1" << "-x" << "%2";
				settings.setValue( "Terminals/mate-terminal", QVariant( termInfo ) );
				break;
			}
		}
	}

	if ( settings.value( "Terminals/xfce4-terminal" ).toString().isEmpty() ) {
		QStringList paths = QString( getenv( "PATH" ) ).split( ":" );
		foreach( QString path, paths ) {
			if ( QFileInfo( path + "/xfce4-terminal" ).exists() ) {
				QStringList termInfo = QStringList() << "xfce4-terminal" << "--working-directory" << "%1" << "-x" << "%2";
				settings.setValue( "Terminals/xfce4-terminal", QVariant( termInfo ) );
				break;
			}
		}
	}

	if ( settings.value( "Terminals/lxterminal" ).toString().isEmpty() ) {
		QStringList paths = QString( getenv( "PATH" ) ).split( ":" );
		foreach( QString path, paths ) {
			if ( QFileInfo( path + "/lxterminal" ).exists() ) {
				QStringList termInfo = QStringList() << "lxterminal" << "--working-directory" << "%1" << "-e" << "%2";
				settings.setValue( "Terminals/lxterminal", QVariant( termInfo ) );
				break;
			}
		}
	}

	if ( settings.value( "Terminals/xterm" ).toString().isEmpty() ) {
		QStringList paths = QString( getenv( "PATH" ) ).split( ":" );
		foreach( QString path, paths ) {
			if ( QFileInfo( path + "/xterm" ).exists() ) {
				settings.setValue( "Terminals/xterm", QVariant( QStringList() << "xterm" << "-e" << "cd %1 && %2" ) );
				break;
			}
		}
	}

	/*
		*
		* QFont Substitutions - DejaVu Sans Mono is used for addressbar, if not found use courier, or mono
		*
	*/

	QFont::insertSubstitutions( "DejaVu Sans Mono", QStringList() << "Courier" << "Courier New" << "Courier 10 Pitch" << "mono" );

	/*
		*
		* Init the plugins - only if we are not the root user
		*
	*/
	if ( getuid() )
		QTimer::singleShot( 0, PBPluginManager::instance(), SLOT( reloadPlugins() ) );
};
