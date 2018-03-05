/*
    *
    * PBTermWidget.hpp - PBTermWidget.cpp Header
    *
*/

#pragma once

#include "Global.hpp"
#include "qtermwidget.h"

class PBTermWidget : public QTermWidget {
	Q_OBJECT

	public :
		PBTermWidget( QWidget *parent = 0 );
		PBTermWidget( QString, QWidget *parent = 0 );
		PBTermWidget( QString, QString, QWidget *parent = 0 );

		QString currentWorkingDirectory();

	private:
		QString oldCWD;

	private slots:
		void reloadSettings();
		void handleFSWSignals( QString );

	Q_SIGNALS:
		void chDir( QString );
};
