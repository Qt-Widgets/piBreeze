/*
	*
	* PBProcessManagerGUI.hpp - PBProcessManagerGUI.cpp header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBIconManager.hpp"
#include "PBProcessManager.hpp"
#include "PBProgressBar.hpp"
#include "PBErrorsDialog.hpp"
#include "PBMessageDialog.hpp"

class PBProcessWidget : public QWidget {
	Q_OBJECT

	public:
		PBProcessWidget( PBProcess::Progress *progress, PBAbstractProcess *proc, QWidget *parent );

	private:
		/* Create a GUI */
		void createGUI();

		/* Icon for the process */
		QLabel *iconLbl;

		/* Copying/Moving/Permissions/Deleting etc */
		QLabel *titleLbl;

		/* Source and target */
		QLabel *srcLbl, *tgtLbl;

		/* Speed and ETA */
		QLabel *speedLbl, *etcLbl;

		/* Current file */
		QLabel *cFileLbl;

		/* ProgressBar */
		PBProgressBar *totalPB, *cfilePB;

		/* Buttons */
		QToolButton *pauseBtn, *detailsBtn;

		/* Refresh timer */
		QBasicTimer updateTimer;

		/* Time spend on puase */
		QTime pauseTime;
		quint64 pausedSecs;

		/* Toggle details flag */
		bool detailsShown;

		PBProcess::Progress *mProgress;
		PBAbstractProcess *mProcess;

	private slots:
		void toggleDetails();
		void togglePauseResume();

	protected:
		void timerEvent( QTimerEvent* );
		void paintEvent( QPaintEvent* );
};
