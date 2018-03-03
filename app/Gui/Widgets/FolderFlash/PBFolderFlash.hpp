/*
	*
	* PBPreviewWidget.hpp - PiBreeze PreviewWidget Class Header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBIconViewRestricted.hpp"

class PBFolderFlash : public QWidget {
	Q_OBJECT

	public :
		PBFolderFlash( QString path = QDir::homePath(), QWidget *parent = NULL );
		QString path;
		PBIconViewRestricted *peekWidgetBase;

	private :
		void createGUI();
		void setWindowProperties();

	private slots :
		void loadFolder();

	protected :
		void keyPressEvent( QKeyEvent *keyEvent );
		void changeEvent( QEvent *cEvent );

		void paintEvent( QPaintEvent *pEvent );

	Q_SIGNALS:
		void loadFolder( QString );
};
