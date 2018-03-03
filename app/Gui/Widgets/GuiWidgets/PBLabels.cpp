/*
	*
	* PBLabels.cpp - Different types of customized labels for PiBreeze
	*
*/

#include "PBLabels.hpp"

PBClickLabel::PBClickLabel( QPixmap icon, QWidget *parent ) : QLabel( parent ) {

	clickEnabled = true;

	setPixmap( icon );
	setAlignment( Qt::AlignCenter );

	setStyleSheet( "QLabel{border: 1px solid gray; border-radius: 3px; padding: 5px;}" );
};

PBClickLabel::PBClickLabel( QString text, QWidget *parent ) : QLabel( parent ) {

	clickEnabled = true;

	setText( text );
	setAlignment( Qt::AlignCenter );
};

void PBClickLabel::mousePressEvent( QMouseEvent *mEvent ) {

	if ( clickEnabled )
		emit pressed();

	mEvent->accept();
};

void PBClickLabel::mouseReleaseEvent( QMouseEvent *mEvent ) {

	if ( clickEnabled and rect().contains( mEvent->pos() ) ) {
		emit clicked();
		emit released();
	}

	mEvent->accept();
};

void PBClickLabel::setClickable( bool canClick ) {

	clickEnabled = canClick;
};

PBInfoLabel::PBInfoLabel() : QLabel() {

	setAlignment( Qt::AlignCenter );
	setFont( QFont( "DejaVu Sans Mono", 9 ) );
};

void PBInfoLabel::setText( QString name, QString size, QString type, QString perm ) {

	QString text = QString(
		"<table style='width: 700px; height: 56px;'>"			\
		"	<tr>"												\
		"		<td colspan=3 align = 'left'>%1</td>"			\
		"	</tr>"												\
		"	<tr>"												\
		"		<td width=300 align='left'>%2</td>"				\
		"		<td width=300 align='left'>%3</td>"				\
		"		<td width=100 align='center'>%4</td>"			\
		"	</tr>"												\
		"</table>"
	);

	QLabel::setText( text.arg( name ).arg( size ).arg( type ).arg( perm ) );
};
