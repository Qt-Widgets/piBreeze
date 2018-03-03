/*
	*
	* PBFolderView.hpp - PiBreeze FolderView Class Header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBConfirmDeleteDialog.hpp"
#include "PBContextMenu.hpp"
#include "PBCustomActions.hpp"
#include "PBCustomPeek.hpp"
#include "PBDeleteManager.hpp"
#include "PBDeleteProcess.hpp"
#include "PBErrorsDialog.hpp"
#include "PBEncFS.hpp"
#include "PBFileDialog.hpp"
#include "PBItemViewModel.hpp"
#include "PBFolderFlash.hpp"
#include "PBGuiWidgets.hpp"
#include "PBIconManager.hpp"
#include "PBIconView.hpp"
#include "PBIOProcess.hpp"
#include "PBLogger.hpp"
#include "PBMessageDialog.hpp"
#include "PBNewNodeDialog.hpp"
#include "PBPluginManager.hpp"
#include "PBProcessManager.hpp"
#include "PBPropertiesDialog.hpp"
#include "PBRenameDialog.hpp"
#include "PBRunCmdDialog.hpp"
#include "PBVault.hpp"
#include "PBVaultDatabase.hpp"

class PBFolderView : public QStackedWidget {
	Q_OBJECT

	public :
		PBFolderView( QWidget *parent = NULL);

		QModelIndexList getSelection();
		bool hasSelection();

		QAction *peekAct, *moveAct, *copyAct, *pasteAct, *renameAct, *reloadAct, *trashAct, *delAct, *propertiesAct, *permissionsAct;
		QAction *actPrevDir, *actNextDir, *actParDir, *actHomeDir, *actGoHome, *showHideDotFiles, *openVTE, *openVTEin;
		QAction *actNewDir, *actNewFile, *actNewEncFS;
		QAction *sortByNameAct, *sortByTypeAct, *sortBySizeAct, *sortByDateAct;
		QAction *groupsAct;
		QAction *addBookMarkAct, *addToSuperStartAct;

		PBIconView *IconView;

		PBItemViewModel *fsModel;
		PBActionsMenu *customMenu;

	private :
		QModelIndex indexAt( QPoint );
		void createAndSetupActions();

		bool moveItems;

		QClipboard *clipBoard;
		PBProcessManager *pMgr;

	public slots :
		void goUp();
		void goBack();
		void goForward();

		void newFolder();
		void newFile();

		void doOpen( QModelIndex );
		void doOpen( QString loc );
		void doOpenHome();
		void doOpenSS();
		void loadHomeDir();
		void doOpenWith();
		void doOpenInNewWindow();
		void doOpenInNewTab();
		void doOpenWithCmd();

		void showFolders();

		void doRename();
		void doPeek();
		void doReload();
		void doToggleHidden();
		void move( QStringList, QString );
		void copy( QStringList, QString );
		void link( QStringList, QString );
		void doSendToTrash();
		void doDelete();

		void selectAll();

		void sortByName();
		void sortByType();
		void sortBySize();
		void sortByDate();

		void openTerminal();
		void openTerminalIn();
		void showContextMenu( QPoint );

		// Over-riding the setFocus() of QWidget
		void setFocus();

	private slots :
		void prepareCopy();
		void prepareMove();
		void prepareIO();

		void handleWatchDogBark( QString path );
		void extract( QString );
		void compress( QStringList );

		void createEncFS();

		void updateActions();
		void addBookMark();
		void addToSuperStart();

	Q_SIGNALS :
		void newWindow( QString );
		void newTab( QString );
		void selectionChanged( const QItemSelection&, const QItemSelection& );

		void showProperties();
		void showPermissions();

		void reloadCatalogs();
		void reloadBookmarks();
		void reloadSuperStart();

		void toggleGroups();

		void hideStatusBar();
		void showStatusBar();
};
