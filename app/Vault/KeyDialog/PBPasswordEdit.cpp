/*
	*
	* PBPasswordEdit.cpp - Password Input Dialog Class
	*
*/

#include "PBPasswordEdit.hpp"

QString stylesheet = QString(
	"QLabel {"
	"	border: 1px solid %1;"
	"	border-radius: 2px;"
	"	background-color: white;"
	"}"
	"QLabel:disabled {"
	"	background-color: transparent;"
	"}"
);

PBPasswordEdit::PBPasswordEdit( QWidget *parent ) : QLabel( parent ) {

	QLabel::setText( "<font color='darkgray'>Click to set the password</font>" );

	setAlignment( Qt::AlignCenter );
	setFixedHeight( 32 );

	setStyleSheet( stylesheet.arg( "lightgray") );

	setMouseTracking( true );
};

QString PBPasswordEdit::text() {

	return mText;
};

void PBPasswordEdit::setText( QString txt ) {

	if ( not txt.count() ) {

		mText.clear();

		QLabel::setText( "<font color='darkgray'>Click to set the password</font>" );
		return;
	}

	mText = txt;

	QLabel::setText( QString::fromUtf8( "\u2B22\u2B22\u2B22\u2B22\u2B22\u2B22\u2B22\u2B22\u2B22\u2B22" ) );
	emit passwordSet();
};

void PBPasswordEdit::setOkay() {

	setDisabled( true );
	QLabel::setText( QString::fromUtf8( "<font color='darkgreen'><b>Password Validated ✓</b></font>" ) );
};

void PBPasswordEdit::clear() {

	setEnabled( true );
	QLabel::setText( "<font color='darkgray'>Click to set the password</font>" );
};

void PBPasswordEdit::mousePressEvent( QMouseEvent *mEvent ) {

	mEvent->accept();
	setStyleSheet( stylesheet.arg( "#7EB6FF" ) );
};

void PBPasswordEdit::mouseReleaseEvent( QMouseEvent *mEvent ) {

	if ( not rect().contains( mEvent->pos() ) ) {
		mEvent->accept();
		return;
	}

	PBPasswordInput *pInput = new PBPasswordInput( qobject_cast<QWidget*>( parent() ) );
	if ( pInput->exec() == QDialog::Accepted ) {

		setText( pInput->password() );
		pInput->clear();
	}

	setStyleSheet( stylesheet.arg( "lightgray") );
	mEvent->accept();
};
