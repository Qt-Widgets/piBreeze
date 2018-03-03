/*
	*
	* PBRunCmdDialog.cpp - PiBreeze 'Open file with' Class
	*
*/

#include "PBRunCmdDialog.hpp"

PBRunCmdDialog::PBRunCmdDialog( QString origName, QWidget *parent ) : PBDialog( parent ) {

	fileName = QString( origName );

	createGUI();
	createAndSetupActions();

	setWindowProperties();
	runOk = false;
}

QString PBRunCmdDialog::commandString() {

	return execCmd;
};

bool PBRunCmdDialog::canRun() {

	return runOk;
};

void PBRunCmdDialog::createGUI() {

	QVBoxLayout *lyt = new QVBoxLayout();
	QHBoxLayout *btnLyt = new QHBoxLayout();

	QLabel *lbl1 = new QLabel( tr( "Open '<tt><b>%1</b></tt>' with: " ).arg( fileName ) );
	QLabel *lbl2 = new QLabel( tr( "Custom command:" ) );
	le = new PBLineEdit( this );

	appList = new PBOpenWithView();

	okBtn = new PBButton( QIcon( ":/icons/ok.png" ), tr( "&Open" ), this );
	okBtn->setObjectName( "okBtn" );
	okBtn->setDisabled( true );

	defaultAndRunBtn = new PBButton( QIcon( ":/icons/ok.png" ), tr( "&Make default and open" ), this );
	defaultAndRunBtn->setObjectName( "okBtn" );
	defaultAndRunBtn->setDisabled( true );

	cancelBtn = new PBButton( QIcon( ":/icons/cancel.png" ), tr( "&Cancel" ), this );
	cancelBtn->setObjectName( "cancelBtn" );

	PBSegmentButton *segBtn = new PBSegmentButton( this );
	segBtn->addSegment( defaultAndRunBtn );
	segBtn->setSegmentWidth( 180 );

	PBSegmentButton *segBtns = new PBSegmentButton( this );
	segBtns->addSegment( okBtn );
	segBtns->addSegment( cancelBtn );
	segBtns->setSegmentWidth( 90 );

	btnLyt->addWidget( segBtn );
	btnLyt->addStretch( 0 );
	btnLyt->addWidget( segBtns );

	lyt->addWidget( lbl1 );
	lyt->addWidget( appList );
	lyt->addWidget( lbl2 );
	lyt->addWidget( le );
	lyt->addLayout( btnLyt );

	setLayout( lyt );
	le->setFocus();
};

void PBRunCmdDialog::createAndSetupActions() {

	connect( appList, SIGNAL( clicked( QModelIndex ) ), this, SLOT( appSelected( QModelIndex ) ) );
	connect( appList, SIGNAL( doubleClicked( QModelIndex ) ), this, SLOT( runCommand( QModelIndex ) ) );
	connect( appList, SIGNAL( activated( QModelIndex ) ), this, SLOT( runCommand( QModelIndex ) ) );

	connect( appList->selectionModel(), SIGNAL( currentChanged( QModelIndex, QModelIndex ) ), this, SLOT( appSelected( QModelIndex ) ) );

	connect( le, SIGNAL( textEdited( QString ) ), this, SLOT( handleTextChanged( QString ) ) );
	connect( le, SIGNAL( returnPressed() ), this, SLOT( runCommand() ) );

	connect( defaultAndRunBtn, SIGNAL( clicked() ), this, SLOT( makeDefaultAndRunCommand() ) );

	connect( okBtn, SIGNAL( clicked() ), this, SLOT( runCommand() ) );
	connect( cancelBtn, SIGNAL( clicked() ), this, SLOT( cancel() ) );
};

void PBRunCmdDialog::setWindowProperties() {

	setDialogTitle( "PiBreeze - Run Command" );
	setDialogIcon( QIcon( ":/icons/openWith.png" ) );

	QDesktopWidget dw;
	int hpos = ( int )( ( dw.width() - 480 ) / 2 );
	int vpos = ( int )( ( dw.height() - 640 ) / 2 );
	move( hpos, vpos );

	setFixedSize( 480, 640 );
};

void PBRunCmdDialog::handleTextChanged( QString newText ) {

	if ( newText.isEmpty() ) {
		defaultAndRunBtn->setDisabled( true );
		okBtn->setDisabled( true );
		return;
	}

	QStringList paths = QString( getenv( "PATH" ) ).split( ":" );
	foreach( QString path, paths ) {
		if ( QFileInfo( path + "/" + newText.split( " " )[ 0 ] ).exists() ) {
			execCmd = newText;
			okBtn->setEnabled( true );
			defaultAndRunBtn->setEnabled( true );
			return;
		}
	}

	defaultAndRunBtn->setDisabled( true );
	okBtn->setDisabled( true );
};

void PBRunCmdDialog::appSelected( QModelIndex index ) {

	le->setText( index.data( Qt::UserRole + 1 ).toString() );
	execCmd = index.data( Qt::UserRole + 3 ).toStringList().join( " " );
	okBtn->setEnabled( true );
	defaultAndRunBtn->setEnabled( true );
};

void PBRunCmdDialog::runCommand() {

	runOk = true;
	close();
};

void PBRunCmdDialog::runCommand( QModelIndex index ) {

	execCmd = index.data( Qt::UserRole + 3 ).toStringList().join( " " );
	runOk = true;
	close();
};

void PBRunCmdDialog::cancel() {

	runOk = false;
	close();
};

void PBRunCmdDialog::makeDefaultAndRunCommand() {

	QModelIndex idx = appList->currentIndex();

	PBAppEngine::setApplicationAsDefault( idx.data( Qt::UserRole + 9 ).toString(), getMimeType( fileName ) );
	execCmd = idx.data( Qt::UserRole + 3 ).toStringList().join( " " );
	runOk = true;
	close();
};
