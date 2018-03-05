/*
    *
    * * TtyTabWidget.cpp - TabWidget to hold Pty for PBTerminal
    *
*/

#include "TtyTabWidget.hpp"
#include "PBTermWidget.hpp"

TtyTabWidget::TtyTabWidget( QWidget *parent, bool mode ) : QTabWidget( parent ) {

	/* DropDownMode */
	ddMode = mode;

	setMovable( true );
	setTabsClosable( true );
	setDocumentMode( true );

	setFocusPolicy( Qt::NoFocus );

	connect( this, SIGNAL( tabCloseRequested( int ) ), this, SLOT( closeTab( int ) ) );

	/* Terminal Icon on top left corner */
	QLabel *iconLbl = new QLabel( this );
	iconLbl->setPixmap( QIcon::fromTheme( "utilities-terminal" ).pixmap( 24 ) );

	setTabPosition( QTabWidget::North );
	setCornerWidget( iconLbl, Qt::TopLeftCorner );
	setStyleSheet( getStyleSheet( "PBTabWidget" ) );
};

int TtyTabWidget::newTerminal() {

	PBTermWidget *widget = new PBTermWidget( this );
	widget->setFocusPolicy( Qt::StrongFocus );

	int idx = addTab( widget, QIcon::fromTheme( "utilities-terminal" ), "" );
	setTabText( idx, QString( "Terminal %1" ).arg( idx + 1 ) );
	connect( widget, SIGNAL( finished() ), this, SLOT( closeTab() ) );

	setCurrentIndex( idx );

	widget->setFocus();

	return idx;
};

int TtyTabWidget::newTerminal( QString wDir, QString cmd ) {

	PBTermWidget *widget = new PBTermWidget( wDir, cmd, this );
	widget->setFocusPolicy( Qt::StrongFocus );

	int idx = addTab( widget, QIcon::fromTheme( "utilities-terminal" ), "" );
	setTabText( idx, QString( "Terminal %1" ).arg( idx + 1 ) );

	connect( widget, SIGNAL( finished() ), this, SLOT( closeTab() ) );

	setCurrentIndex( idx );

	widget->setFocus();

	return idx;
};

int TtyTabWidget::newTerminalCWD() {

	PBTermWidget *curTerm = qobject_cast<PBTermWidget *>( currentWidget() );
	QString cwd = curTerm->currentWorkingDirectory();

	return newTerminal( cwd, "" );
};

void TtyTabWidget::clearTerminal() {

	PBTermWidget *widget = qobject_cast<PBTermWidget *>( currentWidget() );
	widget->clear();
};

void TtyTabWidget::copyToClipboard() {

	PBTermWidget *widget = qobject_cast<PBTermWidget *>( currentWidget() );
	widget->copyClipboard();
};

void TtyTabWidget::pasteClipboard() {

	PBTermWidget *widget = qobject_cast<PBTermWidget *>( currentWidget() );
	widget->pasteClipboard();
};

void TtyTabWidget::closeTab( int tabIndex ) {

	removeTab( tabIndex );

	if ( count() == 0 ) {
		if ( ddMode )
			newTerminal();

		else
			emit close();
	}

	else {
		for( int idx = 0; idx < count(); idx++ )
			setTabText( idx, QString( "Terminal %1" ).arg( idx + 1 ) );
	}
};

void TtyTabWidget::prevTerminal() {

	int idx = currentIndex();
	if ( idx == 0 )
		setCurrentIndex( count() - 1 );

	else
		setCurrentIndex( idx - 1 );
};

void TtyTabWidget::nextTerminal() {

	int idx = currentIndex();
	if ( idx == count() - 1 )
		setCurrentIndex( 0 );

	else
		setCurrentIndex( idx + 1 );
};

void TtyTabWidget::closeTab() {

	if ( qobject_cast<PBTermWidget*>( sender() ) )
		removeTab( indexOf( qobject_cast<PBTermWidget*>( sender() ) ) );

	else
		removeTab( currentIndex() );

	if ( count() == 0 ) {
		if ( ddMode )
			newTerminal();

		else
			emit close();
	}

	else {
		for( int idx = 0; idx < count(); idx++ )
			setTabText( idx, QString( "Terminal %1" ).arg( idx + 1 ) );
	}
};

void TtyTabWidget::printSelection( bool selection ) {

	PBTermWidget *widget = qobject_cast<PBTermWidget *>( currentWidget() );
	widget->copyClipboard();

	QClipboard *klipper = qApp->clipboard();
	qDebug() << klipper->text();

	if ( selection ) {
		widget->pasteSelection();
	}
};
