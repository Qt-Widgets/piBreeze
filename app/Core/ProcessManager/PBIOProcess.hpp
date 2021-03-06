/*
	*
	* PBIOProcess.hpp - PBIOProcess.cpp header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBAbstractProcess.hpp"

class PBIOProcess : public PBAbstractProcess {
	Q_OBJECT

	public:
		PBIOProcess( QStringList sources, PBProcess::Progress *progress );

		// The list of nodes which could not be copied/moved/archived
		QStringList errors();

		// Cancel the IO Operation
		void cancel();

		// Pause the IO Operation
		void pause();

		// Resume the paused IO
		void resume();

		// Can we undo this operation?
		bool canUndo();

		// Perform the undo
		void undo();

	protected:
		void run();

	private:
		// Things to be done before IO begins like computing sizes
		bool preIO();

		// List the directory contents, and get the size
		void processDirectory( QString );

		// Copy a file
		void copyFile( QString );

		/* This will store the 'sources' list passed to the process  */
		QStringList origSources;
		/* All the not directory nodes in the sources will be listed here */
		QStringList sourceList;

		QStringList errorNodes;

		bool mCanceled;
		bool mPaused;

		bool mUndo;

		PBProcess::Progress *mProgress;
};
