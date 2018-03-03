/*
	*
	* PBProcessManagerUI.hpp - PBProcessManagerUI.cpp header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBDialog.hpp"
#include "PBAbstractProcess.hpp"
#include "PBProcessWidget.hpp"
#include "PBIconManager.hpp"
#include "PBMessageDialog.hpp"
#include "PBProcessManager.hpp"

class PBProcessManagerUI : public PBDialog {
	Q_OBJECT

	public:
		/* Global static instance */
		static PBProcessManagerUI* instance();

	private:
		PBProcessManagerUI();

		void createGUI();
		void setWindowProperties();

		QVBoxLayout *baseLyt;

		PBProcessManager *pMgr;
		static PBProcessManagerUI* procMgrUI;

		static int procCount;

	public Q_SLOTS:
		void show();

	private Q_SLOTS:
		void addProcess( PBProcess::Progress*, PBAbstractProcess* );
};

class PBProcessManagerMini : public QToolButton {
	Q_OBJECT

	public:
		// Init all variables
		PBProcessManagerMini( QWidget *parent );

		// Show the Process Manager
		void showProcessManager();

	private:
		PBProcessManagerUI *procMgrUI;
		PBProcessManager *pMgr;

		QBasicTimer timer;

		qreal fraction;
		quint64 jobs;

	private Q_SLOTS:
		void updateTimer();

		// Show/Hide Process Manager;
		void toggleProcessManager();

	protected:
		void timerEvent( QTimerEvent * );
		void paintEvent( QPaintEvent * );
};
