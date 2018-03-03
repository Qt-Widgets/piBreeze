/*
	*
	* PBContextmenu.cpp - ContextMenu class for PiBreeze
	*
*/

#include "PBContextMenu.hpp"
#include "PBFolderView.hpp"
#include "PBPluginInterface.hpp"
#include "PBPluginManager.hpp"
#include "PBGuiFunctions.hpp"
#include "PBVaultDatabase.hpp"
#include "PBVault.hpp"

PBActionsMenu::PBActionsMenu( QList<QModelIndex> selectedIndexes, QString dir, QWidget *parent ) : QMenu( parent ) {
	/*
		*
		* We set the custom actions based on the indexes
		*
	*/

	selection.clear();

	selection << selectedIndexes;
	workingDir = QString( dir );

	setTitle( tr( "&Actions" ) );
	setIcon( QIcon( ":/icons/process.png" ) );

	buildDefaultActions();
	buildPluginsActions();
	buildCustomActionsMenu();

	QAction *addActionAct = new QAction( QIcon( ":/icons/list-add.png" ), tr( "Add custo&m action" ), this );
	connect( addActionAct, SIGNAL( triggered() ), this, SLOT( showCustomActionsDialog() ) );

	addSeparator();
	addAction( addActionAct );
};

void PBActionsMenu::buildDefaultActions() {

	PBVault *vlt = PBVault::instance();
	if ( selection.count() == 1 ) {
		QString node = QDir( workingDir ).filePath( selection.at( 0 ).data().toString() );
		if ( PBVaultDatabase::isEncryptedLocation( node ) ) {
			if ( isDir( node ) and vlt->isDirectoryDecrypted( node ) ) {
				QAction *encryptAct = new QAction( QIcon( ":/icons/emblem-mounted.png" ), tr( "&Encrypt" ), this );
				connect( encryptAct, SIGNAL( triggered() ), this, SLOT( encrypt() ) );
				addAction( encryptAct );
			}

			else {
				QAction *decryptAct = new QAction( QIcon( ":/icons/emblem-unmounted.png" ), tr( "&Decrypt" ), this );
				connect( decryptAct, SIGNAL( triggered() ), this, SLOT( decrypt() ) );
				addAction( decryptAct );
			}
		}

		else{
			QAction *encryptAct = new QAction( QIcon( ":/icons/emblem-mounted.png" ), tr( "&Encrypt" ), this );
			connect( encryptAct, SIGNAL( triggered() ), this, SLOT( encrypt() ) );
			addAction( encryptAct );
		}
	}
};

void PBActionsMenu::buildPluginsActions() {

	quint64 dirs = 0, files = 0, others = 0;
	Q_FOREACH( QModelIndex idx, selection ) {
		if ( isDir( QDir( workingDir ).filePath( idx.data().toString() ) ) )
			dirs++;

		else if ( isFile( QDir( workingDir ).filePath( idx.data().toString() ) ) )
			files++;

		else
			others++;
	}

	PluginList pluginList;
	PBPluginManager *pMgr = PBPluginManager::instance();

	/* If there are sockets, fifos, etc */
	if ( others ) {
		if ( others > 1 )
			pluginList = pMgr->plugins( PBPluginInterface::ActionInterface, PBPluginInterface::Enhancement, PBPluginInterface::Nodes, QString() );

		else
			pluginList = pMgr->plugins( PBPluginInterface::ActionInterface, PBPluginInterface::Enhancement, PBPluginInterface::Node, QString() );
	}

	/* If both files and dirs are selected */
	else if ( dirs and files )
		pluginList = pMgr->plugins( PBPluginInterface::ActionInterface, PBPluginInterface::Enhancement, PBPluginInterface::Nodes, QString() );

	/* If only dirs are selected */
	else if ( dirs ) {
		if ( dirs > 1 )
			pluginList = pMgr->plugins( PBPluginInterface::ActionInterface, PBPluginInterface::Enhancement, PBPluginInterface::Dirs, QString() );

		else
			pluginList = pMgr->plugins( PBPluginInterface::ActionInterface, PBPluginInterface::Enhancement, PBPluginInterface::Dir, QString() );
	}

	/* If only files are selected */
	else if ( files ) {
		if ( files > 1 )
			pluginList = pMgr->plugins( PBPluginInterface::ActionInterface, PBPluginInterface::Enhancement, PBPluginInterface::Files, QString() );

		else
			pluginList = pMgr->plugins( PBPluginInterface::ActionInterface, PBPluginInterface::Enhancement, PBPluginInterface::File, getMimeType( QDir( workingDir ).filePath( selection.at( 0 ).data().toString() ) ) );
	}

	/* If there is no selection */
	else {
		pluginList = pMgr->plugins( PBPluginInterface::ActionInterface, PBPluginInterface::Enhancement, PBPluginInterface::Nodes, QString() );
	}

	/* Plugins based actions */
	quint64 acts = 0;
	Q_FOREACH( PBPluginInterface* plugin, pluginList ) {
		if ( plugin ) {
			QStringList sources;
			Q_FOREACH( QModelIndex idx, selection )
				sources << QDir( workingDir ).filePath( idx.data().toString() );

			QList<QAction*> actions = plugin->actions( PBPluginInterface::ActionInterface, sources );
			acts += actions.count();
			Q_FOREACH( QAction *act, actions )
				addAction( act );
		}
	}

	if ( acts )
		addSeparator();
};

void PBActionsMenu::buildCustomActionsMenu() {

	Q_FOREACH( QString group, actionSettings.childGroups() ) {
		actionSettings.beginGroup( group );

		QString name = actionSettings.value( "Name" ).toString();
		QStringList glob = actionSettings.value( "Glob" ).toStringList();
		QString exec = actionSettings.value( "Exec" ).toString();

		if ( exec.contains( "%term" ) ) {
			QStringList terminal = getTerminal();
			QString file = QDir( workingDir ).filePath( selection.at( 0 ).data().toString() );
			if ( terminal.at( 0 ) == QString( "xterm" ) )
				terminal[ 1 ] = QString( "cd %1 && /bin/bash" ).arg( termFormatString( file ) );

			else {
				terminal[ 2 ] = file;
				terminal[ 4 ] = "/bin/bash";
			}
			exec.replace( "%term", terminal.join( " " ) );
		}

		QAction *customAction = new QAction( QIcon( ":/icons/openWith.png" ), name, this );
		if ( ( exec.endsWith( "%f" ) or exec.endsWith( "%d" ) ) and selection.count() == 1 ) {
			QString file = QDir( workingDir ).filePath( selection.at( 0 ).data().toString() );

			if ( glob.at( 0 ) == QString( "*" ) ) {
				exec.replace( "%f", file ).replace( "%d", file );
				customAction->setData( exec.split( " ", QString::SkipEmptyParts ) );

				connect( customAction, SIGNAL( triggered() ), this, SLOT( doCustomAction() ) );
				addAction( customAction );
			}

			else if ( glob.at( 0 ) == QString( "file" ) and QFileInfo( file ).isFile() ) {
				exec.replace( "%f", file ).replace( "%d", file );
				customAction->setData( exec.split( " ", QString::SkipEmptyParts ) );

				connect( customAction, SIGNAL( triggered() ), this, SLOT( doCustomAction() ) );
				addAction( customAction );
			}

			else if ( glob.at( 0 ) == QString( "folder" ) and QFileInfo( file ).isDir() ) {
				exec.replace( "%f", file ).replace( "%d", file );
				customAction->setData( exec.split( " ", QString::SkipEmptyParts ) );

				connect( customAction, SIGNAL( triggered() ), this, SLOT( doCustomAction() ) );
				addAction( customAction );
			}
		}

		else if ( exec.endsWith( "%F" ) or exec.endsWith( "%D" ) ) {
			QStringList execList = exec.split( " ", QString::SkipEmptyParts );
			execList.removeLast();
			foreach( QModelIndex idx, selection )
				execList << QDir( workingDir ).filePath( idx.data().toString() );
			customAction->setData( execList );

			connect( customAction, SIGNAL( triggered() ), this, SLOT( doCustomAction() ) );
			addAction( customAction );
		}

		else if ( selection.count() == 1 ) {
			QString file = QDir( workingDir ).filePath( selection.at( 0 ).data().toString() );
			if ( glob.at( 0 ) == QString( "*" ) ) {
				exec.replace( "%f", file ).replace( "%d", file );
				customAction->setData( exec.split( " ", QString::SkipEmptyParts ) );

				connect( customAction, SIGNAL( triggered() ), this, SLOT( doCustomAction() ) );
				addAction( customAction );
			}

			else if ( glob.at( 0 ) == QString( "file" ) and QFileInfo( file ).isFile() ) {
				exec.replace( "%f", file ).replace( "%d", file );
				customAction->setData( exec.split( " ", QString::SkipEmptyParts ) );

				connect( customAction, SIGNAL( triggered() ), this, SLOT( doCustomAction() ) );
				addAction( customAction );
			}

			else if ( glob.at( 0 ) == QString( "folder" ) and QFileInfo( file ).isDir() ) {
				exec.replace( "%f", file ).replace( "%d", file );
				customAction->setData( exec.split( " ", QString::SkipEmptyParts ) );

				connect( customAction, SIGNAL( triggered() ), this, SLOT( doCustomAction() ) );
				addAction( customAction );
			}
		}

		actionSettings.endGroup();
	}
};

void PBActionsMenu::extract() {
};

void PBActionsMenu::compress() {
};

void PBActionsMenu::decrypt() {

	PBVault *vlt = PBVault::instance();

	QString node = QDir( workingDir ).filePath( selection.at( 0 ).data().toString() );
	if ( isDir( node ) )
		vlt->decryptDirectory( node );

	else if ( isFile( node ) )
		vlt->decryptFile( node );
};

void PBActionsMenu::encrypt() {

	PBVault *vlt = PBVault::instance();

	QString node = QDir( workingDir ).filePath( selection.at( 0 ).data().toString() );
	if ( isDir( node ) )
		vlt->encryptDirectory( node );

	else if ( isFile( node ) )
		vlt->encryptFile( node );
};

void PBActionsMenu::showCustomActionsDialog() {

	PBCustomActions *customActions = new PBCustomActions();
	customActions->exec();
};

void PBActionsMenu::doCustomAction() {

	QAction *customAct = qobject_cast<QAction*>( sender() );

	QStringList cmdList = customAct->data().toStringList();
	if ( cmdList.count() == 1 )
		QProcess::startDetached( cmdList.at( 0 ) );

	else
		QProcess::startDetached( cmdList.takeFirst(), cmdList );
};

PBOpenWithMenu::PBOpenWithMenu( QString icon, QString name, QWidget *parent ) : QMenu( parent ) {

	setIcon( QIcon( icon ) );
	setTitle( name );
};

void PBOpenWithMenu::setWorkingDirectory( QString wDir ) {

	workingDir = QString( wDir );
};

void PBOpenWithMenu::buildMenu( QList<QModelIndex> selection ) {

	PBFolderView *FolderView = qobject_cast<PBFolderView *>( parent() );

	if ( selection.count() > 1 ) {
		/*
			*
			* In a multi selection if we have both files, folders and system nodes, then we do nothing.
			* We do something only when we have multiple files
			*
			* So,
			* 	=> Step 1: List all nodes and check if all are files
			* 	=> Step 2: If all nodes are not files, then add open
			* 		with and finish our business
			*   => Step 3: If all the files are nodes, then list all
			*		apps that handle the node for each file
			*   => Step 4: Get an intersection of all the apps via QSet
			* 	=> Step 5: If the app handles multiple files, add it to
			* 		the context menu, otherwise skip it.
			*
		*/

		QStringList nodes;
		PBAppEngine *engine = PBAppEngine::instance();

		bool allNodesAreFiles = true;
		for ( int i = selection.count() - 1; i >= 0; i-- ) {
			QString node = QDir( workingDir ).absoluteFilePath( selection[ i ].data().toString() );
			nodes << node;
			allNodesAreFiles = allNodesAreFiles and isFile( node );
		}

		if ( not allNodesAreFiles ) {
			QAction *openWithCmdAct = new QAction( QIcon( ":/icons/openWith.png" ), "&Open with...", this );
			openWithCmdAct->setData( QVariant( nodes ) );
			connect( openWithCmdAct, SIGNAL( triggered() ), FolderView, SLOT( doOpenWithCmd() ) );
			addAction( openWithCmdAct );
		}

		else {
			QSet<PBAppFile> finalAppsList;
			QList<PBAppsList> appsPerNode;
			foreach( QString file, nodes )
				appsPerNode << engine->appsForMimeType( mimeDb.mimeTypeForFile( file ) );

			// Creating a set of unique applications that can handle all the files
			finalAppsList = appsPerNode.at( 0 ).toQList().toSet();
			for( int i = 1; i < appsPerNode.count(); i++ ) {
				QList<PBAppFile> aList = appsPerNode.at( i ).toQList();
				finalAppsList = finalAppsList.intersect( aList.toSet() );
			}

			// Adding them to the context menu
			foreach( PBAppFile app, finalAppsList ) {
				QString name = app.value( PBAppFile::Name ).toString();
				QStringList exec = app.execArgs();
				QString icon = app.value( PBAppFile::Icon ).toString();

				QIcon progIcon = QIcon::fromTheme( icon, QIcon( icon ) );
				if ( progIcon.pixmap( QSize( 16, 16 ) ).isNull() )
					progIcon = QIcon( "/usr/share/pixmaps/" + icon + ".png" );

				if ( progIcon.pixmap( QSize( 16, 16 ) ).isNull() )
					progIcon = QIcon( "/usr/share/pixmaps/" + icon + ".xpm" );

				if ( progIcon.pixmap( QSize( 16, 16 ) ).isNull() )
					progIcon = QIcon( ":/icons/exec.png" );

				// Prepare @v exec
				if ( app.multipleArgs() ) {
					int idx = exec.indexOf( "<#NEWBREEZE-ARG-FILES#>" );
					exec.removeAt( idx );
					foreach( QString node, nodes )
						exec.insert( idx, node );
				}

				else {
					// We are intereseted only in multiple arg takers
					continue;
				}

				QAction *openWithAct = new QAction( progIcon, name, this );
				openWithAct->setData( QVariant( QStringList() << exec ) );
				connect( openWithAct, SIGNAL( triggered() ), FolderView, SLOT( doOpenWith() ) );
				addAction( openWithAct );
			}

			// Add a separator
			addSeparator();

			QAction *openWithCmdAct = new QAction( QIcon( ":/icons/openWith.png" ), "&Open with...", this );
			openWithCmdAct->setData( QVariant( nodes ) );
			connect( openWithCmdAct, SIGNAL( triggered() ), FolderView, SLOT( doOpenWithCmd() ) );
			addAction( openWithCmdAct );
		}
	}

	else {
		QString path;
		if ( workingDir.isEmpty() ) {
			path = selection[ 0 ].data( Qt::UserRole + 2 ).toString();
			path = exists( path ) ? path : selection[ 0 ].data( Qt::UserRole + 7 ).toString();
		}

		else {

			path = QDir( workingDir ).absoluteFilePath( selection[ 0 ].data().toString() );
		}

		PBAppEngine *engine = PBAppEngine::instance();
		PBAppsList apps = engine->appsForMimeType( mimeDb.mimeTypeForFile( path ) );
		Q_FOREACH( PBAppFile app, apps.toQList() ) {
			QString name = app.value( PBAppFile::Name ).toString();
			QStringList exec = app.execArgs();
			QString iconStr = app.value( PBAppFile::Icon ).toString();

			// '::icon(...)' means @icon is an externally defined function
			QIcon progIcon = ::icon( PBIconManager::instance()->icon( iconStr ) );

			// Prepare @v exec
			if ( app.takesArgs() )
				if ( app.multipleArgs() ) {
					int idx = exec.indexOf( "<#NEWBREEZE-ARG-FILES#>" );
					exec.removeAt( idx );
					exec.insert( idx, path );
				}

				else {
					int idx = exec.indexOf( "<#NEWBREEZE-ARG-FILE#>" );
					exec.removeAt( idx );
					exec.insert( idx, path );
				}
			else
				exec << path;

			QAction *openWithAct = new QAction( progIcon, name, this );
			openWithAct->setData( QVariant( QStringList() << exec ) );
			connect( openWithAct, SIGNAL( triggered() ), FolderView, SLOT( doOpenWith() ) );
			addAction( openWithAct );
		}

		// Open with vlc
		if ( isDir( path ) ) {
			QAction *openWithVLCAct = new QAction( QIcon::fromTheme( "vlc" ), "Open with &VLC", this );
			openWithVLCAct->setData( QVariant( QStringList() << "vlc" << path ) );
			connect( openWithVLCAct, SIGNAL( triggered() ), FolderView, SLOT( doOpenWith() ) );
			addSeparator();
			addAction( openWithVLCAct );
		}

		// Execute and Execute in terminal
		if ( isFile( path ) and isExec( path ) ) {
			QAction *runAct = new QAction( QIcon( ":/icons/exec.png" ), "Execute", this );
			runAct->setData( QVariant( path ) );
			connect( runAct, SIGNAL( triggered() ), FolderView, SLOT( doOpenWith() ) );

			QAction *runInTermAct = new QAction( QIcon( ":/icons/exec.png" ), "Execute in terminal", this );
			runInTermAct->setData( QVariant( ( getTerminal().join( " " ).arg( workingDir ).arg( path ) ).split( " " ) ) );
			connect( runInTermAct, SIGNAL( triggered() ), FolderView, SLOT( doOpenWith() ) );

			addSeparator();
			addAction( runAct );
			addAction( runInTermAct );
		}

		addSeparator();

		QAction *openWithCmdAct = new QAction( QIcon( ":/icons/openWith.png" ), "&Open with...", this );
		openWithCmdAct->setData( QVariant( path ) );
		connect( openWithCmdAct, SIGNAL( triggered() ), FolderView, SLOT( doOpenWithCmd() ) );
		addAction( openWithCmdAct );
	}
};

PBAddToCatalogMenu::PBAddToCatalogMenu( QString wNode, QModelIndexList nodeList, QWidget *parent ) : QMenu( parent ) {

	setTitle( "Add to Catalo&g" );
	setIcon( QIcon( ":/icons/catalogs.png" ) );

	workNode = QString( wNode );
	sNodes << nodeList;

	QAction *addToDocumentsAct = new QAction( QIcon( ":/icons/documents.png" ), "&Documents", this );
	connect( addToDocumentsAct, SIGNAL( triggered() ), this, SLOT( addToCatalog() ) );
	addAction( addToDocumentsAct );

	QAction *addToMusicAct = new QAction( QIcon::fromTheme( "folder-sound" ), "&Music", this );
	connect( addToMusicAct, SIGNAL( triggered() ), this, SLOT( addToCatalog() ) );
	addAction( addToMusicAct );

	QAction *addToPicturesAct = new QAction( QIcon::fromTheme( "folder-image" ), "&Pictures", this );
	connect( addToPicturesAct, SIGNAL( triggered() ), this, SLOT( addToCatalog() ) );
	addAction( addToPicturesAct );

	QAction *addToVideosAct = new QAction( QIcon::fromTheme( "folder-video" ), "&Videos", this );
	connect( addToVideosAct, SIGNAL( triggered() ), this, SLOT( addToCatalog() ) );
	addAction( addToVideosAct );

	addSeparator();

	/* Add custom catalogs */
	QSettings cSett( "PiBreeze", "Catalogs" );
	cSett.beginGroup( "Custom" );
	Q_FOREACH( QString ctlg, cSett.childKeys() ) {
		QAction *ctlgAct = new QAction( QIcon( ":/icons/catalogs-alt.png" ), ctlg, this );
		connect( ctlgAct, SIGNAL( triggered() ), this, SLOT( addToCatalog() ) );
		addAction( ctlgAct );
	}
	cSett.endGroup();

	/* Add to new catalog */
	QAction *addToNewCtlgAct = new QAction( QIcon( ":/icons/catalogs-alt.png" ), "Add to &New Catalog", this );
	connect( addToNewCtlgAct, SIGNAL( triggered() ), this, SLOT( addToNewCatalog() ) );
	addAction( addToNewCtlgAct );
};

void PBAddToCatalogMenu::addToCatalog() {

	QAction *act = qobject_cast<QAction*>( sender() );
	QString catalog = act->text().replace( "&", "" );

	QSettings catalogsConf( "PiBreeze", "Catalogs" );

	if ( catalogsConf.childKeys().contains( catalog ) ){
		QStringList existing = catalogsConf.value( catalog ).toStringList();

		if ( sNodes.count() ) {
			foreach( QModelIndex idx, sNodes ) {
				QString node = QDir( workNode ).filePath( idx.data().toString() );
				if ( isDir( node ) )
					existing << node;
			}
		}

		else {
			existing << workNode;
		}

		existing.removeDuplicates();
		catalogsConf.setValue( catalog, existing );
	}

	else {
		catalogsConf.beginGroup( "Custom" );

		QStringList existing = catalogsConf.value( catalog ).toStringList();

		if ( sNodes.count() ) {
			foreach( QModelIndex idx, sNodes ) {
				QString node = QDir( workNode ).filePath( idx.data().toString() );
				if ( isDir( node ) )
					existing << node;
			}
		}

		else {
			existing << workNode;
		}

		existing.removeDuplicates();
		catalogsConf.setValue( catalog, existing );
		catalogsConf.endGroup();
	}

	catalogsConf.sync();

	emit reloadCatalogs();
};

void PBAddToCatalogMenu::addToNewCatalog() {

	QAction *action = qobject_cast<QAction*>( sender() );
	if ( not action )
		return;

	QString catalog = action->text().replace( "&", "" );

	QSettings catalogsConf( "PiBreeze", "Catalogs" );
	QStringList existing = catalogsConf.value( catalog ).toStringList();

	if ( sNodes.count() ) {
		foreach( QModelIndex idx, sNodes ) {
			QString node = QDir( workNode ).filePath( idx.data().toString() );
			if ( isDir( node ) )
				existing << node;
		}
	}

	else {
		existing << workNode;
	}

	existing.removeDuplicates();

	catalogsConf.setValue( catalog, existing );
	catalogsConf.sync();

	emit reloadCatalogs();
};

void PBFolderView::showContextMenu( QPoint position ) {

	QList<QModelIndex> selectedList = getSelection();
	QMenu *menu = new QMenu( this );

	switch( fsModel->modelDataType() ) {
		case PBItemViewModel::SuperStart: {

			/* No selections */
			if ( not selectedList.count() ) {

				menu->addAction( reloadAct );
			}

			/* Single selection */
			else {
				menu->addAction( peekAct );

				QFileInfo fInfo( fsModel->nodeInfo( selectedList[ 0 ] ) );
				QString file = termFormatString( fInfo.absoluteFilePath() );

				if ( fInfo.isDir() ) {
					QAction *openInNewWinAct = new QAction( QIcon( ":/icons/newwin.png" ), "Open in New &Window", this );
					openInNewWinAct->setData( QVariant( fInfo.absoluteFilePath() ) );
					connect( openInNewWinAct, SIGNAL( triggered() ), this, SLOT( doOpenInNewWindow() ) );
					menu->addAction( openInNewWinAct );
				}

				PBOpenWithMenu *openWithMenu = new PBOpenWithMenu( ":/icons/openWith.png", "&Open With", this );
				openWithMenu->setWorkingDirectory( "" );
				openWithMenu->buildMenu( selectedList );

				menu->addMenu( openWithMenu );
				menu->addSeparator();

				/* If the node is a folder */
				if ( fInfo.isDir() ) {
					/* Add to catalogs to menu */
					PBAddToCatalogMenu *addToCatalogMenu = new PBAddToCatalogMenu( fsModel->currentDir(), selectedList, this );
					connect( addToCatalogMenu, SIGNAL( reloadCatalogs() ), this, SIGNAL( reloadCatalogs() ) );
					menu->addMenu( addToCatalogMenu );
					menu->addSeparator();
					menu->addAction( openVTEin );
					menu->addSeparator();
				}

				menu->addAction( renameAct );

				menu->addSeparator();

				trashAct->setText( "Remove from &SuperStart" );
				menu->addAction( trashAct );

				menu->addSeparator();
				menu->addAction( propertiesAct );
				menu->addAction( permissionsAct );
			}

			menu->exec( mapToGlobal( position ) );

			return;
		}

		case PBItemViewModel::Applications: {
			/* We should fix this later */

			return;
		}

		case PBItemViewModel::Catalogs: {
			/* We should fix this later */

			return;
		}

		case PBItemViewModel::FileSystem: {
			if ( selectedList.isEmpty() ) {

				// Create a new file/directory
				QMenu *createNewMenu = new QMenu( "Create &New" );
				createNewMenu->setIcon( QIcon::fromTheme( "archive-insert" ) );

				createNewMenu->addAction( actNewDir );
				createNewMenu->addAction( actNewFile );
				createNewMenu->addSeparator();
				createNewMenu->addAction( actNewEncFS );

				// Add this folder to catalog
				PBAddToCatalogMenu *addToCatalogMenu = new PBAddToCatalogMenu( fsModel->currentDir(), selectedList, this );
				connect( addToCatalogMenu, SIGNAL( reloadCatalogs() ), this, SIGNAL( reloadCatalogs() ) );

				// File/directory sorting
				QMenu *sortMenu = new QMenu( "&Sort by" );
				sortMenu->setIcon( QIcon::fromTheme( "view-sort-ascending" ) );

				sortMenu->addAction( sortByNameAct );
				sortMenu->addAction( sortBySizeAct );
				sortMenu->addAction( sortByTypeAct );
				sortMenu->addAction( sortByDateAct );
				sortMenu->addSeparator();
				sortMenu->addAction( groupsAct );

				menu->addMenu( createNewMenu );
				menu->addSeparator();

				menu->addMenu( addToCatalogMenu );
				menu->addAction( addBookMarkAct );
				menu->addAction( addToSuperStartAct );
				menu->addSeparator();

				menu->addAction( openVTE );
				menu->addSeparator();

				menu->addAction( pasteAct );
				menu->addSeparator();

				menu->addAction( reloadAct );
				menu->addSeparator();

				menu->addSeparator();
				menu->addMenu( sortMenu );

				menu->addSeparator();
				menu->addAction( propertiesAct );
				menu->addAction( permissionsAct );
			}

			else if ( selectedList.count() == 1 ) {
				menu->addAction( peekAct );

				QFileInfo fInfo( fsModel->nodeInfo( selectedList[ 0 ] ) );
				QString file = termFormatString( fInfo.absoluteFilePath() );

				if ( fInfo.isDir() ) {
					QAction *openInNewWinAct = new QAction( QIcon( ":/icons/newwin.png" ), "Open in New &Window", this );
					openInNewWinAct->setData( QVariant( fInfo.absoluteFilePath() ) );
					connect( openInNewWinAct, SIGNAL( triggered() ), this, SLOT( doOpenInNewWindow() ) );
					menu->addAction( openInNewWinAct );
				}

				PBOpenWithMenu *openWithMenu = new PBOpenWithMenu( ":/icons/openWith.png", "&Open With", this );
				openWithMenu->setWorkingDirectory( fsModel->currentDir() );
				openWithMenu->buildMenu( selectedList );

				// Add this folder to catalog
				PBAddToCatalogMenu *addToCatalogMenu = new PBAddToCatalogMenu( fsModel->currentDir(), selectedList, this );
				connect( addToCatalogMenu, SIGNAL( reloadCatalogs() ), this, SIGNAL( reloadCatalogs() ) );

				// Custom Actions
				customMenu = new PBActionsMenu( selectedList, fsModel->currentDir(), this );

				// File/directory sorting
				QMenu *sortMenu = new QMenu( "&Sort by" );
				sortMenu->setIcon( QIcon::fromTheme( "view-sort-ascending" ) );

				sortMenu->addAction( sortByNameAct );
				sortMenu->addAction( sortBySizeAct );
				sortMenu->addAction( sortByTypeAct );
				sortMenu->addAction( sortByDateAct );
				sortMenu->addSeparator();
				sortMenu->addAction( groupsAct );

				menu->addMenu( openWithMenu );
				menu->addSeparator();

				/* If the node is a folder */
				if ( fInfo.isDir() ) {
					/* Add to catalogs to menu */
					PBAddToCatalogMenu *addToCatalogMenu = new PBAddToCatalogMenu( fsModel->currentDir(), selectedList, this );
					connect( addToCatalogMenu, SIGNAL( reloadCatalogs() ), this, SIGNAL( reloadCatalogs() ) );
					menu->addMenu( addToCatalogMenu );
					menu->addAction( addBookMarkAct );
				}

				/* We can add file or folder to SuperStart */
				menu->addAction( addToSuperStartAct );
				menu->addSeparator();

				if ( fInfo.isDir() ) {
					menu->addAction( openVTEin );
					menu->addSeparator();
				}

				menu->addMenu( customMenu );
				menu->addSeparator();

				menu->addAction( moveAct );
				menu->addAction( copyAct );
				menu->addSeparator();

				menu->addAction( renameAct );

				menu->addSeparator();

				trashAct->setText( "Move to trash" );
				menu->addAction( trashAct );
				menu->addAction( delAct );

				menu->addSeparator();
				menu->addMenu( sortMenu );

				menu->addSeparator();
				menu->addAction( propertiesAct );
				menu->addAction( permissionsAct );
			}

			else {
				PBOpenWithMenu *openWithMenu = new PBOpenWithMenu( ":/icons/openWith.png", "&Open With", this );
				openWithMenu->setWorkingDirectory( fsModel->currentDir() );
				openWithMenu->buildMenu( selectedList );

				// Add this folder to catalog
				PBAddToCatalogMenu *addToCatalogMenu = new PBAddToCatalogMenu( fsModel->currentDir(), selectedList, this );
				connect( addToCatalogMenu, SIGNAL( reloadCatalogs() ), this, SIGNAL( reloadCatalogs() ) );

				// Custom Actions Menu
				customMenu = new PBActionsMenu( selectedList, fsModel->currentDir(), this );

				// File/directory sorting
				QMenu *sortMenu = new QMenu( "&Sort by" );
				sortMenu->setIcon( QIcon::fromTheme( "view-sort-ascending" ) );

				sortMenu->addAction( sortByNameAct );
				sortMenu->addAction( sortBySizeAct );
				sortMenu->addAction( sortByTypeAct );
				sortMenu->addAction( sortByDateAct );
				sortMenu->addSeparator();
				sortMenu->addAction( groupsAct );

				menu->addMenu( openWithMenu );
				menu->addSeparator();

				menu->addMenu( addToCatalogMenu );
				menu->addAction( addBookMarkAct );
				menu->addAction( addToSuperStartAct );
				menu->addSeparator();

				menu->addMenu( customMenu );
				menu->addSeparator();

				menu->addAction( moveAct );
				menu->addAction( copyAct );
				menu->addSeparator();

				trashAct->setText( "Move to trash" );
				menu->addAction( trashAct );
				menu->addAction( delAct );

				menu->addSeparator();
				menu->addMenu( sortMenu );

				menu->addSeparator();
				menu->addAction( propertiesAct );
				menu->addAction( permissionsAct );
			}

			menu->exec( mapToGlobal( position ) );
			return;
		}
	}
};
