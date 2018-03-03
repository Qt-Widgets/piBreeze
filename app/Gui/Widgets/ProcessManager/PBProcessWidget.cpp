/*
	*
	* PBProcessManagerGUI.cpp - The PBProcessManagerGUI class for PiBreeze
	*
*/

#include "PBProcessWidget.hpp"

/*
    *
    * ┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
    * │ ┌──────┐  Copying source directory name                                                                 [<] [>] [@] [-] [x] │
    * │ │ ☒ ☐ │  Source: /home/cosmos/.....                                                                                        │
    * │ │ ☐ ☒ │  Target: /home/cosmos/.....                                                                                        │
    * │ └─────-┘  Speed:  31 MiB/s                ETA:  1 hr 24 min 36 sec                                                          │
    * │ [ ═══════════════════════════════════════>                                                                                ] │
    * │ Current File: /home/cosmos/......                                                                                           │
    * │ [ ════════════════════════════════════════════════════════════════════════════════>                                       ] │
    * └─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
    *
*/

PBProcessWidget::PBProcessWidget( PBProcess::Progress *progress, PBAbstractProcess *proc, QWidget *parent ) : QWidget( parent ) {

	mProgress = progress;
	mProcess = proc;

	detailsShown = true;

	pausedSecs = 0;

	createGUI();
};

void PBProcessWidget::createGUI() {

	iconLbl = new QLabel( this );
	iconLbl->setPixmap( QIcon( ":/icons/process.png" ).pixmap( 48 ) );
	iconLbl->setFixedSize( QSize( 48, 48 ) );

	switch( mProgress->type ) {
		case PBProcess::Copy: {
			titleLbl = new QLabel( "Copying files", this );
			break;
		}
		case PBProcess::Move: {
			titleLbl = new QLabel( "Moving files", this );
			break;
		}
		case PBProcess::Delete: {
			titleLbl = new QLabel( "Deleting files", this );
			break;
		}
		case PBProcess::Trash: {
			titleLbl = new QLabel( "Sending files to trash", this );
			break;
		}
		case PBProcess::Properties: {
			titleLbl = new QLabel( "Settings file permissions", this );
			break;
		}
	}
	titleLbl->setFont( QFont( font().family(), font().pointSize() + 1, QFont::Bold, false ) );

	srcLbl = new QLabel( this );
	srcLbl->setText( "Source: " + mProgress->sourceDir );

	tgtLbl = new QLabel( this );
	tgtLbl->setText( "Target: " + mProgress->targetDir  );

	speedLbl = new QLabel( this );
	speedLbl->setText( "Speed: 0 B/s" );

	cFileLbl = new QLabel( this );
	cFileLbl->setText( "Current File: " );

	etcLbl = new QLabel( this );
	etcLbl->setText( "ETC:" );
	etcLbl->setToolTip( QString( "Estimated time of completion" ) );

	totalPB = new PBProgressBar( this );
	totalPB->setValue( -1 );

	cfilePB = new PBProgressBar( this );
	cfilePB->setValue( -1 );

	detailsBtn = new QToolButton( this );
	detailsBtn->setIcon( QIcon( ":/icons/arrow-up.png" ) );
	detailsBtn->setIconSize( QSize( 16, 16 ) );
	detailsBtn->setFixedSize( QSize( 20, 20 ) );
	connect( detailsBtn, SIGNAL( clicked() ), this, SLOT( toggleDetails() ) );

	pauseBtn = new QToolButton( this );
	pauseBtn->setIcon( QIcon::fromTheme( "media-playback-pause" ) );
	pauseBtn->setIconSize( QSize( 16, 16 ) );
	pauseBtn->setFixedSize( QSize( 20, 20 ) );
	connect( pauseBtn, SIGNAL( clicked() ), this, SLOT( togglePauseResume() ) );

	QHBoxLayout *btnLyt = new QHBoxLayout();
	btnLyt->addWidget( titleLbl );
	btnLyt->addStretch();
	btnLyt->addWidget( detailsBtn );
	btnLyt->addWidget( pauseBtn );

	QHBoxLayout *speedLyt = new QHBoxLayout();
	speedLyt->addWidget( speedLbl );
	speedLyt->addWidget( etcLbl );

	QVBoxLayout *lblLyt = new QVBoxLayout();
	lblLyt->addLayout( btnLyt );
	lblLyt->addWidget( srcLbl );
	lblLyt->addWidget( tgtLbl );
	lblLyt->addLayout( speedLyt );

	QHBoxLayout *icnLyt = new QHBoxLayout();
	icnLyt->addWidget( iconLbl );
	icnLyt->addLayout( lblLyt );

	QVBoxLayout *baseLyt = new QVBoxLayout();
	baseLyt->addLayout( icnLyt );
	baseLyt->addWidget( totalPB );
	baseLyt->addWidget( cFileLbl );
	baseLyt->addWidget( cfilePB );

	setLayout( baseLyt );

	updateTimer.start( 500, this );
};

void PBProcessWidget::toggleDetails() {

	if ( detailsShown ) {
		detailsBtn->setIcon( QIcon( ":/icons/arrow-down.png" ) );

		srcLbl->show();
		tgtLbl->hide();
		speedLbl->hide();
		etcLbl->hide();
		cFileLbl->hide();
		cfilePB->hide();

		detailsShown = false;

		setFixedHeight( 95 );
	}

	else {
		detailsBtn->setIcon( QIcon( ":/icons/arrow-up.png" ) );

		srcLbl->show();
		tgtLbl->show();
		speedLbl->show();
		etcLbl->show();
		cFileLbl->show();
		cfilePB->show();

		detailsShown = true;
		setFixedHeight( 180 );
	}
};

void PBProcessWidget::togglePauseResume() {

	if ( mProgress->state == PBProcess::Paused ) {
		pauseBtn->setIcon( QIcon::fromTheme( "media-playback-pause" ) );
		pauseBtn->setToolTip( "Pause" );

		mProcess->resume();

		pausedSecs += pauseTime.elapsed() / 1000;
	}

	else {
		pauseBtn->setIcon( QIcon::fromTheme( "media-playback-start" ) );
		pauseBtn->setToolTip( "Resume" );

		mProcess->pause();

		pauseTime.start();
	}
};

void PBProcessWidget::timerEvent( QTimerEvent *tEvent ) {

	if ( tEvent->timerId() == updateTimer.timerId() ) {
		switch( mProgress->state ) {

			case PBProcess::NotStarted: {

				if ( not mProgress->progressText.isEmpty() )
					totalPB->setProgressText( mProgress->progressText );

				tEvent->accept();
				return;
			}

			case PBProcess::Starting: {

				totalPB->setSway( true );
				cfilePB->setSway( true );

				if ( not mProgress->progressText.isEmpty() )
					totalPB->setProgressText( mProgress->progressText );

				tEvent->accept();
				return;
			}

			case PBProcess::Started: {

				/* We need the update labels only if we are performing an IO */
				if ( ( mProgress->type == PBProcess::Copy ) or ( mProgress->type == PBProcess::Move ) ) {
					/* Total fraction */
					qreal tFraction = 1.0 * mProgress->totalBytesCopied / mProgress->totalBytes;

					/* Current File fraction */
					qreal cFraction = 1.0 * mProgress->fileBytesCopied / mProgress->fileBytes;

					/* Speed: totalBytesCopied / effective time spent on copying */
					qreal speed = 1.0 * mProgress->totalBytesCopied / ( mProgress->startTime.secsTo( QTime::currentTime() ) - pausedSecs );

					/* ETC = Remaining bytes / last chunk speed = Remaining bytes / ( last chunk size / 0.5 )  */
					quint64 secs = ( mProgress->totalBytes - mProgress->totalBytesCopied ) / speed;

					QString hrs = ( secs / 3600 ? QString( "%1 hours, " ).arg( secs / 3600 ) : QString() );
					QString mns = ( ( secs % 3600 ) / 60 ? QString( "%1 minutes, " ).arg( ( secs % 3600 ) / 60 ) : QString() );
					QString scs = ( ( secs % 3600 ) % 60 ? QString( "%1 seconds" ).arg( ( secs % 3600 ) % 60 ) : QString() );

					cFileLbl->setText( mProgress->currentFile );

					totalPB->setValue( tFraction );
					cfilePB->setValue( cFraction );

					speedLbl->setText( QString( "Speed: %1/s" ).arg( formatSize( speed ) ) );
					etcLbl->setText( QString( "ETC: %1%2%3" ).arg( hrs ).arg( mns ).arg( scs ) );
				}

				else {

					cFileLbl->setText( mProgress->currentFile );
				}

				return;
			}

			case PBProcess::Paused: {

				etcLbl->setText( "ETC: -- hours, -- minutes, -- seconds" );
				return;
			}

			case PBProcess::Canceled:
			case PBProcess::Completed: {

				cFileLbl->setText( QString() );

				totalPB->setValue( 1 );
				cfilePB->setValue( 1 );

				speedLbl->setText( "Average " + speedLbl->text() );

				/* ETC = Remaining bytes / last chunk speed = Remaining bytes / ( last chunk size / 0.5 )  */
				quint64 secs = mProgress->startTime.secsTo( QTime::currentTime() );

				QString hrs = ( secs / 3600 ? QString( "%1 hours, " ).arg( secs / 3600 ) : QString() );
				QString mns = ( ( secs % 3600 ) / 60 ? QString( "%1 minutes, " ).arg( ( secs % 3600 ) / 60 ) : QString() );
				QString scs = ( ( secs % 3600 ) % 60 ? QString( "%1 seconds" ).arg( ( secs % 3600 ) % 60 ) : QString() );
				etcLbl->setText( QString( "Completed: %1%2%3" ).arg( hrs ).arg( mns ).arg( scs ) );

				updateTimer.stop();

				pauseBtn->setDisabled( true );

				if ( mProcess->errors().count() ) {
					QString title;
					QString message = QString( "Some errors were encountered while " );

					switch( mProgress->type ) {
						case PBProcess::Copy: {
							title = "PiBreeze - Error copying files";
							message += "copying";
							break;
						}

						case PBProcess::Move: {
							title = "PiBreeze - Error copying files";
							message += "moving";
							break;
						}

						case PBProcess::Trash: {
							title = "PiBreeze - Error trashing files";
							message += "trashing";
							break;
						}

						case PBProcess::Delete: {
							title = "PiBreeze - Error deleting files";
							message += "deleteing";
							break;
						}

						case PBProcess::Properties: {
							title = "PiBreeze - Error settings permissions";
							message += "settings the permissions for";
							break;
						}

						default: {
							title = "PiBreeze - Error performing operation";
							message += "performing an operation(!!) on";
							break;
						}
					}

					message += "the nodes you requested. The list of nodes which were not processed correctly are given below.";

					PBErrorsDialog *errDlg = new PBErrorsDialog( title, message, mProcess->errors(), this );
					errDlg->exec();
				}

				return;
			}
		}
	}

	else {

		QWidget::timerEvent( tEvent );
	}
};

void PBProcessWidget::paintEvent( QPaintEvent *pEvent ) {

	QWidget::paintEvent( pEvent );

	QPainter *painter = new QPainter( this );
	painter->setPen( Qt::gray );
	painter->drawRect( rect().adjusted( 0, 0, -1, -1 ) );
	painter->end();

	pEvent->accept();
};
