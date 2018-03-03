/*
	*
	* PiBreezeTray.cpp - PiBreeze Tray Icon Class
	*
*/

#include "PiBreezeTray.hpp"

PBTrayIcon::PBTrayIcon() : QSystemTrayIcon() {

	setIcon( QIcon( ":/icons/pibreeze.png" ) );
	connect( this, SIGNAL( activated( QSystemTrayIcon::ActivationReason ) ), this, SLOT( handleActivation( QSystemTrayIcon::ActivationReason ) ) );

	QMenu *menu = new QMenu( "TrayMenu" );
	menu->addAction( QIcon( ":/icons/pibreeze.png" ), "New &Window", this, SIGNAL( newWindow() ) );
	menu->addAction( "&Toggle Visible Windows", this, SLOT( toggleVisible() ) );
	menu->addAction( QIcon( ":/icons/delete.png" ), "&Quit PiBreeze", this, SLOT( quit() ) );
	setContextMenu( menu );
};

void PBTrayIcon::handleActivation( QSystemTrayIcon::ActivationReason reason ) {

	switch( reason ) {
		case PBTrayIcon::Context: {
			break;
		};

		case PBTrayIcon::MiddleClick: {
			qApp->quit();
			break;
		};

		case PBTrayIcon::Trigger: {
			toggleVisible();
			break;
		}

		case PBTrayIcon::DoubleClick: {
			emit newWindow();
			break;
		};

		default:{
			break;
		};
	};
};

void PBTrayIcon::toggleVisible() {

	bool visible = true;
	Q_FOREACH( QWidget *nb, qApp->topLevelWidgets() ) {
		if ( qobject_cast<PiBreeze*>( nb ) ) {
			if ( nb->isVisible() )
				visible &= true;

			else
				visible &= false;
		}
	}

	/* All PiBreeze instances are visible; hide them */
	if ( visible ) {
		Q_FOREACH( QWidget *nb, qApp->topLevelWidgets() )
			if ( qobject_cast<PiBreeze*>( nb ) )
				nb->hide();
	}

	/* Some are hidden, show them all */
	else {
		Q_FOREACH( QWidget *w, qApp->topLevelWidgets() ) {
			PiBreeze *nb = qobject_cast<PiBreeze*>( w );
			if ( nb and not nb->isClosed() )
				nb->show();
		}
	}
};

void PBTrayIcon::showInfo() {

	showMessage(
		"TrayIcon initialized",
		"PiBreeze is now open in SystemTray. Double click to open a new window, right click to quit.",
		QSystemTrayIcon::Information,
		2500
	);
};

void PBTrayIcon::quit() {

	/* Close all top level widgets */
	Q_FOREACH( QWidget *w, qApp->topLevelWidgets() )
		w->close();

	qApp->quit();
};
