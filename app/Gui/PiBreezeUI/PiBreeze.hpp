/*
	*
	* PiBreeze.hpp - PiBreeze Header
	*
*/

#pragma once

#include "Global.hpp"

#include "AboutPB.hpp"
#include "PBAddressBar.hpp"
#include "PBCustomActions.hpp"
#include "PBFilterWidget.hpp"
#include "PBFolderView.hpp"
#include "PBGuiWidgets.hpp"
#include "PBInfoPanel.hpp"
#include "PBLogger.hpp"
#include "PBMessageDialog.hpp"
#include "PBPropertiesDialog.hpp"
#include "PBSettingsManager.hpp"
#include "PBSidePanel.hpp"
#include "PBTrashManager.hpp"
#include "PBAppEditorDialog.hpp"

class PiBreeze : public QMainWindow {
	Q_OBJECT

	public :
		PiBreeze( QString loc = QString() );

		bool isClosed();

	private :
		/* Create our UI */
		void createGUI();

		/* Setup the SidePanel */
		void setupSidePanel();

		/* Setup the InfoPanel */
		void setupInfoPanel();

		/* Set window title, icon, size etc */
		void setWindowProperties();

		/* Create and Setup signal/slot mechanisms */
		void createAndSetupActions();

		/* Open a file */
		void openFile( QString );

		PBAddressBar *AddressBar;
		PBSidePanel *SidePanel;
		PBFolderView *FolderView;
		PBInfoPanel *InfoPanel;
		PBFilterWidget *FilterWidget;

		bool mTerminate;
		bool mClosed;

		QBasicTimer spShowTimer, spHideTimer, ipShowTimer, ipHideTimer;

	public Q_SLOTS:
		void show();

	private Q_SLOTS:
		void showAboutPB();
		void showAboutQt();
		void showLicense();

		void showInfoDlg();
		void showSettingsDialog();
		void showCustomActionsDialog();

		void newWindow( QString path = QString() );

		void handleMessages( const QString );

		void updateInfoBar();
		void updateVarious( QString url = QString() );

		void showProperties();
		void showPermissions();

		void handleDriveUrl( QString );
		void showApplications();
		void showCatalogs();
		void showFolders();
		void showTrash();

		void chdirUI( QString );

		void filterFiles( QString );
		void clearFilters();

		void openAddressBar();
		void openWithList();

		void toggleGrouping();

		void showSidePanel();
		void hideSidePanel();

		void showInfoPanel();
		void hideInfoPanel();

		/* Busy cursor during loading of models */
		void setBusyCursor();
		void setNormalCursor();

		void quit();

	protected:
		void timerEvent( QTimerEvent *tEvent );
		void closeEvent( QCloseEvent *cEvent );

};

// The code for @f PBStartup() and ReadSettings() is in PBStartup.cpp
void PBStartup();
