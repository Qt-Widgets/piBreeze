/*
	*
	* PBAddressWidget.cpp - AddressBar for PiBreeze
	*
*/

#include "PBAddressBar.hpp"
#include "PBGuiFunctions.hpp"

PBToggleButton::PBToggleButton( QWidget *parent ) : QWidget( parent ) {

	checked = false;
	setFixedSize( 28, 28 );
};

bool PBToggleButton::isChecked() {

	return checked;
};

void PBToggleButton::setChecked( bool truth ) {

	checked = truth;
	repaint();
};

void PBToggleButton::setShortcut( QString kSeq ) {

	shortcut = new QAction( this );
	shortcut->setShortcut( kSeq );
	connect( shortcut, SIGNAL( triggered() ), this, SLOT( toggle() ) );
	connect( shortcut, SIGNAL( triggered() ), this, SIGNAL( clicked() ) );

	addAction( shortcut );
};

void PBToggleButton::toggle() {

	if ( checked )
		checked = false;

	else
		checked = true;

	repaint();
};

void PBToggleButton::mousePressEvent( QMouseEvent *mEvent ) {

	toggle();

	emit clicked();
	mEvent->accept();
};

void PBToggleButton::paintEvent( QPaintEvent *pEvent ) {

	QPainter painter( this );

	painter.setFont( QFont( font().family(), 14 ) );

	if ( palette().color( QPalette::Text ) == Qt::white ) {
		if ( checked )
			painter.setPen( Qt::lightGray );

		else
			painter.setPen( QColor( 60, 60, 60 ) );
	}

	else {
		if ( checked )
			painter.setPen( QColor( 60, 60, 60 ) );

		else
			painter.setPen( Qt::lightGray );
	}

	painter.drawText( QRectF( 0, 0, 28, 28 ), Qt::AlignCenter, QString::fromUtf8( "\u270D " ) );

	painter.end();
	pEvent->accept();
};

PBAddressEdit::PBAddressEdit( QWidget *parent ) : QLineEdit( parent ) {

	setStyleSheet( "background: transparent; border: none;" );
};

PBAddressWidget::PBAddressWidget( QWidget *parent ) : QWidget( parent ) {

	addressEdit = new PBAddressEdit( this );
	addressEdit->setFont( QFont( "DejaVu Sans Mono", 9 ) );
	addressEdit->setFocusPolicy( Qt::NoFocus );

	connect( addressEdit, SIGNAL( returnPressed() ), this, SLOT( revertToCrumbsBar() ) );

	crumbsBar = new PBCrumbsBar( this );
	connect( crumbsBar, SIGNAL( openLocation( QString ) ), this, SIGNAL( openLocation( QString ) ) );

	toggleBtn = new PBToggleButton( this );

	QHBoxLayout *bLyt = new QHBoxLayout();
	bLyt->setContentsMargins( QMargins() );
	bLyt->setSpacing( 0 );

	bLyt->addWidget( crumbsBar );
	bLyt->addWidget( addressEdit );
	bLyt->addWidget( toggleBtn );

	QWidget *base = new QWidget( this );
	base->setObjectName( "addressWidget" );
	base->setLayout( bLyt );

	QHBoxLayout *lyt = new QHBoxLayout();
	lyt->setContentsMargins( QMargins() );
	lyt->setSpacing( 0 );

	lyt->addWidget( base );
	setLayout( lyt );

	addressEdit->hide();

	dModel = new QDirModel();
	dModel->setFilter( QDir::Dirs | QDir::NoDotAndDotDot );

	QCompleter *completer = new QCompleter();
	completer->setModel( dModel );
	addressEdit->setCompleter( completer );

	setWidgetProperties();
};

void PBAddressWidget::setFocus() {

	if ( not toggleBtn->isChecked() ) {
		toggleBtn->setChecked( true );
		crumbsBar->hide();
		addressEdit->show();
		addressEdit->setFocus();
	}
};

void PBAddressWidget::setShowHidden( bool shown ) {

	if ( shown )
		dModel->setFilter( QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden );

	else
		dModel->setFilter( QDir::Dirs | QDir::NoDotAndDotDot );
};

void PBAddressWidget::setWidgetProperties() {

	setContentsMargins( QMargins() );
	setFont( QFont( "DejaVu Sans Mono", 9 ) );

	setFixedHeight( 28 );

	toggleBtn->setChecked( false );
	toggleBtn->setFocusPolicy( Qt::NoFocus );
	toggleBtn->setShortcut( Settings->Shortcuts.ToggleCrumbLE.at( 0 ).toString() );

	connect( toggleBtn, SIGNAL( clicked() ), this, SLOT( toggleCrumbsBarAndEdit() ) );
};

void PBAddressWidget::toggleCrumbsBarAndEdit() {

	if ( toggleBtn->isChecked() ) {
		crumbsBar->hide();
		addressEdit->show();
		if ( Settings->General.ShowHidden )
			dModel->setFilter(  QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden );

		else
			dModel->setFilter(  QDir::Dirs | QDir::NoDotAndDotDot );
	}

	else {
		crumbsBar->show();
		addressEdit->hide();
	}
};

void PBAddressWidget::revertToCrumbsBar() {

	if ( toggleBtn->isChecked() ) {
		toggleBtn->setChecked( false );
		crumbsBar->show();
		addressEdit->hide();

		emit openLocation( addressEdit->text() );
	}
};

PBAddressBar::PBAddressBar( QWidget *parent ) : QWidget( parent ) {

	setFixedHeight( 28 );

	QHBoxLayout *fLyt = new QHBoxLayout();
	fLyt->setContentsMargins( QMargins( 3, 0, 0, 0 ) );

	// Previous button

	backBtn = new PBButton( icon( PBIconManager::instance()->icon( "go-previous" ) ), this );
	backBtn->setFocusPolicy( Qt::NoFocus );
	backBtn->setFlat( true );

	// Next Button
	forwardBtn = new PBButton( icon( PBIconManager::instance()->icon( "go-next" ) ), this );
	forwardBtn->setFocusPolicy( Qt::NoFocus );
	forwardBtn->setFlat( true );

	// Home Button
	homeBtn = new PBButton( icon( PBIconManager::instance()->icon( "go-home" ) ), this );
	homeBtn->setFocusPolicy( Qt::NoFocus );
	homeBtn->setFlat( true );

	// SuperStart Button
	ssBtn = new PBButton( QIcon( ":/icons/superstart.png" ), this );
	ssBtn->setFocusPolicy( Qt::NoFocus );
	ssBtn->setFlat( true );

	PBSegmentButton *navBtns = new PBSegmentButton( this );
	navBtns->addSegment( backBtn );
	navBtns->addSegment( homeBtn );
	navBtns->addSegment( ssBtn );
	navBtns->addSegment( forwardBtn );

	// AddressWidget
	addressWidget = new PBAddressWidget( this );

	// FilterButton
	filterBtn = new PBButton( icon( PBIconManager::instance()->icon( "edit-find" ) ) );
	filterBtn->setObjectName( "base" );
	filterBtn->setFocusPolicy( Qt::NoFocus );

	// Process Widget
	mProcWidget = new PBProcessManagerMini( this );
	mProcWidget->setObjectName( "base" );

	addressWidget->addressEdit->setFocusPolicy( Qt::ClickFocus );
	addressWidget->crumbsBar->setFocusPolicy( Qt::NoFocus );

	/* Layouts */
	fLyt->addWidget( navBtns );
	fLyt->addWidget( Separator::vertical() );
	fLyt->addWidget( addressWidget );
	fLyt->addWidget( Separator::vertical() );
	fLyt->addWidget( filterBtn );
	fLyt->addWidget( Separator::vertical() );
	fLyt->addWidget( mProcWidget );

	QWidget *base = new QWidget( this );
	base->setObjectName( "baseWidget" );
	base->setLayout( fLyt );

	QHBoxLayout *bLyt = new QHBoxLayout();
	bLyt->setContentsMargins( QMargins() );
	bLyt->setSpacing( 0 );
	bLyt->addWidget( base );

	setLayout( bLyt );

	/* Connections */
	connect( backBtn, SIGNAL( clicked() ), this, SIGNAL( goBack() ) );
	connect( homeBtn, SIGNAL( clicked() ), this, SIGNAL( goHome() ) );
	connect( ssBtn, SIGNAL( clicked() ), this, SIGNAL( openSuperStart() ) );
	connect( forwardBtn, SIGNAL( clicked() ), this, SIGNAL( goForward() ) );
	connect( addressWidget, SIGNAL( openLocation( QString ) ), this, SIGNAL( openLocation( QString ) ) );
	connect( filterBtn, SIGNAL( clicked() ), this, SIGNAL( openSearch() ) );
};

void PBAddressBar::updateNavigationButtons( bool bBtn, bool fBtn ) {

	backBtn->setEnabled( bBtn );
	forwardBtn->setEnabled( fBtn );
};

QString PBAddressBar::address() {

	return addressWidget->addressEdit->text();
};

void PBAddressBar::setAddress( QString url ) {

	addressWidget->addressEdit->setText( url );
	addressWidget->crumbsBar->setCurrentDirectory( url );
};

int PBAddressBar::checkedAction() {

	return 1;
};

void PBAddressBar::focusAddressEdit() {

	addressWidget->setFocus();
};

void PBAddressBar::hideSearchButton() {

	filterBtn->setIcon( QIcon() );
};

void PBAddressBar::showSearchButton() {

	filterBtn->setIcon( icon( PBIconManager::instance()->icon( "edit-find" ) ) );
};
