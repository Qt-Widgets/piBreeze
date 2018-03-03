/*
	*
	* PBCrumbsBar.cpp - BreadCrumbs Navigation bar for PiBreeze
	*
*/

#include "PBCrumbsBar.hpp"

typedef QList<PBCrumb*> PBCrumbsList;

inline QWidget *getCrumbsHolder( QWidget *parent, QString path, QString cPath, const int width, bool *xCrumbsRequired ) {

	if ( path.startsWith( "PB://" ) ) {

		QHBoxLayout *crumbsLyt = new QHBoxLayout();
		crumbsLyt->setContentsMargins( QMargins() );
		crumbsLyt->setSpacing( 0 );

		/* We have only one crumb */
		PBCrumb *specialCrumb = new PBCrumb( path, true );
		QObject::connect( specialCrumb, SIGNAL( loadPath( QString ) ), parent, SIGNAL( openLocation( QString ) ) );
		crumbsLyt->addWidget( specialCrumb );

		crumbsLyt->addStretch();

		QWidget *crumbsHolder = new QWidget( parent );
		crumbsHolder->setLayout( crumbsLyt );

		return crumbsHolder;
	}

	QList<PBCrumbsList> linesList;
	linesList << PBCrumbsList();

	path = QFileInfo( path ).canonicalFilePath();
	cPath = QFileInfo( cPath ).canonicalFilePath();

	PBCrumbsList fullList;

	PBCrumb *crumb;
	QString completed = "/";

	/* We now add the root - '/' first */
	crumb = new PBCrumb( "/", ( QDir( cPath ) == QDir::rootPath() ) );
	QObject::connect( crumb, SIGNAL( loadPath( QString ) ), parent, SIGNAL( openLocation( QString ) ) );
	fullList << crumb;

	Q_FOREACH( QString crumbStr, path.split( "/", QString::SkipEmptyParts ) ) {
		completed += ( crumbStr + "/" );

		crumb = new PBCrumb( completed, ( QDir( completed ) == QDir( cPath ) ) );
		QObject::connect( crumb, SIGNAL( loadPath( QString ) ), parent, SIGNAL( openLocation( QString ) ) );

		fullList << crumb;
	}

	int line = 0, lineToBeShown = 0;
	int x = 0;
	Q_FOREACH( crumb, fullList ) {
		int nextX = x + crumb->sizeHint().width();

		/* Next line */
		if ( nextX > width ) {
			x = 0;
			nextX = x + crumb->sizeHint().width();
			line += 1;

			/* Every time a line is added to the layout, add an empty list for that line */
			linesList << PBCrumbsList();
		}

		linesList[ line ].append( crumb );
		if ( QDir( crumb->path() ) == QDir( cPath ) )
			lineToBeShown = line;

		x = nextX;
	}

	if ( linesList.count() > 1 )
		*xCrumbsRequired = true;

	else
		*xCrumbsRequired = false;

	QHBoxLayout *crumbsLyt = new QHBoxLayout();
	crumbsLyt->setContentsMargins( QMargins() );
	crumbsLyt->setSpacing( 0 );

	/* Add the last line to the layout */
	Q_FOREACH( PBCrumb *cr, linesList.at( lineToBeShown ) )
		crumbsLyt->addWidget( cr );

	crumbsLyt->addStretch();

	QWidget *crumbsHolder = new QWidget( parent );
	crumbsHolder->setLayout( crumbsLyt );

	return crumbsHolder;
}

PBCrumbsBar::PBCrumbsBar( QWidget *parent ) : QWidget( parent ) {

	/* Setup the ExtraCrumb */
	xCrumb = new PBExtraCrumb( this );
	xCrumb->setRequired( false );
	xCrumb->setFixedSize( QSize( 24, 24 ) );
	connect( xCrumb, SIGNAL( clicked() ), this, SLOT( showCrumbsBarX() ) );

	/* Base Layout */
	QHBoxLayout *baseLyt = new QHBoxLayout();
	baseLyt->setContentsMargins( QMargins() );
	baseLyt->setSpacing( 0 );

	baseLyt->addWidget( xCrumb );

	setLayout( baseLyt );

	/* Make the widget expand horizontally */
	setSizePolicy( QSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Fixed ) );

	/* Set up layout timer */
	resizeTimer = new QTimer();
	resizeTimer->setInterval( 250 );
	resizeTimer->setSingleShot( true );
	connect( resizeTimer, SIGNAL( timeout() ), this, SLOT( doLayout() ) );

	/* NoFocus */
	setFocusPolicy( Qt::NoFocus );
};

void PBCrumbsBar::setCurrentDirectory( QString path ) {

	/* No setting empty path */
	if ( path.isEmpty() )
		return;

	/* If there is no change, lets go on ahead */
	if ( not mCurrentPath.isEmpty() and ( QDir( path ) == QDir( mCurrentPath ) ) )
		return;

	/* Let's add a trailing '/' */
	if ( not path.endsWith( "/" ) )
		path += "/";

	if ( path == "/" ) {

		mCurrentPath = "/";
		if ( not mPath.startsWith( "/" ) )
			mPath = "/";
	}

	else if ( mPath.contains( path ) ) {

		mCurrentPath = path;
	}

	else {
		mPath = path;
		mCurrentPath = path;
	}

	/* This prevents the program from crashing when changing using the crumbsBar to navigate */
	QTimer::singleShot( 100, this, SLOT(  doLayout() ) );
};

void PBCrumbsBar::doLayout() {

	bool xCrumbsRequired = false;
	QHBoxLayout * baseLyt = qobject_cast<QHBoxLayout*>( layout() );

	if ( baseLyt->count() == 2 ) {
		baseLyt->removeWidget( crumbsHolder );
		delete crumbsHolder;
	}

	/* rect().width() gives the entire PBCrumbsBar width. We have to deduct the xCrumb width: 24 */
	crumbsHolder = getCrumbsHolder( this, mPath, mCurrentPath, rect().width() - 24, &xCrumbsRequired );
	baseLyt->addWidget( crumbsHolder );

	xCrumb->setRequired( xCrumbsRequired );
};

void PBCrumbsBar::showCrumbsBarX() {

	PBCrumbsBarX *crumbsBarX = new PBCrumbsBarX( mPath, mCurrentPath, this );
	connect( crumbsBarX, SIGNAL( loadPath( QString ) ), this, SIGNAL( openLocation( QString ) ) );
	int height = crumbsBarX->heightForWidth( width() );
	crumbsBarX->resize( width(), height );

	crumbsBarX->move( mapToGlobal( rect().topLeft() ) );
	crumbsBarX->show();
};

void PBCrumbsBar::resizeEvent( QResizeEvent *rEvent ) {

	QWidget::resizeEvent( rEvent );
	rEvent->accept();

	if ( not resizeTimer->isActive() ) {
		resizeTimer->start();
		doLayout();
	}
};

PBCrumbsBarX::PBCrumbsBarX( QString path, QString cPath, QWidget *parent ) : QWidget( parent ) {

	setWindowFlags( Qt::Popup );

	lyt = new FlowLayout();

	PBCrumb *crumb;
	QString completed = "/";

	/* We now add the root - '/' first */
	crumb = new PBCrumb( "/", ( QDir( cPath ) == QDir::rootPath() ) );
	connect( crumb, SIGNAL( loadPath( QString ) ), this, SIGNAL( loadPath( QString ) ) );
	connect( crumb, SIGNAL( loadPath( QString ) ), this, SLOT( close() ) );
	lyt->addWidget( crumb );

	Q_FOREACH( QString crumbStr, path.split( "/", QString::SkipEmptyParts ) ) {
		completed += ( crumbStr + "/" );

		crumb = new PBCrumb( completed, ( QDir( completed ) == QDir( cPath ) ) );
		connect( crumb, SIGNAL( loadPath( QString ) ), this, SIGNAL( loadPath( QString ) ) );
		connect( crumb, SIGNAL( loadPath( QString ) ), this, SLOT( close() ) );

		lyt->addWidget( crumb );
	}

	setLayout( lyt );
};

int PBCrumbsBarX::heightForWidth( int width ) {

	return lyt->heightForWidth( width );
};

void PBCrumbsBarX::paintEvent( QPaintEvent *pEvent ) {

	QWidget::paintEvent( pEvent );

	QPainter *painter = new QPainter( this );
	painter->setPen( palette().color( QPalette::Window ).darker( 125 ) );
	painter->drawRect( rect().adjusted( 0, 0, -1, -1 ) );
	painter->end();

	pEvent->accept();
};
