/*
	*
	* PBButton.cpp - Alternate PushButton/ToolButton for PiBreeze
	*
*/

#include "PBButton.hpp"

PBButton::PBButton( QWidget *parent ) : QPushButton( parent ) {

	initPrivateVariables();
};

PBButton::PBButton( QIcon icon, QWidget *parent ) : QPushButton( parent ) {

	initPrivateVariables();

	/* Set the icon */
	setIcon( icon );
};

PBButton::PBButton( QString text, QWidget *parent ) : QPushButton( parent ) {

	initPrivateVariables();

	/* Set the text */
	setText( text );
};

PBButton::PBButton( QIcon icon, QString text, QWidget *parent ) : QPushButton( parent ) {

	initPrivateVariables();

	/* Set the icon */
	setIcon( icon );

	/* Set the text */
	setText( text );
};

QIcon PBButton::icon() {

	return mIcon;
};

void PBButton::setIcon( QIcon icon ) {

	mIcon = icon;
	QPushButton::setIcon( icon );

	if ( mText.isEmpty() ) {
		/* Its a toolbutton; default size: 28x28 */
		mHeight = qMax( mIconSize.height() + 8, 28 );
		mWidth = qMax( mIconSize.width() + 8, 28 );
	}

	else {
		/* Its a pushbutton with default size: 100x28 */
		mHeight = qMax( mIconSize.height() + 8, 28 );
		mWidth = 100;

		/* Make sure the size is larger than the text: Buffer 10 */
		QFontMetrics fm( font() );
		mWidth = qMax( fm.size( Qt::TextShowMnemonic, mText ).width() + mHeight + 10, 100 );
	}

	setFixedSize( QSize( mWidth, mHeight ) );
};

QString PBButton::text() {

	return mText;
};

void PBButton::setText( QString text ) {

	mText = text;
	QPushButton::setText( text );

	if ( text.isEmpty() ) {
		/* Its a toolbutton; default size: 28x28 */
		mHeight = qMax( mIconSize.height() + 8, 28 );
		mWidth = qMax( mIconSize.width() + 8, 28 );
	}

	else {
		/* Its a pushbutton with default size: 120x24 */
		mHeight = qMax( mIconSize.height() + 8, 28 );

		/* Make sure the size is larger than the text: Buffer 10 */
		QFontMetrics fm( font() );
		mWidth = qMax( fm.size( Qt::TextShowMnemonic, mText ).width() + mHeight + 10, 100 );
	}

	setFixedSize( QSize( mWidth, mHeight ) );
};

QSize PBButton::iconSize() {

	return mIconSize;
};

void PBButton::setIconSize( QSize iconSize ) {

	mIconSize = iconSize;

	if ( mText.isEmpty() ) {
		/* Its a toolbutton; default size: 28x28 */
		mHeight = qMax( mIconSize.height() + 8, 28 );
		mWidth = qMax( mIconSize.width() + 8, 28 );
	}

	else {
		/* Its a pushbutton with default size: 120x24 */
		mHeight = qMax( mIconSize.height() + 8, 28 );
		mWidth = 100;

		/* Make sure the size is larger than the text: Buffer 10 */
		QFontMetrics fm( font() );
		mWidth = qMax( fm.size( Qt::TextShowMnemonic, mText ).width() + mHeight + 10, 100 );
	}

	setFixedSize( QSize( mWidth, mHeight ) );
	QPushButton::setIconSize( iconSize );
};

QSize PBButton::size() {

	return QSize( mWidth, mHeight );
};

QSize PBButton::sizeHint() const {

	return QSize( mWidth, mHeight );
};

void PBButton::setObjectName( QString objName ) {

	QPushButton::setObjectName( objName );

	if ( objName == "okBtn" )
		mButtonColor = QColor( 37, 174, 96 );

	else if ( objName == "cancelBtn" )
		// mButtonColor = QColor( 218, 68, 83 );
		mButtonColor = QColor( 246, 116, 0 );

	else if ( objName == "abortBtn" )
		mButtonColor = QColor( 180, 10, 10 );

	else
		mButtonColor = QColor( 61, 174, 233 );

	setButtonStyle();
};

int PBButton::segment() {

	return mSegment;
};

void PBButton::setSegment( PBButton::Segment seg ) {

	mSegment = seg;

	radius.clear();
	/* TLR, BLR, TRR, BRR */
	switch( seg ) {
		case PBButton::FirstSegment: {
			radius << "4px" << "4px" << "0px" << "0px";
			leftBorderSize = "0px";
			break;
		}
		case PBButton::MiddleSegment: {
			radius << "0px" << "0px" << "0px" << "0px";
			leftBorderSize = "1px";
			break;
		}
		case PBButton::LastSegment: {
			radius << "0px" << "0px" << "4px" << "4px";
			leftBorderSize = "1px";
			break;
		}
		case PBButton::SingleButton: {
			radius << "4px" << "4px" << "4px" << "4px";
			leftBorderSize = "0px";
			break;
		}
	}

	setButtonStyle();
};

void PBButton::setButtonStyle() {
	/*
		*
		* This is a way to generate button stylesheet. It will be used to colorize special buttons: Cancel, Ok, Abort etc
		* The buttons are modelled to appear and behave similar to Breeze style, Plasma 5.
		*
	*/

	/* color: General color of the button */
	/* tlc: Top left corner radius */
	/* trc: Top right corner radius */
	/* blc: Bottom left corner radius */
	/* brc: Bottom right corner radius */

	QString styleSkel = QString( "QPushButton {"
		"    border: none;"
		"    border-left: <LBS> solid qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop:0 transparent, stop:0.4 lightgray, stop:0.6 lightgray, stop:1 transparent);"
		"    border-top-left-radius: <TLR>;"
		"    border-bottom-left-radius: <BLR>;"
		"    border-top-right-radius: <TRR>;"
		"    border-bottom-right-radius: <BRR>;"
		"}"
		"QPushButton:focus {"
		"    border: 2px solid <Border>;"
		"    color: <Text>;"
		"    font-weight : bold;"
		"}"
		"QPushButton:hover {"
		"    background-color: <Hover>;"
		"}"
		"QPushButton:pressed {"
		"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop:0 <Edge>, stop:0.4 <Middle>, stop:0.6 <Middle>, stop:1 <Edge>);"
		"}"
		"QPushButton:checked {"
		"    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop:0 <Gray>, stop:0.4 <Gray>, stop:0.6 <Gray>, stop:1 <Gray>);"
		"}"
	);

	/* Border color */
	QString colorSkelB = QString( "rgba(%1, %2, %3, 150)" ).arg( mButtonColor.red() ).arg( mButtonColor.green() ).arg( mButtonColor.blue() );
	/* Button edge highlight color */
	QString colorSkelE = QString( "rgba(%1, %2, %3, 100)" ).arg( mButtonColor.red() ).arg( mButtonColor.green() ).arg( mButtonColor.blue() );
	/* Buton middle highlight color */
	QString colorSkelM = QString( "rgba(%1, %2, %3, 180)" ).arg( mButtonColor.red() ).arg( mButtonColor.green() ).arg( mButtonColor.blue() );
	/* Buton hover color */
	QString colorSkelH = QString( "rgba(%1, %2, %3, 100)" ).arg( mButtonColor.red() ).arg( mButtonColor.green() ).arg( mButtonColor.blue() );
	/* Button text */
	QString colorSkelT = QString( "rgba(%1, %2, %3, 255)" ).arg( mButtonColor.red() ).arg( mButtonColor.green() ).arg( mButtonColor.blue() );
	/* Checked button */
	QString colorSkelG = QString( "rgba(%1, %1, %1, 255)" ).arg( 0.21 * mButtonColor.red() + 0.72 * mButtonColor.green() + 0.07 * mButtonColor.blue() );

	styleSkel = styleSkel.replace( "<TLR>", radius.at( 0 ) ).replace( "<BLR>", radius.at( 1 ) ).replace( "<TRR>", radius.at( 2 ) ).replace( "<BRR>", radius.at( 3 ) );
	styleSkel = styleSkel.replace( "<Border>", colorSkelB );
	styleSkel = styleSkel.replace( "<Text>", colorSkelT );
	styleSkel = styleSkel.replace( "<Edge>", colorSkelE );
	styleSkel = styleSkel.replace( "<Middle>", colorSkelM );
	styleSkel = styleSkel.replace( "<Hover>", colorSkelH );
	styleSkel = styleSkel.replace( "<Gray>", colorSkelG );
	styleSkel = styleSkel.replace( "<LBS>", leftBorderSize );

	QPushButton::setStyleSheet( styleSkel );
};

void PBButton::initPrivateVariables() {

	/* Icon Size */
	mIconSize = QSize( 16, 16 );
	setIconSize( mIconSize );

	/* Button Size: Default tool button */
	mHeight = 28,
	mWidth = 28;
	setFixedSize( QSize( 28, 28 ) );

	/* Size policy: Always fixed */
	setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );

	/* Flat Buttons */
	setFlat( true );

	/* Button Color */
	mButtonColor = QColor( 61, 174, 233 );
	mSegment = PBButton::SingleButton;
	radius << "4px" << "4px" << "4px" << "4px";

	/* Set the default button style */
	setButtonStyle();

	/* No focus by default: Do we need this? */
};
