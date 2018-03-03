/*
	*
	* PBSegmentButton.cpp - The Segmented Buttons Class
	*
*/

#include "PBButtons.hpp"

PBSegmentButton::PBSegmentButton( QWidget *parent ) : QWidget( parent ) {

	btnGrp = new QButtonGroup();

	myButtonHeight = 28;
	myWidgetWidth = 28;
	btnGrp = new QButtonGroup();

	myButtonHeight = 28;
	myWidgetWidth = 28;
	setContentsMargins( QMargins() );

	btnsLyt = new QHBoxLayout();
	btnsLyt->setContentsMargins( QMargins() );
	btnsLyt->setSpacing( 0 );

	QWidget *btnsBase = new QWidget( this );
	btnsBase->setObjectName( "base" );
	btnsBase->setLayout( btnsLyt );

	QHBoxLayout *baseLyt = new QHBoxLayout();
	baseLyt->setContentsMargins( QMargins() );
	baseLyt->setSpacing( 0 );
	baseLyt->setAlignment( Qt::AlignCenter );
	baseLyt->addWidget( btnsBase );

	setLayout( baseLyt );

	/* Internal styling hack: until we learn to draw smooth rounded rects */
	setStyleSheet( "QWidget#base { border: 1px solid lightgray; border-radius: 4px; }" );
};

PBSegmentButton::PBSegmentButton( QList<QIcon> icons, QWidget *parent ) : QWidget( parent ) {

	btnGrp = new QButtonGroup();

	myButtonHeight = 28;
	myWidgetWidth = 28;
	setContentsMargins( QMargins() );

	btnsLyt = new QHBoxLayout();
	btnsLyt->setContentsMargins( QMargins() );
	btnsLyt->setSpacing( 0 );

	segments.clear();
	int max = icons.count();
	for( int i = 0; i < max; i++ ) {
		PBButton *btn = new PBButton( icons.value( i ), this );
		connect( btn, SIGNAL( clicked() ), this, SLOT( handleSegmentClick() ) );

		segments << btn;
		btnGrp->addButton( btn );
		btnsLyt->addWidget( btn );
	}

	QWidget *btnsBase = new QWidget( this );
	btnsBase->setObjectName( "base" );
	btnsBase->setLayout( btnsLyt );

	QHBoxLayout *baseLyt = new QHBoxLayout();
	baseLyt->setContentsMargins( QMargins() );
	baseLyt->setSpacing( 0 );
	baseLyt->setSpacing( 0 );
	baseLyt->setAlignment( Qt::AlignCenter );
	baseLyt->addWidget( btnsBase );

	setLayout( baseLyt );

	/* Internal styling hack: until we learn to draw smooth rounded rects */
	setStyleSheet( "QWidget#base { border: 1px solid lightgray; border-radius: 4px; }" );
	resetStyleSheets();

	/* AutoResize */
	autoResize();
};

PBSegmentButton::PBSegmentButton( QStringList labels, QWidget *parent ) : QWidget( parent ) {

	btnGrp = new QButtonGroup();

	myButtonHeight = 28;
	myWidgetWidth = 28;
	setContentsMargins( QMargins() );

	btnsLyt = new QHBoxLayout();
	btnsLyt->setContentsMargins( QMargins() );
	btnsLyt->setSpacing( 0 );

	segments.clear();
	int max = labels.count();
	for( int i = 0; i < max; i++ ) {
		PBButton *btn = new PBButton( labels.value( i ), this );
		connect( btn, SIGNAL( clicked() ), this, SLOT( handleSegmentClick() ) );

		segments << btn;
		btnGrp->addButton( btn );
		btnsLyt->addWidget( btn );
	}

	QWidget *btnsBase = new QWidget( this );
	btnsBase->setObjectName( "base" );
	btnsBase->setLayout( btnsLyt );

	QHBoxLayout *baseLyt = new QHBoxLayout();
	baseLyt->setContentsMargins( QMargins() );
	baseLyt->setSpacing( 0 );
	baseLyt->setAlignment( Qt::AlignCenter );
	baseLyt->addWidget( btnsBase );

	setLayout( baseLyt );

	/* Internal styling hack: until we learn to draw smooth rounded rects */
	setStyleSheet( "QWidget#base { border: 1px solid lightgray; border-radius: 4px; }" );
	resetStyleSheets();

	/* AutoResize */
	autoResize();
};

PBSegmentButton::PBSegmentButton( QStringList labels, QList<QIcon> icons, QWidget *parent ) : QWidget( parent ) {

	btnGrp = new QButtonGroup();

	myButtonHeight = 28;
	myWidgetWidth = 28;
	setContentsMargins( QMargins() );

	btnsLyt = new QHBoxLayout();
	btnsLyt->setContentsMargins( QMargins() );
	btnsLyt->setSpacing( 0 );

	segments.clear();
	int max = qMax( labels.count(), icons.count() );
	for( int i = 0; i < max; i++ ) {
		PBButton *btn = new PBButton( icons.value( i ), labels.value( i ), this );
		connect( btn, SIGNAL( clicked() ), this, SLOT( handleSegmentClick() ) );

		segments << btn;
		btnGrp->addButton( btn );
		btnsLyt->addWidget( btn );
	}

	QWidget *btnsBase = new QWidget( this );
	btnsBase->setObjectName( "base" );
	btnsBase->setLayout( btnsLyt );

	QHBoxLayout *baseLyt = new QHBoxLayout();
	baseLyt->setContentsMargins( QMargins() );
	baseLyt->setSpacing( 0 );
	baseLyt->setAlignment( Qt::AlignCenter );
	baseLyt->addWidget( btnsBase );

	setLayout( baseLyt );

	/* Internal styling hack: until we learn to draw smooth rounded rects */
	setStyleSheet( "QWidget#base { border: 1px solid lightgray; border-radius: 4px; }" );
	resetStyleSheets();

	/* AutoResize */
	autoResize();
};

int PBSegmentButton::count() {

	return segments.count();
};

void PBSegmentButton::setCount( int count ) {

	if ( segments.count() > count ) {
		qWarning() << "You are attempting to remove some buttons, which is not possible";
		return;
	}

	else if ( segments.count() == count ) {
		qWarning() << "There are already" << count << "segments in this SegmentButton. What are you trying to do?";
		return;
	}

	else {
		for( int i = segments.count(); i < count; i++ )
			addSegment( new PBButton( this ) );

		resetStyleSheets();
	}

	/* AutoResize */
	autoResize();
};

PBButton* PBSegmentButton::segment( int sgmnt ) {

	return segments.at( sgmnt );
};

void PBSegmentButton::setSegmentIcon( int segment, QIcon segIcon ) {

	segments.at( segment )->setIcon( segIcon );

	/* AutoResize */
	autoResize();
};

void PBSegmentButton::setSegmentText( int segment, QString segText ) {

	segments.at( segment )->setText( segText );

	/* AutoResize */
	autoResize();
};

void PBSegmentButton::setSegmentWidth( int width ) {

	Q_FOREACH( PBButton *pBtn, segments )
		pBtn->resize( width, 0 );

	/* AutoResize */
	autoResize();
};

void PBSegmentButton::setSegmentShortcut( int segment, QKeySequence shortcut ) {

	segments.at( segment )->setShortcut( shortcut );
};

void PBSegmentButton::setSegmentDisabled( int segment ) {

	segments.at( segment )->setDisabled( true );
};

void PBSegmentButton::setSegmentEnabled( int segment ) {

	segments.at( segment )->setEnabled( true );
};

void PBSegmentButton::insertSegment( PBButton *button, int logicalPos ) {

	button->setParent( this );
	connect( button, SIGNAL( clicked() ), this, SLOT( handleSegmentClick() ) );

	if ( logicalPos >= segments.count() ) {
		segments << button;
		btnsLyt->addWidget( button );
	}

	else {
		segments.insert( logicalPos, button );
		btnsLyt->insertWidget( logicalPos * 2, button );
	}

	resetStyleSheets();

	/* AutoResize */
	autoResize();
};

void PBSegmentButton::addSegment( PBButton *button ) {

	button->setParent( this );

	connect( button, SIGNAL( clicked() ), this, SLOT( handleSegmentClick() ) );

	btnsLyt->addWidget( button );
	segments << button;
	btnGrp->addButton( button );

	resetStyleSheets();

	/* AutoResize */
	autoResize();
};

QSize PBSegmentButton::size() {

	return QSize( myWidgetWidth, myButtonHeight );
};

int PBSegmentButton::selectionBehavior() {

	return mSelectionBehavior;
};

void PBSegmentButton::setSelectionBehavior( SelectionBehavior behavior ) {

	mSelectionBehavior = behavior;

	switch( behavior ) {
		case PBSegmentButton::SelectOne: {
			btnGrp->setExclusive( true );
			Q_FOREACH( PBButton *btn, segments )
				btn->setCheckable( true );

			break;
		}

		case PBSegmentButton::SelectAll: {
			btnGrp->setExclusive( false );
			Q_FOREACH( PBButton *btn, segments )
				btn->setCheckable( true );

			break;
		}

		case PBSegmentButton::SelectNone: {
			btnGrp->setExclusive( false );
			Q_FOREACH( PBButton *btn, segments )
				btn->setCheckable( false );

			break;
		}
	}
};

QSize PBSegmentButton::sizeHint() {

	return QSize( myWidgetWidth, myButtonHeight );
};

void PBSegmentButton::resetStyleSheets() {

	if ( segments.count() == 1 ) {
		segments.at( 0 )->setSegment( PBButton::SingleButton );
		return;
	}

	for( int i = 0; i < segments.count(); i++ ) {
		if ( i == 0 )
			segments.at( i )->setSegment( PBButton::FirstSegment );

		else if ( i == segments.count() - 1 )
			segments.at( i )->setSegment( PBButton::LastSegment );

		else
			segments.at( i )->setSegment( PBButton::MiddleSegment );
	}
};

void PBSegmentButton::autoResize() {

	myWidgetWidth = 0;
	/* Sum of the width of all the segments */
	Q_FOREACH( PBButton *btn, segments )
		myWidgetWidth += btn->width();

	resize( myWidgetWidth, myButtonHeight );

	setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
};

void PBSegmentButton::handleSegmentClick() {

	PBButton *btn = qobject_cast<PBButton*>( sender() );
	emit clicked( segments.indexOf( btn ) );
};
