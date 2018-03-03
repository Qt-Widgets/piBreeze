/*
	*
	* PBProcessManager.hpp - PBProcessManager.cpp header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBAbstractProcess.hpp"
#include "PBIOProcess.hpp"

typedef QList<quint64> ProcessIDList;
typedef QList<PBAbstractProcess*> ProcessList;
typedef QList<PBProcess::Progress *> ProgressList;

class PBProcessManager : public QObject {
	Q_OBJECT

	public:
		/* One single global instance */
		static PBProcessManager* instance();

		/* Total number of processes */
		quint64 processCount();

		/* All the processes */
		ProcessList processes();

		/* All the processes */
		ProgressList progresses();

		/* Number of active processes */
		quint64 activeProcessCount();

		/* All the processes */
		ProcessList activeProcesses();

		/* All the processes */
		ProgressList activeProgresses();

		/* Add a process */
		quint64 addProcess( PBProcess::Progress *progress, PBAbstractProcess * );

		/* Get the process for a process ID */
		PBAbstractProcess* process( quint64 );

		/* Get the progress details for a process ID */
		PBProcess::Progress* progress( quint64 );

	private:
		PBProcessManager() : QObject() {

			init = true;
		};

		/* Our internal global static instance */
		static PBProcessManager *pMgr;

		/* Init flag */
		bool init;

		ProcessIDList completedProcIDs;
		ProgressList progressList;
		ProcessList processList;

	private Q_SLOTS:
		void handleProcessComplete();

	Q_SIGNALS:
		void processAdded( PBProcess::Progress*, PBAbstractProcess* );
		void activeProcessCountChanged( quint64 );
};
