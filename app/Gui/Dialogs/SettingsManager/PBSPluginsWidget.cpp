/*
	*
	* PBSPluginsWidget.cpp - Plugins Manager for PiBreeze
	*
*/

#include "PBSPluginsWidget.hpp"

PBSPluginsWidget::PBSPluginsWidget( QWidget *parent ) : QWidget( parent ) {

	createGUI();
};

void PBSPluginsWidget::createGUI() {

	// ListWidget for Plugin Paths
	pPathsLW = new QListWidget( this );

	// Populating the pPathsLW
	QSettings nbsett( "PiBreeze", "Plugins" );

	QStringList pluginPaths;
	if ( not nbsett.value( "PluginPaths" ).toStringList().count() )
		nbsett.setValue( "PluginPaths", QStringList() << "/usr/lib/pibreeze/plugins" << PBXdg::home() + ".config/PiBreeze/plugins/" );
	pPathsLW->addItems( nbsett.value( "PluginPaths" ).toStringList() );

	pathsBtn = new PBSegmentButton( this );
	pathsBtn->setCount( 2 );

	pathsBtn->setSegmentIcon( 0, QIcon::fromTheme( "list-add" ) );
	pathsBtn->setSegmentText( 0, "&Add Path" );

	pathsBtn->setSegmentIcon( 1, QIcon::fromTheme( "list-remove" ) );
	pathsBtn->setSegmentText( 1, "&Remove Path" );

	connect( pathsBtn, SIGNAL( clicked( int ) ), this, SLOT( buttonClick( int ) ) );

	QHBoxLayout *btnLyt = new QHBoxLayout();
	btnLyt->addStretch();
	btnLyt->addWidget( pathsBtn );
	btnLyt->addStretch();

	QVBoxLayout *baseLyt = new QVBoxLayout();
	baseLyt->addWidget( new QLabel( "<p><large>Plugin search paths</large></p><p>PiBreeze will search the following paths for plugins:<p>", this ) );
	baseLyt->addWidget( pPathsLW );
	baseLyt->addLayout( btnLyt );

	setLayout( baseLyt );
};

void PBSPluginsWidget::buttonClick( int b ) {

	if ( b == 0 )
		addPath();

	else
		removePath();
};

void PBSPluginsWidget::addPath() {

	QString path = PBDirectoryDialog::getDirectoryName( this, "Plugin Search Path", "/" );

	if ( path.count() ) {
		pPathsLW->addItem( path );
		updateSettings();
	}
};

void PBSPluginsWidget::removePath() {

	int curRow = pPathsLW->currentRow();
	if ( pPathsLW->selectedItems().contains( pPathsLW->item( curRow ) ) ) {
		pPathsLW->takeItem( curRow );
		updateSettings();
	}
};

void PBSPluginsWidget::updateSettings() {

	QStringList paths;
	for( int i = 0; i < pPathsLW->count(); i++ )
		paths << pPathsLW->item( i )->text();

	QSettings nbsett( "PiBreeze", "PiBreeze" );

	QStringList pluginPaths;
	if ( not paths.count() )
		nbsett.setValue( "PluginPaths", QStringList() << "/usr/lib/pibreeze/plugins" << PBXdg::home() + ".config/PiBreeze/plugins/" );

	else
		nbsett.setValue( "PluginPaths", paths );

	pPathsLW->clear();
	pPathsLW->addItems( nbsett.value( "PluginPaths" ).toStringList() );
};
