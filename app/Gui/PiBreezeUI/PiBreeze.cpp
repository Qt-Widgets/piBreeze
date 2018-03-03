/*
	*
	* PiBreeze.cpp - PiBreeze MainWindow Class
	*
*/

#include "PiBreeze.hpp"

PiBreeze::PiBreeze( QString loc ) : QMainWindow() {

	/* If we have root privileges */
	if ( not getuid() ) {
		PBMessageDialog::critical(
			this,
			"PiBreeze - Attention!!",
			"You are running PiBreeze as superuser privileges. This is generally a bad idea and it is advised to perform actions as root, on a teminal or a tty. "
			"To minimize the potential damage that can be caused, PiBreeze will be started in safe mode with all the plugins disabled."
		);
	}

	/* No terminate signal until close */
	mTerminate = false;

	/* Closed flag */
	mClosed = false;

	/* Create the UI */
	createGUI();

	/* Make sure we always have absolute paths */
	if ( loc.count() )
		loc = QFileInfo( loc ).absoluteFilePath();

	/* Open with SuperStart */
	if ( Settings->General.SpecialOpen and Settings->General.SuperStart and loc.isEmpty() )
		FolderView->doOpen( "PB://SuperStart" );

	/* Open with Catalogs */
	else if ( Settings->General.SpecialOpen and Settings->General.OpenWithCatalog and loc.isEmpty() )
		FolderView->doOpen( "PB://Catalogs" );

	/* Load the a folder */
	else if ( not loc.isEmpty() ) {
		if ( loc.startsWith( "PB://SuperStart" ) )
			FolderView->doOpen( "PB://SuperStart" );

		else if ( loc.startsWith( "PB://Applications" ) )
			FolderView->doOpen( "PB://Applications" );

		else if ( loc.startsWith( "PB://Catalogs" ) )
			FolderView->doOpen( "PB://Catalogs" );

		if ( isFile( loc ) ) {

			/* This is a file, just open the file */
			openFile( loc );
		}

		else if ( isDir( loc ) ) {

			/* This is a folder open it */
			FolderView->doOpen( loc );
		}

		else if ( exists( loc ) ) {

			/* we do not what this node is */
			qDebug() << "I do not know how to open this:" << loc;
			qDebug() << "Trying xdg-open";
			QProcess proc;
			proc.startDetached( "xdg-open", QStringList() << loc );
		}

		else {

			/* What the hell is this? */
			qDebug() << "What should I do with:" << loc;
		}
	}

	else {
		if ( exists( Settings->Session.LastDir ) ) {
			FolderView->doOpen( QString( Settings->Session.LastDir ) );
		}

		else {
			FolderView->doOpen( QString( QDir::homePath() ) );
		}
	}

	/* Focus the CurrentWidget in FolderView */
	FolderView->currentWidget()->setFocus();

	/* Window Properties */
	setWindowProperties();

	/* Signal-Slot Connections */
	createAndSetupActions();
};

bool PiBreeze::isClosed() {

	return mClosed;
};

void PiBreeze::createGUI() {

	QWidget *BaseWidget = new QWidget( this );
	BaseWidget->setObjectName( "base" );

	AddressBar = new PBAddressBar( this );
	setupSidePanel();
	FolderView = new PBFolderView( this );
	setupInfoPanel();
	FilterWidget = new PBFilterWidget( FolderView );

	QWidget *Spacer = new QWidget();
	Spacer->setFixedHeight( 3 );
	Spacer->setStyleSheet( "border-bottom: 1px solid darkgray;" );

	QHBoxLayout *ViewLayout = new QHBoxLayout();
	ViewLayout->setContentsMargins( QMargins() );
	ViewLayout->setSpacing( 0 );

	/* One of the SidePanel's will be hidden automatically */
	ViewLayout->addWidget( SidePanel );

	ViewLayout->addWidget( FolderView );
	ViewLayout->addWidget( InfoPanel );

	QVBoxLayout *BaseLayout = new QVBoxLayout();
	BaseLayout->setContentsMargins( QMargins( 0, 3, 0, 0 ) );
	BaseLayout->setSpacing( 0 );

	BaseLayout->addWidget( AddressBar );
	BaseLayout->addWidget( Spacer );
	BaseLayout->addLayout( ViewLayout );

	BaseWidget->setLayout( BaseLayout );
	setCentralWidget( BaseWidget );

	// Widget Properties
	AddressBar->setFixedHeight( 32 );
	FilterWidget->hide();

	AddressBar->setFocusPolicy( Qt::NoFocus );
	SidePanel->setFocusPolicy( Qt::NoFocus );
	FolderView->setFocusPolicy( Qt::StrongFocus );

	setFocusPolicy( Qt::NoFocus );
};

void PiBreeze::setupSidePanel() {

	SidePanel = new PBSidePanel( this );

	connect( SidePanel, SIGNAL( driveClicked( QString ) ), this, SLOT( handleDriveUrl( QString ) ) );
	connect( SidePanel, SIGNAL( showFolders() ), this, SLOT( showFolders() ) );
	connect( SidePanel, SIGNAL( showApplications() ), this, SLOT( showApplications() ) );
	connect( SidePanel, SIGNAL( showCatalogs() ), this, SLOT( showCatalogs() ) );
	connect( SidePanel, SIGNAL( showTrash() ), this, SLOT( showTrash() ) );

	SidePanel->hide();
};

void PiBreeze::setupInfoPanel() {

	/* Create the InfoPanel */
	InfoPanel = new PBInfoPanel( this );
	InfoPanel->hide();

	updateInfoBar();
};

void PiBreeze::setWindowProperties() {

	setWindowTitle( QString( "%1 - PiBreeze" ).arg( baseName( FolderView->fsModel->currentDir() ) ) );
	setWindowIcon( QIcon( ":/icons/pibreeze.png" ) );

	setMinimumSize( 900, 600 );

	if ( not Settings->Session.Maximized )
		setGeometry( Settings->Session.Geometry );
};

void PiBreeze::createAndSetupActions() {

	connect( AddressBar, SIGNAL( openLocation( QString ) ), FolderView, SLOT( doOpen( QString ) ) );
	connect( AddressBar, SIGNAL( openSearch() ), FilterWidget, SLOT( show() ) );

	connect( AddressBar, SIGNAL( goBack() ), FolderView, SLOT( goBack() ) );
	connect( AddressBar, SIGNAL( goHome() ), FolderView, SLOT( loadHomeDir() ) );
	connect( AddressBar, SIGNAL( openSuperStart() ), FolderView, SLOT( doOpenSS() ) );
	connect( AddressBar, SIGNAL( goForward() ), FolderView, SLOT( goForward() ) );

	connect( FilterWidget, SIGNAL( search( QString ) ), this, SLOT( filterFiles( QString ) ) );
	connect( FilterWidget, SIGNAL( shown() ), AddressBar, SLOT( hideSearchButton() ) );
	connect( FilterWidget, SIGNAL( hidden() ), AddressBar, SLOT( showSearchButton() ) );
	connect( FilterWidget, SIGNAL( hidden() ), this, SLOT( clearFilters() ) );

	connect( FolderView, SIGNAL( showProperties() ), this, SLOT( showProperties() ) );
	connect( FolderView, SIGNAL( showPermissions() ), this, SLOT( showPermissions() ) );

	connect( FolderView, SIGNAL( newWindow( QString ) ), this, SLOT( newWindow( QString ) ) );
	connect( FolderView, SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( updateInfoBar() ) );
	connect( FolderView, SIGNAL( toggleGroups() ), this, SLOT( toggleGrouping() ) );

	connect( FolderView, SIGNAL( reloadCatalogs() ), SidePanel, SLOT( flashCatalogs() ) );
	connect( FolderView, SIGNAL( reloadBookmarks() ), SidePanel, SLOT( flashBookmarks() ) );

	connect( FolderView->fsModel, SIGNAL( directoryLoading( QString ) ), this, SLOT( updateVarious( QString ) ) );
	connect( FolderView->fsModel, SIGNAL( directoryLoading( QString ) ), this, SLOT( updateInfoBar() ) );
	connect( FolderView->fsModel, SIGNAL( directoryLoading( QString ) ), FolderView, SLOT( updateActions() ) );

	connect( FolderView->fsModel, SIGNAL( directoryLoading( QString ) ), this, SLOT( setBusyCursor() ) );
	connect( FolderView->fsModel, SIGNAL( directoryLoaded( QString ) ), this, SLOT( setNormalCursor() ) );

	connect( FolderView->IconView, SIGNAL( showInfoPanel() ), this, SLOT( showInfoPanel() ) );
	connect( FolderView->IconView, SIGNAL( hideInfoPanel() ), this, SLOT( hideInfoPanel() ) );

	connect( FolderView->IconView, SIGNAL( showSidePanel() ), this, SLOT( showSidePanel() ) );
	connect( FolderView->IconView, SIGNAL( hideSidePanel() ), this, SLOT( hideSidePanel() ) );

	// About PB
	QAction *aboutPBAct = new QAction( this );
	aboutPBAct->setShortcuts( Settings->Shortcuts.AboutPB );
	connect( aboutPBAct, SIGNAL( triggered() ), this, SLOT( showAboutPB() ) );

	// About Qt
	QAction *aboutQtAct = new QAction( this );
	aboutQtAct->setShortcuts( Settings->Shortcuts.AboutQt );
	connect( aboutQtAct, SIGNAL( triggered() ), this, SLOT( showAboutQt() ) );

	// About Qt
	QAction *licenseAct = new QAction( this );
	licenseAct->setShortcut( tr( "Ctrl+Shift+L" ) );
	connect( licenseAct, SIGNAL( triggered() ), this, SLOT( showLicense() ) );

	// PB Info
	QAction *showInfoAct = new QAction( this );
	showInfoAct->setShortcuts( Settings->Shortcuts.PBInfo );
	connect( showInfoAct, SIGNAL( triggered() ), this, SLOT( showInfoDlg() ) );

	// Show settings
	QAction *showSettingsAct = new QAction( this );
	showSettingsAct->setShortcuts( Settings->Shortcuts.Settings );
	connect( showSettingsAct, SIGNAL( triggered() ), this, SLOT( showSettingsDialog() ) );

	// Focus AddressBar
	QAction *focusAddressBarAct = new QAction( this );
	focusAddressBarAct->setShortcuts( Settings->Shortcuts.FocusAddressBar );
	connect( focusAddressBarAct, SIGNAL( triggered() ), AddressBar, SLOT( focusAddressEdit() ) );

	// Open new window
	QAction *newWindowAct = new QAction( this );
	newWindowAct->setShortcuts( Settings->Shortcuts.NewWindow );
	connect( newWindowAct, SIGNAL( triggered() ), this, SLOT( newWindow() ) );

	// Show Application View
	QAction *showApplicationsAct = new QAction( "show Applications", this );
	showApplicationsAct->setShortcuts( QList<QKeySequence>() << QKeySequence( "Alt+Shift+A" ) );
	connect( showApplicationsAct, SIGNAL( triggered() ), this, SLOT( showApplications() ) );

	// Show Catalog View
	QAction *showCatalogsAct = new QAction( "show Catalogs", this );
	showCatalogsAct->setShortcuts( QList<QKeySequence>() << QKeySequence( "Alt+Shift+C" ) );
	connect( showCatalogsAct, SIGNAL( triggered() ), this, SLOT( showCatalogs() ) );

	// Show Folder View
	QAction *showFoldersAct = new QAction( "Toggle Catalogs", this );
	showFoldersAct->setShortcuts( QList<QKeySequence>() << QKeySequence( "Alt+Shift+F" ) );
	connect( showFoldersAct, SIGNAL( triggered() ), this, SLOT( showFolders() ) );

	// Focus the search bar
	QAction *focusSearchAct = new QAction( "Focus SearchBar", this );
	focusSearchAct->setShortcuts( Settings->Shortcuts.FocusSearchBar );
	connect( focusSearchAct, SIGNAL( triggered() ), FilterWidget, SLOT( show() ) );

	// Clear the search bar
	QAction *clearSearchAct = new QAction( "Clear SearchBar", this );
	clearSearchAct->setShortcuts( Settings->Shortcuts.ClearSearchBar );
	connect( clearSearchAct, SIGNAL( triggered() ), this, SLOT( clearFilters() ) );

	// Quit PiBreeze
	QAction *quitPBAct = new QAction( "Quit PiBreeze", this );
	quitPBAct->setShortcuts( Settings->Shortcuts.QuitPiBreeze );
	connect( quitPBAct, SIGNAL( triggered() ), this, SLOT( quit() ) );

	addAction( focusAddressBarAct );
	addAction( newWindowAct );
	addAction( showApplicationsAct );
	addAction( showCatalogsAct );
	addAction( showFoldersAct );
	addAction( focusSearchAct );
	addAction( clearSearchAct );
	addAction( aboutPBAct );
	addAction( aboutQtAct );
	addAction( licenseAct );
	addAction( showInfoAct );
	addAction( showSettingsAct );
	addAction( quitPBAct );

	/* Update Widgets */
	updateVarious();
	updateInfoBar();
};

void PiBreeze::openFile( QString file ) {

	PBAppFile app = PBAppEngine::instance()->xdgDefaultApp( mimeDb.mimeTypeForFile( file ) );

	/* Show Open with command  */
	if ( not app.isValid() ) {

		PBRunCmdDialog *runCmd = new PBRunCmdDialog( QFileInfo( file ).fileName() );
		runCmd->exec();

		if ( runCmd->canRun() ) {
			QStringList exec = runCmd->commandString().split( " " );

			// Prepare @v exec
			if ( exec.contains( "<#NEWBREEZE-ARG-FILES#>" ) ) {
				int idx = exec.indexOf( "<#NEWBREEZE-ARG-FILES#>" );
				exec.removeAt( idx );
				exec.insert( idx, file );
			}

			else if ( exec.contains( "<#NEWBREEZE-ARG-FILE#>" ) ) {
				int idx = exec.indexOf( "<#NEWBREEZE-ARG-FILE#>" );
				exec.removeAt( idx );
				exec.insert( idx, file );
			}

			else
				exec << file;

			qDebug( "Opening file: %s [%s]", file.toLocal8Bit().data(), ( QProcess::startDetached( exec.takeFirst(), exec ) ? "DONE" : " FAILED" ) );
		}
	}

	/* We have a valid app */
	else {
		QStringList exec = app.execArgs();

		// Prepare @v exec
		if ( app.takesArgs() ) {
			if ( app.multipleArgs() ) {

				int idx = exec.indexOf( "<#NEWBREEZE-ARG-FILES#>" );
				exec.removeAt( idx );
				exec.insert( idx, file );
			}

			else {

				int idx = exec.indexOf( "<#NEWBREEZE-ARG-FILE#>" );
				exec.removeAt( idx );
				exec.insert( idx, file );
			}
		}

		else {

			exec << file;
		}

		qDebug( "Opening file: %s [%s]", file.toLocal8Bit().data(), ( QProcess::startDetached( exec.takeFirst(), exec ) ? "DONE" : " FAILED" ) );
	}
};

void PiBreeze::show() {

	if ( mTerminate )
		return;

	else
		QMainWindow::show();
};

void PiBreeze::showAboutPB() {

	AboutPB *about = new AboutPB( this );
	about->exec();
};

void PiBreeze::showAboutQt() {

	QMessageBox::aboutQt( this, QObject::tr( "About Qt %1" ).arg( QLatin1String( QT_VERSION_STR ) ) );
};

void PiBreeze::showLicense() {

	License *license = new License( this );
	license->exec();
};

void PiBreeze::showInfoDlg() {

	PBDialog *infoDlg = new PBDialog( this );
	infoDlg->setDialogTitle( "PiBreeze Readme" );
	infoDlg->setDialogIcon( QIcon( ":/icons/pibreeze.png" ) );

	QHBoxLayout *dlgLyt = new QHBoxLayout();
	dlgLyt->setContentsMargins( 0, 0, 0, 0 );

	QVBoxLayout *baseLyt = new QVBoxLayout();
	QHBoxLayout *btnLyt = new QHBoxLayout();

	QTextBrowser *nbInfo = new QTextBrowser();

	QFile nbInfoF( ":/README" );
	nbInfoF.open( QFile::ReadOnly );
	nbInfo->setText( QString( nbInfoF.readAll() ) );
	nbInfoF.close();
	nbInfo->setFocusPolicy( Qt::NoFocus );
	nbInfo->setFont( QFont( "Courier 10 Pitch", 9 ) );

	QPushButton *okBtn = new QPushButton( "&Ok" );
	okBtn->setObjectName( "okBtn" );
	connect( okBtn, SIGNAL( clicked() ), infoDlg, SLOT( close() ) );

	btnLyt->addStretch();
	btnLyt->addWidget( okBtn );
	btnLyt->addStretch();

	baseLyt->addWidget( nbInfo );
	baseLyt->addWidget( Separator::horizontal() );
	baseLyt->addLayout( btnLyt );

	QWidget *widget = new QWidget( this );
	widget->setLayout( baseLyt );

	dlgLyt->addWidget( widget );
	infoDlg->setLayout( dlgLyt );

	infoDlg->setFixedSize( QSize( 720, 630 ) );

	infoDlg->setWindowIcon( QIcon( ":/icons/pibreeze.png" ) );
	infoDlg->setWindowTitle( tr( "PiBreeze Info" ) );

	QDesktopWidget dw;
	int hpos = (int)( ( dw.width() - 720 ) / 2 );
	int vpos = (int)( ( dw.height() - 630 ) / 2 );
	infoDlg->move( hpos, vpos );

	infoDlg->exec();
};

void PiBreeze::showSettingsDialog() {

	PBSettingsManager *settingsMgr = new PBSettingsManager( this );
	settingsMgr->exec();

	qApp->processEvents();
};

void PiBreeze::showCustomActionsDialog() {

	PBCustomActions *customActions = new PBCustomActions();
	customActions->exec();
};

void PiBreeze::newWindow( QString location ) {

	if ( location.isEmpty() and qobject_cast<QAction*>( sender() ) ) {
		location = FolderView->fsModel->currentDir();
	}

	PiBreeze *pibreeze = new PiBreeze( location );
	if ( Settings->Session.Maximized )
		pibreeze->showMaximized();

	else
		pibreeze->showNormal();
};

void PiBreeze::handleMessages( const QString message ) {

	qDebug() << "Message recieved:" << message;

	if ( message.isEmpty() ) {

		/* User did not say what to do */
		newWindow( QString() );
	}

	else {
		if ( isFile( message ) ) {

			/* This is a file, just open the file */
			openFile( message );
		}

		else if ( isDir( message ) ) {

			/* This is a folder open it */
			newWindow( message );
		}

		else if ( exists( message ) ) {

			/* we do not what this node is */
			qDebug() << "I do not know how to open this:" << message;
			qDebug() << "Trying xdg-open";
			QProcess proc;
			proc.startDetached( "xdg-open", QStringList() << message );
		}

		else {

			/* What the hell is this? */
			qDebug() << "What should I do with:" << message;
		}
	}
};

void PiBreeze::updateVarious( QString url ) {

	/* Navigation Buttons */
	bool backBtn = false, fwdBtn = false;

	/* Enable/disable goBack */
	if ( FolderView->fsModel->canGoBack() )
		backBtn = true;

	/* Enable/disable goForward */
	if ( FolderView->fsModel->canGoForward() )
		fwdBtn = true;

	AddressBar->updateNavigationButtons( backBtn, fwdBtn );

	if ( url.isEmpty() or url.isNull() )
		url = FolderView->fsModel->currentDir();

	setWindowTitle( QString( "%1 - PiBreeze" ).arg( baseName( url ) ) );
	AddressBar->setAddress( url );
};

void PiBreeze::updateInfoBar() {

	if ( qobject_cast<PBItemViewModel*>( sender() ) == FolderView->fsModel )
		InfoPanel->updatePanel( FolderView->fsModel->currentDir(), QModelIndexList() );

	else {
		QModelIndexList selectedIndexes = FolderView->getSelection();
		InfoPanel->updatePanel( FolderView->fsModel->currentDir(), selectedIndexes );
	}
};

void PiBreeze::showProperties() {

	QList<QModelIndex> selectedList = FolderView->getSelection();
	QStringList paths;

	if ( !selectedList.count() )
		paths << FolderView->fsModel->currentDir();

	else
		foreach( QModelIndex idx, selectedList )
			paths << FolderView->fsModel->nodePath( idx );

	if ( FolderView->fsModel->modelDataType() == PBItemViewModel::Applications ) {
		PBDesktopFileEditor *desktopEditor = new PBDesktopFileEditor( this, paths.at( 0 ) );
		desktopEditor->exec();
	}

	else {
		PBPropertiesDialog *propsDlg = new PBPropertiesDialog( paths, PBPropertiesDialog::Properties, &mTerminate, this );
		propsDlg->show();
	}
};

void PiBreeze::showPermissions() {

	QList<QModelIndex> selectedList = FolderView->getSelection();
	QStringList paths;

	if ( !selectedList.count() )
		paths << FolderView->fsModel->currentDir();

	else
		foreach( QModelIndex idx, selectedList )
			paths << FolderView->fsModel->nodePath( idx );

	PBPropertiesDialog *permsDlg = new PBPropertiesDialog( paths, PBPropertiesDialog::Permissions, &mTerminate, this );
	permsDlg->show();
};

void PiBreeze::handleDriveUrl( QString url ){

	if ( url.startsWith( "PB://SuperStart" ) )
		FolderView->fsModel->goHome();

	if ( url.startsWith( "PB://Applications" ) )
		showApplications();

	else if ( url.startsWith( "PB://Catalogs" ) )
		showCatalogs();

	else if ( url.startsWith( "PB://Folders" ) )
		showFolders();

	else
		FolderView->doOpen( url );
};

void PiBreeze::chdirUI( QString url ){

	/* Update the UI to show the cwd of the shell */
	FolderView->doOpen( url );
};

void PiBreeze::showApplications() {

	if ( qobject_cast<PBSidePanel*>( sender() ) != SidePanel )
		SidePanel->flashApplications();

	FolderView->doOpen( "PB://Applications" );
	AddressBar->setAddress( "PB://Applications" );
};

void PiBreeze::showCatalogs() {

	if ( qobject_cast<PBSidePanel*>( sender() ) != SidePanel )
		SidePanel->flashCatalogs();

	FolderView->doOpen( "PB://Catalogs" );
	AddressBar->setAddress( "PB://Catalogs" );
};

void PiBreeze::showFolders() {

	if ( qobject_cast<PBSidePanel*>( sender() ) != SidePanel )
		SidePanel->flashFolders();

	FolderView->showFolders();
};

void PiBreeze::showTrash() {

	PBTrashManager *trashManager = new PBTrashManager();
	trashManager->show();
};

void PiBreeze::filterFiles( QString filter ) {

	FolderView->fsModel->setNameFilters( QStringList() << QString( "*%1*" ).arg( filter ) );
};

void PiBreeze::clearFilters() {

	FilterWidget->clear();
	FolderView->fsModel->setNameFilters( QStringList() );
	FolderView->setFocus();
};

void PiBreeze::openAddressBar() {

	if ( !QFileInfo( AddressBar->address() ).exists() ) {
		QString text = QString( "There is no file or directory named: "		\
			"<tt><b>%1</b></tt>. Please check the path entered."
		).arg(  AddressBar->address() );

		QString title = QString( "PiBreeze - Invalid Location" );

		PBMessageDialog::error( this, title, text );

		return;
	}

	FolderView->doOpen( AddressBar->address() );
};

void PiBreeze::openWithList() {

	if ( not FolderView->hasSelection() )
		return;

	PBOpenWithMenu *openWithMenu = new PBOpenWithMenu( ":/icons/openWith.png", "&Open With", FolderView );
	openWithMenu->setWorkingDirectory( FolderView->fsModel->currentDir() );
	openWithMenu->buildMenu( FolderView->getSelection() );

	QAbstractItemView *itemView = qobject_cast<QAbstractItemView*>( FolderView->currentWidget() );

	QRect rect = itemView->visualRect( FolderView->getSelection().at( 0 ) );
	QPoint pos( rect.x() + rect.width() / 2, rect.y() + rect.height() / 2 );

	openWithMenu->exec( itemView->mapToGlobal( pos ) );
};

void PiBreeze::toggleGrouping() {

	if ( Settings->General.Grouping )
		Settings->General.Grouping = false;

	else
		Settings->General.Grouping = true;

	FolderView->fsModel->setCategorizationEnabled( Settings->General.Grouping );
	FolderView->groupsAct->setChecked( Settings->General.Grouping );

	QSettings sett( FolderView->fsModel->nodePath( ".directory" ), QSettings::NativeFormat );
	sett.setValue( "PiBreeze/Grouping", Settings->General.Grouping );
};

void PiBreeze::showSidePanel() {

	/* If hide timer is active, then SidePanel is visible */
	/* Just stop the hide timer. */
	if ( spHideTimer.isActive() )
		spHideTimer.stop();

	else
		spShowTimer.start( 500, this );
};

void PiBreeze::hideSidePanel() {

	/* If show timer is active, then SidePanel is hidden */
	/* Just stop the show timer. */
	if ( spShowTimer.isActive() )
		spShowTimer.stop();

	else
		spHideTimer.start( 500, this );
};

void PiBreeze::showInfoPanel() {

	/* If hide timer is active, then InfoPanel is visible */
	/* Just stop the hide timer. */
	if ( ipHideTimer.isActive() )
		ipHideTimer.stop();

	else
		ipShowTimer.start( 500, this );
};

void PiBreeze::hideInfoPanel() {

	/* If show timer is active, then InfoPanel is hidden */
	/* Just stop the show timer. */
	if ( ipShowTimer.isActive() )
		ipShowTimer.stop();

	else
		ipHideTimer.start( 500, this );
};

void PiBreeze::setBusyCursor() {

	setCursor( Qt::WaitCursor );
};

void PiBreeze::setNormalCursor() {

	setCursor( Qt::ArrowCursor );
};

void PiBreeze::quit() {

	/* Close all top level widgets */
	Q_FOREACH( QWidget *w, qApp->topLevelWidgets() )
		w->close();

	qApp->quit();
};

void PiBreeze::timerEvent( QTimerEvent *tEvent ) {

	if ( tEvent->timerId() == spShowTimer.timerId() ) {
		spShowTimer.stop();
		SidePanel->show();
	}

	else if ( tEvent->timerId() == spHideTimer.timerId() ) {
		spHideTimer.stop();
		SidePanel->hide();
	}

	else if ( tEvent->timerId() == ipShowTimer.timerId() ) {
		ipShowTimer.stop();
		InfoPanel->show();
	}

	else if ( tEvent->timerId() == ipHideTimer.timerId() ) {
		ipHideTimer.stop();
		InfoPanel->hide();
	}

	else {
		tEvent->accept();
	}
};

void PiBreeze::closeEvent( QCloseEvent *cEvent ) {

	// We hide this window immediately, other processes may take a while longer to close down
	QMainWindow::close();

	// Close down recursive size checker in Properties
	mTerminate = true;

	// If there are background processes, bring them to front
	if ( PBProcessManager::instance()->activeProcessCount() )
		PBProcessManagerUI::instance()->show();

	// Store the previous session - geometry, and open directory.
	Settings->setValue( "Session/Geometry", geometry() );
	Settings->setValue( "Session/Maximized", isMaximized() );

	Settings->setValue( "Session/LastDir", FolderView->fsModel->lastOpenedFolder() );

	// Come to home dir
	chdir( PBXdg::home().toLocal8Bit().constData() );

	cEvent->accept();

	mClosed = true;
	qDebug( "Good Bye!" );
};
