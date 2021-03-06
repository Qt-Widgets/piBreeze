/*
	*
	* PBFlashLabel.hpp - PBFlashLabel.cpp header file
	*
*/

#pragma once

#include "Global.hpp"
#include "PBDeleteProcess.hpp"
#include "PBProcessManager.hpp"
#include "PBMessageDialog.hpp"
#include "PBConfirmDeleteDialog.hpp"

class PBFlashLabel : public QWidget {
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
		PBFlashLabel( QWidget* );

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
		QPixmap mPixmap;
		QString mText;

		QTimer timer;

		qint64 alpha;
		qint64 mAlphaDelta;
		QColor color;

		int currentStep;
		int flashSteps;
		int mFlashDuration;;

		bool flash;
		int flashesCompleted;
		int maxFlashes;

		/*  */
		bool colorFlash;

	protected :
		void paintEvent( QPaintEvent* );
		void mousePressEvent( QMouseEvent* );

		void enterEvent( QEvent* );
		void leaveEvent( QEvent* );

	public slots:
		void flashLabel();
		void flashLabel( QColor );

	Q_SIGNALS:
		void clicked();
		void entered();
		void exited();
};

class PBTrashLabel : public QWidget {
	Q_OBJECT

	public:
		PBTrashLabel( QWidget* );

	private :
		QPixmap mPixmap;

		QTimer timer;

		QColor color;

		int currentStep;
		int flashSteps;
		int radius;

		bool flash;

	protected :
		void paintEvent( QPaintEvent* );
		void mousePressEvent( QMouseEvent* );

		void dragEnterEvent( QDragEnterEvent* );
		void dropEvent( QDropEvent* );

	private slots:
		void handleDeleteFailure( QStringList, QStringList );
		void flashLabel();

	Q_SIGNALS:
		void showTrash();
		void clicked();
};
