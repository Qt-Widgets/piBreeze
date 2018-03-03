/*
	*
	* PBBookmarksView.hpp - PBBookmarksView.cpp header file
	*
*/

#pragma once

#include "Global.hpp"
#include "PBBookmarkInfo.hpp"
#include "PBIconManager.hpp"

class PBBookmarkMenu;
class PBBookmarksIcon : public QWidget {
	Q_OBJECT

	/* Color properties */
	Q_PROPERTY( QColor flashColor READ flashColor WRITE setFlashColor )
	Q_PROPERTY( qint64 alphaDelta READ alphaDelta WRITE setAlphaDelta )

	/* Flash properties */
	Q_PROPERTY( int flashDuration READ flashDuration WRITE setFlashDuration )
	Q_PROPERTY( int flashesNumber READ numberOfFlashes WRITE setNumberOfFlashes )

	/* Flash Frames */
	Q_PROPERTY( int flashFrames READ flashFrames WRITE setFlashFrames )

	public:
		PBBookmarksIcon( QWidget* );

		/* Override the QLabel pixmap property handlers */
		QPixmap pixmap();
		void setPixmap( QPixmap );

		/* Color property handlers */
		QColor flashColor();
		void setFlashColor( QColor );

		/* Alpha property handlers */
		qint64 alphaDelta();
		void setAlphaDelta( qint64 );

		/* Duration property handlers */
		int flashDuration();
		void setFlashDuration( int );

		/* FlashesNumber property handlers */
		int numberOfFlashes();
		void setNumberOfFlashes( int );

		/* FlashesNumber property handlers */
		int flashFrames();
		void setFlashFrames( int );

	private :
		void createGUI();

		QBasicTimer delayTimer;
		QBasicTimer closeTimer;
		QTimer timer;

		QPixmap mPixmap;

		qint64 alpha;
		qint64 mAlphaDelta;
		QColor color;

		int currentStep;
		int flashSteps;
		int mFlashDuration;;

		bool flash;
		int flashesCompleted;
		int maxFlashes;

		/* should the flash be colored */
		bool colorFlash;

		PBBookmarkMenu *bmkView;

	private slots:
		void showBookmarks();
		void clickDrive( QAction* );

	protected :
		void paintEvent( QPaintEvent* );
		void mousePressEvent( QMouseEvent* );
		void mouseMoveEvent( QMouseEvent* );

		void enterEvent( QEvent* );		// => Expand
		void leaveEvent( QEvent* );		// => Contract

		void timerEvent( QTimerEvent * );

	public slots:
		void flashLabel();
		void flashLabel( QColor );

	Q_SIGNALS:
		void driveClicked( QString );
};

class PBBookmarkMenu : public QMenu {
	Q_OBJECT

	public:
		PBBookmarkMenu( QWidget* parent );

	private:
		bool entered;

	public Q_SLOTS:
		void close();

	protected:
		void enterEvent( QEvent * );
};
