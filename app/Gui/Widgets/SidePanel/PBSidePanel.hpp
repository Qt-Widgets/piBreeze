/*
	*
	* PBSidePanel.hpp - PBSidePanel.cpp header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBFlashLabel.hpp"
#include "PBDeviceView.hpp"
#include "PBVfsView.hpp"
#include "PBBookmarkView.hpp"

class PBSidePanel : public QWidget {
	Q_OBJECT

	public :
		PBSidePanel( QWidget *parent );

	private:
		void populateSidePanel();

		PBFlashLabel *dirLbl, *appLbl, *ctlLbl;
		PBTrashLabel *trashLabel;
		PBDevicesIcon *devIcon;
		PBVfsIcon *vfsIcon;
		PBBookmarksIcon *bmkIcon;

	protected :
		void mousePressEvent( QMouseEvent *mEvent );
		void mouseMoveEvent( QMouseEvent *mEvent );

	public slots:
		void flashApplications();
		void flashCatalogs();
		void flashFolders();
		void flashDevices();
		void flashVfs();
		void flashBookmarks();

	Q_SIGNALS:
		void showFolders();
		void showApplications();
		void showCatalogs();

		void showTrash();

		void driveClicked( QString );
};
