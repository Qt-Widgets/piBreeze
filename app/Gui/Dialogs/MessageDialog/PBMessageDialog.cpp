/*
	*
	* PBMessageDialog.cpp - PBMessageDialog class
	*
*/

#include "PBMessageDialog.hpp"

int PBMessageDialog::information( QWidget *parent, QString title, QString text ) {

	QMessageBox *mbox = new QMessageBox( parent );
	mbox->setIconPixmap( QIcon( ":/icons/info.png" ).pixmap( 64 ) );
	mbox->setWindowTitle( title );
	mbox->setText( text );
	mbox->setStandardButtons( QMessageBox::Ok );
	mbox->setEscapeButton( QMessageBox::Ok );

	return mbox->exec();
};

int PBMessageDialog::question( QWidget *parent, QString title, QString text ) {

	QMessageBox *mbox = new QMessageBox( parent );
	mbox->setIconPixmap( QIcon( ":/icons/question.png" ).pixmap( 64 ) );
	mbox->setWindowTitle( title );
	mbox->setText( text );
	mbox->setStandardButtons( QMessageBox::Yes | QMessageBox::No );
	mbox->setDefaultButton( QMessageBox::No );
	mbox->setEscapeButton( QMessageBox::No );

	return mbox->exec();
};

int PBMessageDialog::warning( QWidget *parent, QString title, QString text ) {

	QMessageBox *mbox = new QMessageBox( parent );
	mbox->setIconPixmap( QIcon( ":/icons/warning.png" ).pixmap( 64 ) );
	mbox->setWindowTitle( title );
	mbox->setText( text );
	mbox->setStandardButtons( QMessageBox::Ok );
	mbox->setEscapeButton( QMessageBox::Ok );

	return mbox->exec();
};

int PBMessageDialog::error( QWidget *parent, QString title, QString text ) {

	QMessageBox *mbox = new QMessageBox( parent );
	mbox->setIconPixmap( QIcon( ":/icons/error.png" ).pixmap( 64 ) );
	mbox->setWindowTitle( title );
	mbox->setText( text );
	mbox->setStandardButtons( QMessageBox::Ok );
	mbox->setEscapeButton( QMessageBox::Ok );

	return mbox->exec();
};

int PBMessageDialog::critical( QWidget *parent, QString title, QString text ) {

	QMessageBox *mbox = new QMessageBox( parent );
	mbox->setIconPixmap( QIcon( ":/icons/stop.png" ).pixmap( 64 ) );
	mbox->setWindowTitle( title );
	mbox->setText( text );
	mbox->setStandardButtons( QMessageBox::Ok );
	mbox->setEscapeButton( QMessageBox::Ok );

	return mbox->exec();
};
