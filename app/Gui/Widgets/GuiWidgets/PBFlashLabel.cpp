/*
	*
	* PBFlashLabel.cpp - A highly customized QLabel for PiBreeze
	*
*/

#include "PBFlashLabel.hpp"
#include "PBGuiFunctions.hpp"

PBFlashLabel::PBFlashLabel( QWidget *parent ) : QWidget( parent ) {


	mPixmap = QPixmap();
	mText = QString();

	alpha = 0;
	mAlphaDelta = 30;
	color = QColor( 255, 255, 255, 0 );

	currentStep = 0;
	flashSteps = 10;
	mFlashDuration = 200;

	flash = false;
	flashesCompleted = 0;
	maxFlashes = 2;

	timer.setInterval( mFlashDuration / flashSteps );
	timer.setSingleShot( false );

	connect( &timer, SIGNAL( timeout() ), this, SLOT( repaint() ) );
};

/* Override the QLabel pixmap property handlers */
QPixmap PBFlashLabel::pixmap() {

	return mPixmap;
};

void PBFlashLabel::setPixmap( QPixmap newPixmap ) {

	mPixmap = newPixmap;
	mText = QString();

	repaint();
};

/* Color property handlers */
QColor PBFlashLabel::flashColor() {

	return color;
};

void PBFlashLabel::setFlashColor( QColor newColor ) {

	color = newColor;
	color.setAlpha( alpha );

	repaint();
};

/* Alpha property handlers */
qint64 PBFlashLabel::alphaDelta() {

	return mAlphaDelta;
};

void PBFlashLabel::setAlphaDelta( qint64 newAlphaDelta ) {

	if ( ( newAlphaDelta * 10 ) > 255 )
		mAlphaDelta = 25;

	else
		mAlphaDelta = newAlphaDelta;
};

/* Flash duration property handlers */
int PBFlashLabel::flashDuration() {

	return mFlashDuration;
};

void PBFlashLabel::setFlashDuration( int newFlashDuration ) {

	mFlashDuration = ( newFlashDuration >= 10 ? newFlashDuration : 10 );
	timer.setInterval( mFlashDuration / flashSteps );
};

/* Number of Flashes property handlers */
int PBFlashLabel::numberOfFlashes() {

	return maxFlashes;
};

void PBFlashLabel::setNumberOfFlashes( int newNumOfFlashes ) {

	maxFlashes = newNumOfFlashes;
};

/* flashFrames property handlers - Number of frames per flash */
int PBFlashLabel::flashFrames() {

	return flashSteps;
};

void PBFlashLabel::setFlashFrames( int newFlashSteps ) {

	/* Persistence of vision is 62.5 (~63) ms */

	flashSteps = ( newFlashSteps > mFlashDuration ? mFlashDuration / 63 : newFlashSteps );
	timer.setInterval( mFlashDuration / flashSteps );
};

/* Overriding of paint event for showing flashes */
void PBFlashLabel::paintEvent( QPaintEvent *pEvent ) {

	if ( flash ) {
		if ( flashesCompleted == maxFlashes ) {
			timer.stop();
			flash = false;
			currentStep = 0;
			alpha = 0;
			flashesCompleted = 0;
		}

		else if ( currentStep == flashSteps ) {
			flashesCompleted += 1;
			currentStep = 0;
		}

		else if ( currentStep < flashSteps / 2 ) {
			currentStep += 1;
			alpha += mAlphaDelta;
			color.setAlpha( alpha );
		}

		else if ( currentStep >= ( flashSteps / 2 ) ) {
			currentStep += 1;
			alpha -= mAlphaDelta;
			color.setAlpha( alpha );
		}
	}

	QPainter *painter = new QPainter( this );
	painter->save();

	painter->setPen( Qt::transparent );
	painter->setBrush( color );
	painter->drawRect( 0, 0, width(), height() );

	if ( not mPixmap.isNull() ) {
		painter->setBrush( Qt::transparent );
		painter->drawPixmap( 8, 8, 32, 32, mPixmap );
	}

	else if ( not ( mText.isNull() or  mText.isEmpty() ) ) {
		painter->setBrush( Qt::transparent );
		painter->drawText( 0, 0, width(), height(), Qt::AlignCenter | Qt::TextWordWrap, mText );
	}

	painter->restore();
	painter->end();

	pEvent->accept();
};

/* Overriding QLabel::mousePressEvent to emit clicked signal */
void PBFlashLabel::mousePressEvent( QMouseEvent *mEvent ) {

	emit clicked();
	mEvent->accept();
};

/* Overriding QLabel::enterEvent to emit entered signal */
void PBFlashLabel::enterEvent( QEvent *mEvent ) {

	emit entered();
	mEvent->accept();
};

/* Overriding QLabel::leaveEvent to emit exited signal */
void PBFlashLabel::leaveEvent( QEvent *lEvent ) {

	emit exited();
	lEvent->accept();
};

/* Slot to access the flashing */
void PBFlashLabel::flashLabel() {

	if ( colorFlash ) {
		colorFlash = false;
		color = QColor( 255, 255, 255, 0 );
	}

	if ( timer.isActive() )
		timer.stop();

	timer.start();
	flash = true;
};

/* Slot to access the flashing with a given color */
void PBFlashLabel::flashLabel( QColor newColor ) {

	setFlashColor( newColor );

	if ( timer.isActive() )
		timer.stop();

	timer.start();
	flash = true;
};

PBTrashLabel::PBTrashLabel( QWidget *parent ) : QWidget( parent ) {

	mPixmap = QPixmap();
	color = QColor( 255, 0, 0, 150 );

	currentStep = 0;
	flashSteps = 50;
	radius = 0;

	flash = false;

	setAcceptDrops( true );
	mPixmap = QIcon::fromTheme( "user-trash" ).pixmap( 32 );

	timer.setInterval( 5 );
	timer.setSingleShot( false );

	connect( &timer, SIGNAL( timeout() ), this, SLOT( repaint() ) );
};

/* Overriding of paint event for showing flashes */
void PBTrashLabel::paintEvent( QPaintEvent *pEvent ) {

	QPainter *painter = new QPainter( this );
	painter->save();

	painter->fillRect( QRect( 0, 0, width(), height() ), Qt::transparent );

	QRadialGradient bgGradient( 24, 24, radius );
	if ( flash ) {
		if ( currentStep == flashSteps ) {
			timer.stop();
			flash = false;
			currentStep = 0;
			radius = 0;

			bgGradient.setColorAt( 0, QColor( Qt::transparent ) );
			bgGradient.setColorAt( 1, QColor( Qt::transparent ) );
		}

		else if ( currentStep < flashSteps ) {
			currentStep += 1;
			radius += 3;

			bgGradient.setRadius( radius );
			bgGradient.setColorAt( 0, QColor( Qt::transparent ) );
			bgGradient.setColorAt( 0.85, color );
			bgGradient.setColorAt( 1, QColor( Qt::transparent ) );
		}

		painter->setPen( Qt::transparent );
		painter->setBrush( bgGradient );
		painter->drawEllipse( QPoint( 24, 24 ), radius, radius );
	}

	painter->setBrush( Qt::transparent );
	painter->drawPixmap( 8, 8, 32, 32, mPixmap );

	painter->restore();
	painter->end();

	pEvent->accept();
};

/* Overriding QLabel::mousePressEvent to emit clicked signal */
void PBTrashLabel::mousePressEvent( QMouseEvent *mEvent ) {

	emit clicked();
	mEvent->accept();
};

void PBTrashLabel::dragEnterEvent( QDragEnterEvent *deEvent ) {

	deEvent->acceptProposedAction();
};

/* Drop event handler for the delete label */
void PBTrashLabel::dropEvent( QDropEvent *dpEvent ) {

	const QMimeData *mData = dpEvent->mimeData();
	if ( mData->hasUrls() ) {
		dpEvent->accept();

		/* Check if we have protection set */
		QSettings nbSettings( "PiBreeze", "PiBreeze" );
		QStringList safeNodes = nbSettings.value( "ProtectedNodes" ).toStringList();

		QStringList toBeDeleted;
		Q_FOREACH( QUrl url, mData->urls() ) {
			QString path = url.toLocalFile();
			if ( not safeNodes.contains( path ) )
				toBeDeleted << path.replace( dirName( path ), "" );
		}

		if ( dpEvent->keyboardModifiers() == Qt::NoModifier ) {

			color = QColor( Qt::darkYellow );
			color.setAlpha( 150 );

			PBProcess::Progress *progress = new PBProcess::Progress;
			progress->sourceDir = dirName( mData->urls().at( 0 ).toLocalFile() );
			progress->targetDir = QString();
			progress->type = PBProcess::Trash;

			PBDeleteProcess *proc = new PBDeleteProcess( toBeDeleted, progress );
			PBProcessManager::instance()->addProcess( progress, proc );

			progress->startTime = QTime::currentTime();
			proc->start();

			flashLabel();
		}

		else if ( dpEvent->keyboardModifiers() == Qt::ShiftModifier ) {

			color = QColor( Qt::red );
			color.setAlpha( 150 );

			PBConfirmDeleteDialog *deleteMsg = new PBConfirmDeleteDialog( dirName( toBeDeleted.at( 0 ) ), toBeDeleted, true );
			if ( not deleteMsg->exec() )
				return;

			PBProcess::Progress *progress = new PBProcess::Progress;
			progress->sourceDir = dirName( mData->urls().at( 0 ).toLocalFile() );
			progress->targetDir = QString();
			progress->type = PBProcess::Delete;

			PBDeleteProcess *proc = new PBDeleteProcess( toBeDeleted, progress );
			PBProcessManager::instance()->addProcess( progress, proc );

			progress->startTime = QTime::currentTime();
			proc->start();

			flashLabel();
		}

		else {

			dpEvent->ignore();
			return;
		}
	}

	dpEvent->ignore();
};

/* Slot to handle the deletion failures */
void PBTrashLabel::handleDeleteFailure( QStringList files, QStringList dirs ) {

	if ( ( not files.count() ) and ( not dirs.count() ) )
		return;

	QTableWidget *table = new QTableWidget( 0, 2 );
	table->setFocusPolicy( Qt::NoFocus );

	table->verticalHeader()->hide();
	table->setHorizontalHeaderLabels( QStringList() << "File Name" << "Size" );

	table->setShowGrid( false );
	table->setSelectionBehavior( QAbstractItemView::SelectRows );

	QHeaderView *headerView = new QHeaderView( Qt::Horizontal, table );
	table->setHorizontalHeader( headerView );
	#if QT_VERSION >= 0x050000
		headerView->setSectionResizeMode( 0, QHeaderView::Stretch );
		headerView->setSectionResizeMode( 1, QHeaderView::Fixed );
		table->verticalHeader()->setSectionResizeMode( QHeaderView::Fixed );
	#else
		headerView->setResizeMode( 0, QHeaderView::Stretch );
		headerView->setResizeMode( 1, QHeaderView::Fixed );
		table->verticalHeader()->setResizeMode( QHeaderView::Fixed );
	#endif

	table->setColumnWidth( 1, 100 );

	foreach( QString path, dirs ) {
		QTableWidgetItem *itm1 = new QTableWidgetItem( icon( PBIconManager::instance()->iconsForFile( "", path ) ), path );
		QTableWidgetItem *itm2 = new QTableWidgetItem( formatSize( getSize( path ) ) );

		itm1->setFlags( itm1->flags() & ~Qt::ItemIsEditable );
		itm2->setFlags( itm2->flags() & ~Qt::ItemIsEditable );

		table->insertRow( table->rowCount() );

		table->setItem( table->rowCount() - 1, 0, itm1 );
		table->setItem( table->rowCount() - 1, 1, itm2 );
	}
	foreach( QString path, files ) {
		QTableWidgetItem *itm1 = new QTableWidgetItem( icon( PBIconManager::instance()->iconsForFile( "", path ) ), path );
		QTableWidgetItem *itm2 = new QTableWidgetItem( formatSize( getSize( path ) ) );

		itm1->setFlags( itm1->flags() & ~Qt::ItemIsEditable );
		itm2->setFlags( itm2->flags() & ~Qt::ItemIsEditable );

		table->insertRow( table->rowCount() );

		table->setItem( table->rowCount() - 1, 0, itm1 );
		table->setItem( table->rowCount() - 1, 1, itm2 );
	}

	PBMessageDialog::error( this,
		"PiBreeze - Error while deleting",
		"Some errors were encountered while deleting the files and folders you requested. "			\
		"As a result, some of the files and folders may not have been deleted. For the "			\
		"list for files and folder not deleted click <u>M</u>ore"
	);
};

/* Slot to access the flashing */
void PBTrashLabel::flashLabel() {

	if ( timer.isActive() )
		timer.stop();

	timer.start();
	flash = true;
};
