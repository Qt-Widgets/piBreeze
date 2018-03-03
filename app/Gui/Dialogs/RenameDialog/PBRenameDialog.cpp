/*
	*
	* PBRenameDialog.cpp - PiBreeze Renamer Class
	*
*/

#include "PBRenameDialog.hpp"

PBRenameDialog::PBRenameDialog( QString origName, QDir curDir, QWidget *parent ) : PBDialog( parent ) {

	name = QString( origName );
	dir = QDir( curDir );

	createGUI();
};

void PBRenameDialog::createGUI() {

	setDialogTitle( "Rename" );
	setDialogIcon( QIcon( ":/icons/rename.png" ) );

	renameOk = false;

	QVBoxLayout *lyt = new QVBoxLayout();
	QHBoxLayout *btnLyt = new QHBoxLayout();

	QLabel *lbl = new QLabel();
	le = new PBLineEdit( this );
	replaceCB = new QCheckBox();

	lbl->setText( tr( "Rename the item '%1' to:" ).arg( name ) );

	le->setText( name );
	le->selectAll();

	replaceCB->setText( "Replace E&xisting" );
	replaceCB->setChecked( false );

	segBtns = new PBSegmentButton( this );
	segBtns->setCount( 2 );

	segBtns->setSegmentIcon( 0, QIcon( ":/icons/ok.png" ) );
	segBtns->setSegmentText( 0, tr( "&Rename" ) );
	segBtns->segment( 0 )->setObjectName( "okBtn" );
	segBtns->setSegmentDisabled( 0 );

	segBtns->setSegmentIcon( 1, QIcon( ":/icons/cancel.png" ) );
	segBtns->setSegmentText( 1, tr( "&Cancel" ) );
	segBtns->segment( 1 )->setObjectName( "cancelBtn" );

	connect( segBtns, SIGNAL( clicked( int ) ), this, SLOT( handleSegmentClick( int ) ) );

	btnLyt->addWidget( replaceCB );
	btnLyt->addStretch( 0 );
	btnLyt->addWidget( segBtns );

	lyt->addWidget( lbl );
	lyt->addWidget( le );
	lyt->addLayout( btnLyt );

	connect( le, SIGNAL( textEdited( QString ) ), this, SLOT( handleTextChanged( QString ) ) );
	connect( le, SIGNAL( returnPressed() ), this, SLOT( rename() ) );
	connect( replaceCB, SIGNAL( stateChanged( int ) ), this, SLOT( handleCBStateChanged() ) );

	setLayout( lyt );
	le->setFocus();
};

QString PBRenameDialog::newName() {

	return le->text();
};

bool PBRenameDialog::canRename() {

	return renameOk;
};

void PBRenameDialog::handleTextChanged( QString newText ) {

	if ( newText.isEmpty() ) {
		segBtns->setSegmentDisabled( 0 );
		return;
	}

	if ( newText == name )
		segBtns->setSegmentDisabled( 0 );

	else if ( replaceCB->isChecked() ) {
		if ( isDir( dir.filePath( newText ) ) )
			segBtns->setSegmentDisabled( 0 );

		else
			segBtns->setSegmentEnabled( 0 );
	}

	else {
		if ( dir.entryList().contains( newText ) )
			segBtns->setSegmentDisabled( 0 );

		else
			segBtns->setSegmentEnabled( 0 );
	}
};

void PBRenameDialog::handleCBStateChanged() {

	handleTextChanged( le->text() );
};

void PBRenameDialog::handleSegmentClick( int seg ) {

	switch( seg ) {
		case 0:
			return rename();

		case 1:
			return cancel();
	}
};

void PBRenameDialog::rename() {

	if ( segBtns->segment( 0 )->isEnabled() ) {
		renameOk = true;
		close();
	}
};

void PBRenameDialog::cancel() {

	renameOk = false;
	close();
};

PBSuperStartRenameDialog::PBSuperStartRenameDialog( QString origName, QString category, QWidget *parent ) : PBDialog( parent ) {

	name = QString( origName );
	category = QString( category );

	createGUI();
};

void PBSuperStartRenameDialog::createGUI() {

	setDialogTitle( "Rename" );
	setDialogIcon( QIcon( ":/icons/rename.png" ) );

	renameOk = false;

	QVBoxLayout *lyt = new QVBoxLayout();
	QHBoxLayout *btnLyt = new QHBoxLayout();

	QLabel *lbl = new QLabel();
	le = new PBLineEdit( this );

	lbl->setText( tr( "Change display name of '%1' to:" ).arg( name ) );

	le->setText( name );
	le->selectAll();

	segBtns = new PBSegmentButton( this );
	segBtns->setCount( 2 );

	segBtns->setSegmentIcon( 0, QIcon( ":/icons/ok.png" ) );
	segBtns->setSegmentText( 0, tr( "&Rename" ) );
	segBtns->segment( 0 )->setObjectName( "okBtn" );
	segBtns->setSegmentDisabled( 0 );

	segBtns->setSegmentIcon( 1, QIcon( ":/icons/cancel.png" ) );
	segBtns->setSegmentText( 1, tr( "&Cancel" ) );
	segBtns->segment( 1 )->setObjectName( "cancelBtn" );

	connect( segBtns, SIGNAL( clicked( int ) ), this, SLOT( handleSegmentClick( int ) ) );

	btnLyt->addStretch( 0 );
	btnLyt->addWidget( segBtns );

	lyt->addWidget( lbl );
	lyt->addWidget( le );
	lyt->addLayout( btnLyt );

	connect( le, SIGNAL( textEdited( QString ) ), this, SLOT( handleTextChanged( QString ) ) );
	connect( le, SIGNAL( returnPressed() ), this, SLOT( rename() ) );

	setLayout( lyt );
	le->setFocus();
};

QString PBSuperStartRenameDialog::newName() {

	return le->text();
};

bool PBSuperStartRenameDialog::canRename() {

	return renameOk;
};

void PBSuperStartRenameDialog::handleTextChanged( QString newText ) {

	QSettings sett( "PiBreeze", "SuperStart" );
	sett.beginGroup( category );

	if ( newText.isEmpty() ) {
		segBtns->setSegmentDisabled( 0 );
		return;
	}

	if ( newText == name )
		segBtns->setSegmentDisabled( 0 );

	else {
		if ( sett.allKeys().contains( newText ) )
			segBtns->setSegmentDisabled( 0 );

		else
			segBtns->setSegmentEnabled( 0 );
	}
	sett.endGroup();
};

void PBSuperStartRenameDialog::handleSegmentClick( int seg ) {

	switch( seg ) {
		case 0:
			return rename();

		case 1:
			return cancel();
	}
};

void PBSuperStartRenameDialog::rename() {

	if ( segBtns->segment( 0 )->isEnabled() ) {
		renameOk = true;
		close();
	}
};

void PBSuperStartRenameDialog::cancel() {

	renameOk = false;
	close();
};
