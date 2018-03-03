/*
	*
	* PBSettingsManager.cpp - PiBreeze Settings
	*
*/

#include "PBSettingsManager.hpp"
#include "PBSPluginsWidget.hpp"

PBSettingsManager::PBSettingsManager( QWidget *parent ) : PBDialog( parent ) {

	createGUI();
	setWindowProperties();
};

void PBSettingsManager::createGUI() {

	// Widget and Layout Base
	QVBoxLayout *guiLyt = new QVBoxLayout();
	guiLyt->addLayout( createBodyLayout() );
	guiLyt->addWidget( Separator::horizontal() );
	guiLyt->addLayout( createFooterLayout() );

	setLayout( guiLyt );
};

QHBoxLayout* PBSettingsManager::createBodyLayout() {

	QHBoxLayout *splitLyt = new QHBoxLayout();
	widgetLyt = new QStackedLayout();

	chooserWidget = new PBSSideBar( this );
	connect( chooserWidget, SIGNAL( loadSettingsCategory( int ) ), this, SLOT( settingCategoryChosen( int ) ) );

	PBSGeneralWidget *generalWidget = new PBSGeneralWidget( this );
	iconWidget = new PBIconThemeWidget( this );
	styleWidget = new PBSStyleWidget( this );
	keysWidget = new PBKeyBindingsWidget( this );

	widgetLyt->addWidget( generalWidget );
	widgetLyt->addWidget( iconWidget );
	widgetLyt->addWidget( styleWidget );
	widgetLyt->addWidget( new PBSPluginsWidget( this ) );
	widgetLyt->addWidget( keysWidget );

	splitLyt->addWidget( chooserWidget );
	splitLyt->addWidget( Separator::vertical( this ) );
	splitLyt->addLayout( widgetLyt );

	return splitLyt;
};

QHBoxLayout* PBSettingsManager::createFooterLayout() {

	QHBoxLayout *btnLyt = new QHBoxLayout();

	PBButton *closeBtn = new PBButton( QIcon( ":/icons/close.png" ), "&Close", this );

	btnLyt->addStretch( 0 );
	btnLyt->addWidget( closeBtn );
	btnLyt->addStretch( 0 );

	connect( closeBtn, SIGNAL( clicked() ), this, SLOT( close() ) );

	return btnLyt;
};

void PBSettingsManager::settingCategoryChosen( int row ) {

	widgetLyt->setCurrentIndex( row );
};

void PBSettingsManager::keyPressEvent( QKeyEvent *kEvent ) {

	if ( kEvent->key() == Qt::Key_Escape )
		close();

	else
		QWidget::keyPressEvent( kEvent );
};

void PBSettingsManager::setWindowProperties() {

	setDialogTitle( tr( "PiBreeze - Settings" ) );
	setDialogIcon( QIcon( ":/icons/openWith.png" ) );

	setWindowModality( Qt::ApplicationModal );

	setFixedWidth( 810 );
	setMinimumHeight( 550 );

	QDesktopWidget dw;
	int hpos = ( int )( ( dw.width() - 810 ) / 2 );
	int vpos = ( int )( ( dw.height() - 480 ) / 2 );
	move( hpos, vpos );
};
