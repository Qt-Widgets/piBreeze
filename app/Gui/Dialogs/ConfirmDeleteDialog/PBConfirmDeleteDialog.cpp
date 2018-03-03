/*
	*
	* PBConfirmDeleteDialog.cpp - PBConfirmDeleteDialog class
	*
*/

#include "PBConfirmDeleteDialog.hpp"
#include "PBGuiFunctions.hpp"

PBConfirmDeleteDialog::PBConfirmDeleteDialog( QString src, QStringList paths, bool permanent, QWidget *parent ) : PBDialog( parent ) {

	deletePaths = paths;
	source = src;

	setupGUI( permanent );
	setupTable();
}

void PBConfirmDeleteDialog::setupGUI( bool permanent ) {

	QVBoxLayout *dlgLyt = new QVBoxLayout();
	QHBoxLayout *msgLyt = new QHBoxLayout();
	QHBoxLayout *btnLyt = new QHBoxLayout();

	iconLbl = new QLabel();
	iconLbl->setMaximumSize( QSize( 64, 64 ) );
	iconLbl->setPixmap( QPixmap( ":/icons/question.png" ).scaled( 64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation ) );

	if ( permanent ) {
		textLbl = new QLabel(
			QString(
				"<p>You are about to delete the below listed files/directories from</p>"
				"<center><b><tt>%1</tt></b></center>"
				"<p>Items once deleted cannot be retrieved. Do you want to continue?</p>"
			).arg( source )
		);
	}

	else {
		textLbl = new QLabel(
			QString(
				"<p>You are about to trash the below listed files/directories from</p>"
				"<center><b><tt>%1</tt></b></center>"
				"<p>Do you wan't to continue?</p>"
			).arg( source )
		);
	}

	textLbl->setWordWrap( true );

	table = new QTableWidget( 0, 2 );
	table->setFocusPolicy( Qt::NoFocus );

	setDialogIcon( QIcon( ":/icons/pibreeze.png" ) );
	setDialogTitle( "Delete Files?" );

	msgLyt->addWidget( iconLbl );
	msgLyt->addWidget( textLbl );

	segBtns = new PBSegmentButton( this );
	segBtns->setCount( 2 );

	segBtns->setSegmentIcon( 0, QIcon( ":/icons/delete.png" ) );
	segBtns->setSegmentText( 0, tr( "&Delete" ) );
	segBtns->segment( 0 )->setObjectName( "abortBtn" );

	segBtns->setSegmentIcon( 1, QIcon( ":/icons/cancel.png" ) );
	segBtns->setSegmentText( 1, tr( "&Cancel" ) );
	segBtns->segment( 1 )->setObjectName( "cancelBtn" );
	segBtns->segment( 1 )->setFocus();

	connect( segBtns, SIGNAL( clicked( int ) ), this, SLOT( handleSegmentClick( int ) ) );

	btnLyt->addStretch( 0 );
	btnLyt->addWidget( segBtns );

	dlgLyt->addLayout( msgLyt );
	dlgLyt->addWidget( table );
	dlgLyt->addWidget( Separator::horizontal() );
	dlgLyt->addLayout( btnLyt );

	PBDialog::setLayout( dlgLyt );

	setFixedSize( 480, 480 );
};

void PBConfirmDeleteDialog::setupTable() {

	table->verticalHeader()->hide();
	table->setHorizontalHeaderLabels( QStringList() << "File Name" << "Size" );

	table->setShowGrid( false );
	table->setSelectionBehavior( QAbstractItemView::SelectRows );

	QHeaderView *headerView = new QHeaderView( Qt::Horizontal, table );
	table->setHorizontalHeader( headerView );
	#if QT_VERSION >= 0x050000
		headerView->setSectionResizeMode( 0, QHeaderView::Stretch );
		headerView->setSectionResizeMode( 1, QHeaderView::Fixed );
	#else
		headerView->setResizeMode( 0, QHeaderView::Stretch );
		headerView->setResizeMode( 1, QHeaderView::Fixed );
	#endif

	table->setColumnWidth( 1, 100 );
	#if QT_VERSION >= 0x050000
		table->verticalHeader()->setSectionResizeMode( QHeaderView::Fixed );
	#else
		table->verticalHeader()->setResizeMode( QHeaderView::Fixed );
	#endif

	foreach( QString path, deletePaths ) {
		addEntry( path );
	}
};

void PBConfirmDeleteDialog::addEntry( QString path ) {

	QTableWidgetItem *itm1 = new QTableWidgetItem( icon( PBIconManager::instance()->iconsForFile( "", path ) ), baseName( path ) );
	QTableWidgetItem *itm2;
	if ( isDir( path ) )
		itm2 = new QTableWidgetItem( QString( "%1 items" ).arg( nChildren( path ) ) );

	else
		itm2 = new QTableWidgetItem( QString( "%1" ).arg( formatSize( getSize( path ) ) ) );

	itm1->setFlags( itm1->flags() & ~Qt::ItemIsEditable );
	itm2->setFlags( itm2->flags() & ~Qt::ItemIsEditable );

	table->insertRow( table->rowCount() );

	table->setItem( table->rowCount() - 1, 0, itm1 );
	table->setItem( table->rowCount() - 1, 1, itm2 );
};

void PBConfirmDeleteDialog::keyPressEvent( QKeyEvent *keyEvent ) {

	if ( keyEvent->key() == Qt::Key_Escape )
		deleteCancel();

	else
		QWidget::keyPressEvent( keyEvent );
};

void PBConfirmDeleteDialog::close() {

	PBDialog::close();
	emit closed();
};

int PBConfirmDeleteDialog::exec() {

	QEventLoop loop;
	connect( this, SIGNAL( closed() ), &loop, SLOT( quit() ) );

	QWidget::show();
	loop.exec();

	return deleteFiles;
};

void PBConfirmDeleteDialog::handleSegmentClick( int seg ) {

	switch( seg ) {
		case 0:
			return deleteOk();

		case 1:
			return deleteCancel();
	}
};

void PBConfirmDeleteDialog::deleteOk() {

	deleteFiles = true;
	close();
};

void PBConfirmDeleteDialog::deleteCancel() {

	deleteFiles = false;
	close();
};

PBConfirmDeleteProtectedDialog::PBConfirmDeleteProtectedDialog( QString src, QStringList paths, bool root, QWidget *parent ) : PBDialog( parent ) {

	deletePaths = paths;
	source = src;

	/* deleteFiles flag: 0 means cancel, 1 means delete others, 2 means delete all */
	deleteFiles = 0;

	setupGUI( root );
	setupTable();
}

void PBConfirmDeleteProtectedDialog::setupGUI( bool root ) {

	QVBoxLayout *dlgLyt = new QVBoxLayout();
	QHBoxLayout *msgLyt = new QHBoxLayout();
	QHBoxLayout *btnLyt = new QHBoxLayout();

	iconLbl = new QLabel();
	iconLbl->setMaximumSize( QSize( 64, 64 ) );
	iconLbl->setPixmap( QPixmap( ":/icons/question.png" ).scaled( 64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation ) );

	if ( root) {
		textLbl = new QLabel(
			QString(
				"You are attempting to delete some system files. This operation is potentially dangerous. "
				"If you really want to delete them, please type <tt><b>Yes, I really want to delete the selected files</b></tt> "
				"in the box below and press <tt>Delete</tt>.</p>"
			)
		);
	}

	else {
		textLbl = new QLabel(
			QString(
			"<p>One or more of the nodes you are trying to delete are under <b><tt>Accidental Delete Protection</tt></b>. "
			"If you really want to delete them, please type <tt><b>Yes, I really want to delete the selected files</b></tt> "
			"in the box below and press <tt>Delete</tt>.</p>"
			"<p>To delete only the unprotected nodes, press <tt>Delete others</tt>. To cancel the operation, press <tt>Cancel</tt></p>"
			)
		);
	}

	textLbl->setWordWrap( true );

	table = new QTableWidget( 0, 2 );
	table->setFocusPolicy( Qt::NoFocus );

	setDialogIcon( QIcon( ":/icons/pibreeze.png" ) );
	setDialogTitle( "Delete Protected Nodes?" );

	msgLyt->addWidget( iconLbl );
	msgLyt->addWidget( textLbl );

	confirmPhraseLE = new QLineEdit( this );
	confirmPhraseLE->setAlignment( Qt::AlignCenter );
	connect( confirmPhraseLE, SIGNAL( textChanged( QString ) ), this, SLOT( handleTextChanged( QString ) ) );

	segBtns = new PBSegmentButton( this );

	segBtns->setCount( 3 );

	segBtns->setSegmentIcon( 0, QIcon( ":/icons/delete.png" ) );
	segBtns->setSegmentText( 0, tr( "&Delete" ) );
	segBtns->segment( 0 )->setObjectName( "abortBtn" );
	segBtns->setSegmentDisabled( 0 );

	segBtns->setSegmentIcon( 1, QIcon( ":/icons/delete.png" ) );
	segBtns->setSegmentText( 1, tr( "&Delete Others" ) );
	segBtns->segment( 1 )->setObjectName( "abortBtn" );

	segBtns->setSegmentIcon( 2, QIcon( ":/icons/cancel.png" ) );
	segBtns->setSegmentText( 2, tr( "&Cancel" ) );
	segBtns->segment( 2 )->setObjectName( "cancelBtn" );
	segBtns->segment( 2 )->setFocus();

	if ( root )
		segBtns->segment( 1 )->hide();

	connect( segBtns, SIGNAL( clicked( int ) ), this, SLOT( handleSegmentClick( int ) ) );

	btnLyt->addStretch( 0 );
	btnLyt->addWidget( segBtns );

	dlgLyt->addLayout( msgLyt );
	dlgLyt->addWidget( confirmPhraseLE, Qt::AlignCenter );
	dlgLyt->addWidget( table );
	dlgLyt->addWidget( Separator::horizontal() );
	dlgLyt->addLayout( btnLyt );

	PBDialog::setLayout( dlgLyt );

	setFixedSize( 480, 540 );
};

void PBConfirmDeleteProtectedDialog::setupTable() {

	table->verticalHeader()->hide();
	table->setHorizontalHeaderLabels( QStringList() << "File Name" << "Size" );

	table->setShowGrid( false );
	table->setSelectionBehavior( QAbstractItemView::SelectRows );

	QHeaderView *headerView = new QHeaderView( Qt::Horizontal, table );
	table->setHorizontalHeader( headerView );
	#if QT_VERSION >= 0x050000
		headerView->setSectionResizeMode( 0, QHeaderView::Stretch );
		headerView->setSectionResizeMode( 1, QHeaderView::Fixed );
	#else
		headerView->setResizeMode( 0, QHeaderView::Stretch );
		headerView->setResizeMode( 1, QHeaderView::Fixed );
	#endif

	table->setColumnWidth( 1, 100 );
	#if QT_VERSION >= 0x050000
		table->verticalHeader()->setSectionResizeMode( QHeaderView::Fixed );
	#else
		table->verticalHeader()->setResizeMode( QHeaderView::Fixed );
	#endif

	foreach( QString path, deletePaths ) {
		addEntry( path );
	}
};

void PBConfirmDeleteProtectedDialog::addEntry( QString path ) {

	QTableWidgetItem *itm1 = new QTableWidgetItem( icon( PBIconManager::instance()->iconsForFile( "", path ) ), baseName( path ) );
	QTableWidgetItem *itm2;
	if ( isDir( path ) )
		itm2 = new QTableWidgetItem( QString( "%1 items" ).arg( nChildren( path ) ) );

	else
		itm2 = new QTableWidgetItem( QString( "%1" ).arg( formatSize( getSize( path ) ) ) );

	itm1->setFlags( itm1->flags() & ~Qt::ItemIsEditable );
	itm2->setFlags( itm2->flags() & ~Qt::ItemIsEditable );

	table->insertRow( table->rowCount() );

	table->setItem( table->rowCount() - 1, 0, itm1 );
	table->setItem( table->rowCount() - 1, 1, itm2 );
};

void PBConfirmDeleteProtectedDialog::keyPressEvent( QKeyEvent *keyEvent ) {

	if ( keyEvent->key() == Qt::Key_Escape )
		deleteCancel();

	else
		QWidget::keyPressEvent( keyEvent );
};

void PBConfirmDeleteProtectedDialog::close() {

	PBDialog::close();
	emit closed();
};

int PBConfirmDeleteProtectedDialog::exec() {

	QEventLoop loop;
	connect( this, SIGNAL( closed() ), &loop, SLOT( quit() ) );

	QWidget::show();
	loop.exec();

	return deleteFiles;
};

void PBConfirmDeleteProtectedDialog::handleSegmentClick( int seg ) {

	switch( seg ) {
		case 0:
			return deleteAll();

		case 1:
			return deleteOthers();

		case 2:
			return deleteCancel();
	}
};

void PBConfirmDeleteProtectedDialog::handleTextChanged( QString newText ) {

	if ( newText == QString( "Yes, I really want to delete the selected files" ) )
		segBtns->setSegmentEnabled( 0 );

	else
		segBtns->setSegmentDisabled( 0 );
};

void PBConfirmDeleteProtectedDialog::deleteAll() {

	deleteFiles = 2;
	close();
};

void PBConfirmDeleteProtectedDialog::deleteOthers() {

	deleteFiles = 1;
	close();
};

void PBConfirmDeleteProtectedDialog::deleteCancel() {

	deleteFiles = 0;
	close();
};

PBDeleteErrorsDialog::PBDeleteErrorsDialog( QStringList files, QStringList dirs, QWidget *parent ) : PBDialog( parent ) {

	fileList << files;
	dirList << dirs;

	setupGUI();
	setupTable();
}

void PBDeleteErrorsDialog::setupGUI() {

	QVBoxLayout *dlgLyt = new QVBoxLayout();
	QHBoxLayout *msgLyt = new QHBoxLayout();
	QHBoxLayout *btnLyt = new QHBoxLayout();

	iconLbl = new QLabel();
	iconLbl->setMaximumSize( QSize( 64, 64 ) );
	iconLbl->setPixmap( QPixmap( ":/icons/question.png" ).scaled( 64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation ) );

	textLbl = new QLabel(
		"Some errors were encountered while deleting the files and folders you requested. "
		"As a result, some of the files and folders may not have been deleted. For the "
		"list for files and folder not deleted click <u>M</u>ore"
	);

	textLbl->setWordWrap( true );

	table = new QTableWidget( 0, 2 );
	table->setFocusPolicy( Qt::NoFocus );

	msgLyt->addWidget( iconLbl );
	msgLyt->addWidget( textLbl );

	segBtns = new PBSegmentButton( this );
	segBtns->setCount( 1 );

	segBtns->setSegmentIcon( 0, QIcon( ":/icons/ok.png" ) );
	segBtns->setSegmentText( 0, tr( "&Okay" ) );
	segBtns->setSegmentDisabled( 0 );

	btnLyt->addStretch( 0 );
	btnLyt->addWidget( segBtns );

	dlgLyt->addLayout( msgLyt );
	dlgLyt->addWidget( table );
	dlgLyt->addWidget( Separator::horizontal() );
	dlgLyt->addLayout( btnLyt );

	PBDialog::setLayout( dlgLyt );

	setDialogIcon( QIcon( ":/icons/pibreeze.png" ) );
	setDialogTitle( "PiBreeze - Error while deleting" );

	setFixedSize( 480, 480 );
};

void PBDeleteErrorsDialog::setupTable() {

	table->verticalHeader()->hide();
	table->setHorizontalHeaderLabels( QStringList() << "File Name" << "Size" );

	table->setShowGrid( false );
	table->setSelectionBehavior( QAbstractItemView::SelectRows );

	QHeaderView *headerView = new QHeaderView( Qt::Horizontal, table );
	table->setHorizontalHeader( headerView );
	#if QT_VERSION >= 0x050000
		headerView->setSectionResizeMode( 0, QHeaderView::Stretch );
		headerView->setSectionResizeMode( 1, QHeaderView::Fixed );
	#else
		headerView->setResizeMode( 0, QHeaderView::Stretch );
		headerView->setResizeMode( 1, QHeaderView::Fixed );
	#endif

	table->setColumnWidth( 1, 100 );
	#if QT_VERSION >= 0x050000
		table->verticalHeader()->setSectionResizeMode( QHeaderView::Fixed );
	#else
		table->verticalHeader()->setResizeMode( QHeaderView::Fixed );
	#endif

	foreach( QString path, dirList ) {
		addEntry( path );
	}

	foreach( QString path, fileList ) {
		addEntry( path );
	}
};

void PBDeleteErrorsDialog::addEntry( QString path ) {

	QTableWidgetItem *itm1 = new QTableWidgetItem( icon( PBIconManager::instance()->iconsForFile( "", path ) ), baseName( path ) );
	QTableWidgetItem *itm2;
	if ( isDir( path ) )
		itm2 = new QTableWidgetItem( QString( "%1 items" ).arg( nChildren( path ) ) );

	else
		itm2 = new QTableWidgetItem( QString( "%1" ).arg( formatSize( getSize( path ) ) ) );

	itm1->setFlags( itm1->flags() & ~Qt::ItemIsEditable );
	itm2->setFlags( itm2->flags() & ~Qt::ItemIsEditable );

	table->insertRow( table->rowCount() );

	table->setItem( table->rowCount() - 1, 0, itm1 );
	table->setItem( table->rowCount() - 1, 1, itm2 );
};

void PBDeleteErrorsDialog::keyPressEvent( QKeyEvent *keyEvent ) {

	if ( keyEvent->key() == Qt::Key_Escape )
		close();

	else
		QWidget::keyPressEvent( keyEvent );
};

void PBDeleteErrorsDialog::close() {

	PBDialog::close();
	emit closed();
};

int PBDeleteErrorsDialog::exec() {

	QEventLoop loop;
	connect( this, SIGNAL( closed() ), &loop, SLOT( quit() ) );

	QWidget::show();
	loop.exec();

	return 0;
};
