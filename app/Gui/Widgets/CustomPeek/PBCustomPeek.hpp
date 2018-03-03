/*
	*
	* PBPreviewWidget.hpp - PiBreeze PreviewWidget Class Header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBGuiWidgets.hpp"
#include "PBIconManager.hpp"

class PBCustomPeek : public QWidget {
	Q_OBJECT

	public :
		PBCustomPeek( QString path = QDir::homePath(), QWidget *parent = NULL );
		QString path;
		QWidget *peekWidgetBase;
		QLabel *iconLbl, *nameLbl, *pathLbl, *mimeLbl, *sizeLbl, *timeLbl, *ugidLbl;

	private :
		void createGUI();
		void setWindowProperties();
		void getFileProps();

	private slots :
		void openInExternal();

	protected :
		void keyPressEvent( QKeyEvent *keyEvent );
		void changeEvent( QEvent *cEvent );

		void paintEvent( QPaintEvent *pEvent );
};
