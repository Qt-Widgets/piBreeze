/*
    *
    * * PBTSettingsDialogDialog.cpp - QTermWidget reimplementation for PBTerminal
    *
*/

#include "PBTSettingsDialog.hpp"
#include "qtermwidget.h"

PBTSettingsDialog::PBTSettingsDialog() : QDialog() {

	createGUI();
};

void PBTSettingsDialog::createGUI() {

	QLabel *comboLabel = new QLabel( "&Color Scheme" );
	QLabel *dspinLabel = new QLabel( "Tr&ansparency" );
	QLabel *fontLabel = new QLabel( "&Font" );

	colorSchemesCombo = new QComboBox();
	colorSchemesCombo->addItems( QTermWidget::availableColorSchemes() );
	colorSchemesCombo->setCurrentIndex( QTermWidget::availableColorSchemes().indexOf( settings.value( "ColorScheme" ).toString() ) );
	connect( colorSchemesCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( setColorScheme() ) );

	transparencySpin = new QDoubleSpinBox();
	transparencySpin->setRange( 0.00, 1.00 );
	transparencySpin->setSingleStep( 0.01 );
	transparencySpin->setValue( 1 - settings.value( "Opacity" ).toReal() );
	connect( transparencySpin, SIGNAL( valueChanged( double ) ), this, SLOT( setTransparency() ) );

	fontCombo = new QFontComboBox();
	QFont font = settings.value( "Font" ).value<QFont>();
	fontCombo->setCurrentFont( font );
	fontCombo->setFontFilters( QFontComboBox::MonospacedFonts );
	connect( fontCombo, SIGNAL( currentFontChanged( const QFont& ) ), this, SLOT( setFont( QFont ) ) );

	enableTransparencyCheck = new QCheckBox( "Enable &Transparency" );
	enableTransparencyCheck->setChecked( settings.value( "EnableTransparency" ).toBool() );
	connect( enableTransparencyCheck, SIGNAL( toggled( bool ) ), this, SLOT( setEnableTransparency() ) );

	comboLabel->setBuddy( colorSchemesCombo );
	dspinLabel->setBuddy( transparencySpin );
	fontLabel->setBuddy( fontCombo );

	QPushButton *okBtn = new QPushButton( QIcon::fromTheme( "dialog-close" ), tr( "&Ok" ) );
	connect( okBtn, SIGNAL( clicked() ), this, SLOT( close() ) );

	QGridLayout *lyt = new QGridLayout();
	lyt->addWidget( comboLabel, 0, 0, Qt::AlignLeft );
	lyt->addWidget( colorSchemesCombo, 0, 1, Qt::AlignRight );

	lyt->addWidget( dspinLabel, 1, 0, Qt::AlignLeft );
	lyt->addWidget( transparencySpin, 1, 1, Qt::AlignRight );

	lyt->addWidget( fontLabel, 2, 0, Qt::AlignLeft );
	lyt->addWidget( fontCombo, 2, 1, Qt::AlignRight );

	lyt->addWidget( enableTransparencyCheck, 3, 0, Qt::AlignLeft );

	lyt->addWidget( okBtn, 4, 0, 1, 2, Qt::AlignCenter );

	setLayout( lyt );

	setWindowTitle( "PBTerminal - Settings" );
	setWindowIcon( QIcon( ":/icons/qterminal.png" ) );
};

void PBTSettingsDialog::setColorScheme() {

	QString colorScheme = colorSchemesCombo->currentText();
	settings.setValue( "ColorScheme", colorScheme );
	settings.sync();
};

void PBTSettingsDialog::setFont( QFont font ) {

	settings.setValue( "Font", font );
	settings.sync();
};

void PBTSettingsDialog::setEnableTransparency() {

	settings.setValue( "EnableTransparency", enableTransparencyCheck->isChecked() );
	settings.sync();
};

void PBTSettingsDialog::setTransparency() {

	qreal opacity = 1 - transparencySpin->value();
	settings.setValue( "Opacity", opacity );
	settings.sync();
};
