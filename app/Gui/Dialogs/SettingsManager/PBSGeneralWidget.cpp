/*
	*
	* PBSGeneralWidget.cpp - General Settings Widget for PiBreeze
	*
*/

#include "PBSGeneralWidget.hpp"

PBSGeneralWidget::PBSGeneralWidget( QWidget *parent ) : QWidget( parent ) {

	QSettings sett( "PiBreeze", "PiBreeze" );

	/* Grouping */

	perFolderEnabled = new QGroupBox( "&Enabled per-folder view", this );
	perFolderEnabled->setCheckable( true );
	perFolderEnabled->setChecked( sett.value( "PerFolderViews" ).toBool() );
	connect( perFolderEnabled, SIGNAL( toggled( bool ) ), this, SLOT( handlePerFolderChanged( bool ) ) );

	defaultViewModeCB = new QComboBox( this );
	defaultViewModeCB->addItems( QStringList() << "Icons" << "Tiles" << "Details" );
	if ( sett.value( "ViewMode" ).toString().isEmpty() or sett.value( "ViewMode" ).toString() == "Icons" )
		defaultViewModeCB->setCurrentIndex( 0 );

	else if ( sett.value( "ViewMode" ).toString() == "Tiles" )
		defaultViewModeCB->setCurrentIndex( 1 );

	else
		defaultViewModeCB->setCurrentIndex( 2 );

	defaultViewModeCB->setEnabled( false );
	connect( defaultViewModeCB, SIGNAL( currentIndexChanged( int ) ), this, SLOT( handleViewModeChanged( int ) ) );

	iconSizeL = new QLabel( this );
	iconSizeL->setText( QString( "%1 px" ).arg( sett.value( "IconSize" ).toSize().width() ) );

	defaultIconSizeS = new QSlider( Qt::Horizontal, this );
	defaultIconSizeS->setTickPosition( QSlider::TicksBelow );
	defaultIconSizeS->setRange( 16, 128 );
	defaultIconSizeS->setTickInterval( 16 );
	defaultIconSizeS->setSingleStep( 16 );
	defaultIconSizeS->setValue( sett.value( "IconSize" ).toSize().width() );
	connect( defaultIconSizeS, SIGNAL( valueChanged( int ) ), this, SLOT( handleIconSizeChanged( int ) ) );

	defaultGroupingCB = new QCheckBox( "&Group files and folders by default", this );
	defaultGroupingCB->setChecked( sett.value( "Grouping" ).toBool() );
	connect( defaultGroupingCB, SIGNAL( toggled( bool ) ), this, SLOT( handleGroupingChanged( bool ) ) );

	defaultSortCaseCB = new QCheckBox( "Enable c&ase sensitive file and folder sorting", this );
	connect( defaultSortCaseCB, SIGNAL( toggled( bool ) ), this, SLOT( handleSortCaseChanged( bool ) ) );

	defaultSortClmnCB = new QComboBox( this );
	defaultSortClmnCB->addItems( QStringList() << "Name" << "Size" << "Type" << "Date" );
	defaultSortClmnCB->setCurrentIndex( sett.value( "SortColumn" ).toInt() == 3 ? 4 : sett.value( "SortColumn" ).toInt() );
	connect( defaultSortClmnCB, SIGNAL( currentIndexChanged( int ) ), this, SLOT( handleSortColumnChanged( int ) ) );

	/* Others */

	filterFoldersCB = new QCheckBox( "&Filter folder and files", this );
	filterFoldersCB->setChecked( sett.value( "FilterFolders" ).toBool() );
	connect( filterFoldersCB, SIGNAL( toggled( bool ) ), this, SLOT( handleFilterDirsChanged( bool ) ) );

	QHBoxLayout *comboBoxLyt1 = new QHBoxLayout();
	QLabel *lbl1 = new QLabel( "Choose the default &view mode:" );
	lbl1->setBuddy( defaultViewModeCB );
	comboBoxLyt1->addWidget( lbl1 );
	comboBoxLyt1->addWidget( defaultViewModeCB );

	QHBoxLayout *comboBoxLyt2 = new QHBoxLayout();

	QLabel *lbl2 = new QLabel( "Choose the default icon &size:" );
	lbl2->setBuddy( defaultIconSizeS );

	comboBoxLyt2->addWidget( lbl2 );
	comboBoxLyt2->addStretch();
	comboBoxLyt2->addWidget( defaultIconSizeS );
	comboBoxLyt2->addWidget( iconSizeL );

	QHBoxLayout *comboBoxLyt3 = new QHBoxLayout();
	QLabel *lbl3 = new QLabel( "&Sort file and folders by:" );
	lbl3->setBuddy( defaultSortClmnCB );
	comboBoxLyt3->addWidget( lbl3 );
	comboBoxLyt3->addWidget( defaultSortClmnCB );

	showTrayIconCB = new QCheckBox( "&Minimize to tray" );
	showTrayIconCB->setChecked( Settings->General.TrayIcon );
	connect( showTrayIconCB, SIGNAL( toggled( bool ) ), this, SLOT( handleTrayIconChanged( bool ) ) );

	openWithCB = new QCheckBox( "Open with special location when PiBreeze starts?", this );
	openWithCB->setChecked( sett.value( "SpecialOpen" ).toBool() );
	connect( openWithCB, SIGNAL( toggled( bool ) ), this, SLOT( handleOpenWithToggled() ) );

	openWithCatalogRB = new QRadioButton( "Open with Catalog &View", this );
	openWithCatalogRB->setChecked( sett.value( "OpenWithCatalog" ).toBool() );
	connect( openWithCatalogRB, SIGNAL( toggled( bool ) ), this, SLOT( handleOpenWithToggled() ) );

	openWithSuperStartRB = new QRadioButton( "Open with S&uperStart", this );
	openWithSuperStartRB->setChecked( sett.value( "SuperStart" ).toBool() );
	connect( openWithSuperStartRB, SIGNAL( toggled( bool ) ), this, SLOT( handleOpenWithToggled() ) );

	if ( not openWithCB->isChecked() ) {
		openWithCatalogRB->setDisabled( true );
		openWithSuperStartRB->setDisabled( true );
	}

	directIOCB = new QCheckBox( "Enable exten&ded IO" );
	directIOCB->setChecked( Settings->General.DirectIO );
	directIOCB->setToolTip(
		"Extended IO is a replacement for 'Copy/Move to' menu option. When enabled, "
		"a dialog opens where you can choose the target directory to which you want "
		"to copy/move."
	);
	connect( directIOCB, SIGNAL( toggled( bool ) ), this, SLOT( handleDirectIOChanged( bool ) ) );

	QVBoxLayout *grpLyt = new QVBoxLayout( this );
	grpLyt->addLayout( comboBoxLyt1 );
	grpLyt->addLayout( comboBoxLyt2 );
	grpLyt->addWidget( defaultGroupingCB );
	grpLyt->addWidget( defaultSortCaseCB );
	grpLyt->addLayout( comboBoxLyt3 );

	perFolderEnabled->setLayout( grpLyt );

	QHBoxLayout *rbLyt = new QHBoxLayout();
	rbLyt->setContentsMargins( QMargins( 20, 0, 0, 0 ) );
	rbLyt->addWidget( openWithCatalogRB );
	rbLyt->addWidget( openWithSuperStartRB );

	QVBoxLayout *otherGBLyt = new QVBoxLayout();
	otherGBLyt->addWidget( filterFoldersCB );
	otherGBLyt->addWidget( showTrayIconCB );
	otherGBLyt->addWidget( openWithCB );
	otherGBLyt->addLayout( rbLyt );
	otherGBLyt->addWidget( directIOCB );

	QGroupBox *otherOptionsGB = new QGroupBox( "Other Options", this );
	otherOptionsGB->setLayout( otherGBLyt );

	QVBoxLayout *lyt = new QVBoxLayout();
	lyt->addWidget( perFolderEnabled );
	lyt->addWidget( otherOptionsGB );
	lyt->addStretch();

	setLayout( lyt );
};

void PBSGeneralWidget::handlePerFolderChanged( bool perFolder ) {

	Settings->setValue( "PerFolderViews", perFolder );
	Settings->General.PerFolderViews = perFolder;
};

void PBSGeneralWidget::handleIconSizeChanged( int ) {

	int iconSize = defaultIconSizeS->value();
	Settings->setValue( "IconSize", QSize( iconSize, iconSize ) );
	Settings->General.IconSize = QSize( iconSize, iconSize );

	iconSizeL->setText( QString( "%1 px" ).arg( iconSize ) );
};

void PBSGeneralWidget::handleGroupingChanged( bool grouping ) {

	Settings->setValue( "Grouping", grouping );
	Settings->General.Grouping = grouping;
};

void PBSGeneralWidget::handleSortCaseChanged( bool sortCase ) {

	Settings->setValue( "SortCase", sortCase );
	Settings->General.SortCase = sortCase;
};

void PBSGeneralWidget::handleSortColumnChanged( int sortColumn ) {

	switch( sortColumn ) {
		case 0: {
			Settings->setValue( "SortColumn", 0 );
			Settings->General.SortColumn = 0;
			break;
		}

		case 1:{
			Settings->setValue( "SortColumn", 1 );
			Settings->General.SortColumn = 1;
			break;
		}

		case 2:{
			Settings->setValue( "SortColumn", 2 );
			Settings->General.SortColumn = 2;
			break;
		}

		case 3:{
			Settings->setValue( "SortColumn", 4 );
			Settings->General.SortColumn = 4;
			break;
		}

		default: {
			if ( Settings->General.Grouping ) {
				Settings->setValue( "SortColumn", 2 );
				Settings->General.SortColumn = 2;
			}

			else {
				Settings->setValue( "SortColumn", 0 );
				Settings->General.SortColumn = 0;
			}
			break;
		}
	}

};

void PBSGeneralWidget::handleFilterDirsChanged( bool filterFolders ) {

	Settings->setValue( "FilterFolders", filterFolders );
	Settings->General.FilterFolders = filterFolders;
};

void PBSGeneralWidget::handleOpenWithToggled() {

	if ( qobject_cast<QCheckBox*>( sender() ) == openWithCB ) {
		if ( openWithCB->isChecked() ) {
			Settings->setValue( "SpecialOpen", true );
			Settings->General.SpecialOpen = true;

			QSettings gSettings( "PiBreeze", "PiBreeze" );

			Settings->General.OpenWithCatalog = gSettings.value( "OpenWithCatalog", false ).toBool();
			Settings->General.SuperStart = gSettings.value( "SuperStart", false ).toBool();

			openWithCatalogRB->setEnabled( true );
			openWithSuperStartRB->setEnabled( true );
		}

		else {
			Settings->setValue( "SpecialOpen", false );
			Settings->General.SpecialOpen = false;

			Settings->General.OpenWithCatalog = false;
			Settings->General.SuperStart = false;

			openWithCatalogRB->setDisabled( true );
			openWithSuperStartRB->setDisabled( true );
		}
	}

	else if ( qobject_cast<QRadioButton*>( sender() ) == openWithCatalogRB ) {
		Settings->setValue( "OpenWithCatalog", true );
		Settings->setValue( "SuperStart", false );

		Settings->General.OpenWithCatalog = true;
		Settings->General.SuperStart = false;
	}

	else if ( qobject_cast<QRadioButton*>( sender() ) == openWithSuperStartRB ) {
		Settings->setValue( "OpenWithCatalog", false );
		Settings->setValue( "SuperStart", true );

		Settings->General.OpenWithCatalog = false;
		Settings->General.SuperStart = true;
	}
};

void PBSGeneralWidget::handleTrayIconChanged( bool value ) {

	Settings->setValue( "TrayIcon", value );
	Settings->General.TrayIcon = value;
};

void PBSGeneralWidget::handleDirectIOChanged( bool enabled ) {

	Settings->setValue( "DirectIO", enabled );
	Settings->General.DirectIO = enabled;
};
