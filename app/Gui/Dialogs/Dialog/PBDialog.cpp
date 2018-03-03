/*
	*
	* PBDialog.cpp - PiBreeze Dialog Class
	*
*/

#include "PBDialog.hpp"

PBDialog::PBDialog( QWidget *parent ) : QDialog( parent ) {

	setupGUI();
	setupDialogProperties();
};

void PBDialog::setupGUI() {

	QAction *closeAct = new QAction( "&Close", this );
	closeAct->setShortcut( QKeySequence( Qt::Key_Escape ) );
	connect( closeAct, SIGNAL( triggered() ), this, SLOT( reject() ) );
	addAction( closeAct );
};

void PBDialog::setupDialogProperties() {

	/* Dialogs without close button */
	setWindowFlags( Qt::Dialog | Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint );

	/* PiBreeze speciality */
	setDialogIcon( QIcon( ":/icons/pibreeze.png" ) );

	setWindowModality( Qt::ApplicationModal);

	/* Size and Size policy */
	setMinimumSize( 500, 75 );
	setSizePolicy( QSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Fixed ) );
};

void PBDialog::setLayout( QLayout *lyt ) {

	lyt->setContentsMargins( QMargins( 5, 5, 5, 5 ) );
	QDialog::setLayout( lyt );
};

void PBDialog::setDialogTitle( QString title ) {

	setWindowTitle( title );
};

void PBDialog::setDialogIcon( QIcon icon ) {

	setWindowIcon( icon );
};

void PBDialog::closeEvent( QCloseEvent *cEvent ) {

	QDialog::closeEvent( cEvent );

	emit closed();
};
