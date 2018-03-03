/*
*
* PBLogger.cpp - Logging for PiBreeze
*
*/

#include <PBLogger.hpp>

void PBMessageOutput5( QtMsgType type, const QMessageLogContext &context, const QString &message ) {

	Q_UNUSED( context );

	switch ( type ) {
		case QtDebugMsg: {
			fprintf( stderr, "\033[01;30mPiBreeze::Debug# %s\n\033[00;00m", message.toLocal8Bit().data() );
			break;
		}

		case QtWarningMsg: {
			if ( QString( message ).contains( "X Error" ) or QString( message ).contains( "libpng warning" ) )
				break;
			fprintf( stderr, "\033[01;33mPiBreeze::Warning# %s\n\033[00;00m", message.toLocal8Bit().data() );
			break;
		}

		case QtCriticalMsg: {
			fprintf( stderr, "\033[01;31mPiBreeze::CriticalError# %s\n\033[00;00m", message.toLocal8Bit().data() );
			break;
		}

		case QtFatalMsg: {
			fprintf( stderr, "\033[01;41mPiBreeze::FatalError# %s\n\033[00;00m", message.toLocal8Bit().data() );
			abort();
		}
	}
};
