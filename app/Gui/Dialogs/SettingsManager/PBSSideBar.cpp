/*
	*
	* PBSSideBar.hpp - SideBar for Settings Dialog
	*
*/

#include "PBSSideBar.hpp"

PBSSideBar::PBSSideBar( QWidget *parent ) : QWidget( parent ) {

	setFixedWidth( 100 );

	QToolButton *generalBtn = new QToolButton();
	generalBtn->setFixedSize( 81, 81 );
	generalBtn->setIconSize( QSize( 48, 48 ) );
	generalBtn->setIcon( QIcon( ":/icons/pibreeze.png" ) );
	generalBtn->setText( "General" );
	generalBtn->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
	generalBtn->setCheckable( true );
	generalBtn->setFocusPolicy( Qt::NoFocus );
	generalBtn->setShortcut( tr( "Ctrl+1" ) );

	QToolButton *iconsBtn = new QToolButton();
	iconsBtn->setFixedSize( 81, 81 );
	iconsBtn->setIconSize( QSize( 48, 48 ) );
	iconsBtn->setIcon( QIcon( ":/icons/iconthemes.png" ) );
	iconsBtn->setText( "Icons" );
	iconsBtn->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
	iconsBtn->setCheckable( true );
	iconsBtn->setFocusPolicy( Qt::NoFocus );
	iconsBtn->setShortcut( tr( "Ctrl+2" ) );

	QToolButton *styleBtn = new QToolButton();
	styleBtn->setFixedSize( 81, 81 );
	styleBtn->setIconSize( QSize( 48, 48 ) );
	styleBtn->setIcon( QIcon( ":/icons/appearance.png" ) );
	styleBtn->setText( "Style" );
	styleBtn->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
	styleBtn->setCheckable( true );
	styleBtn->setFocusPolicy( Qt::NoFocus );
	styleBtn->setShortcut( tr( "Ctrl+3" ) );

	QToolButton *pluginsBtn = new QToolButton();
	pluginsBtn->setFixedSize( 81, 81 );
	pluginsBtn->setIconSize( QSize( 48, 48 ) );
	pluginsBtn->setIcon( QIcon( ":/icons/plugins.png" ) );
	pluginsBtn->setText( "Plugins" );
	pluginsBtn->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
	pluginsBtn->setCheckable( true );
	pluginsBtn->setFocusPolicy( Qt::NoFocus );
	pluginsBtn->setShortcut( tr( "Ctrl+4" ) );

	QToolButton *keyBindingsBtn = new QToolButton();
	keyBindingsBtn->setFixedSize( 81, 81 );
	keyBindingsBtn->setIconSize( QSize( 48, 48 ) );
	keyBindingsBtn->setIcon( QIcon( ":/icons/keybindings.png" ) );
	keyBindingsBtn->setText( "Key Bindings" );
	keyBindingsBtn->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
	keyBindingsBtn->setCheckable( true );
	keyBindingsBtn->setFocusPolicy( Qt::NoFocus );
	keyBindingsBtn->setShortcut( tr( "Ctrl+5" ) );

	QVBoxLayout *baseLyt = new QVBoxLayout();
	baseLyt->addWidget( generalBtn );
	baseLyt->addWidget( iconsBtn );
	baseLyt->addWidget( styleBtn );
	baseLyt->addWidget( pluginsBtn );
	baseLyt->addWidget( keyBindingsBtn );

	setLayout( baseLyt );

	QButtonGroup *btnGrp = new QButtonGroup();
	btnGrp->setExclusive( true );
	btnGrp->addButton( generalBtn, 0 );
	btnGrp->addButton( iconsBtn, 1 );
	btnGrp->addButton( styleBtn, 2 );
	btnGrp->addButton( pluginsBtn, 3 );
	btnGrp->addButton( keyBindingsBtn, 4 );

	connect( btnGrp, SIGNAL( buttonClicked( int ) ), this, SIGNAL( loadSettingsCategory( int ) ) );

	generalBtn->setChecked( true );
};
