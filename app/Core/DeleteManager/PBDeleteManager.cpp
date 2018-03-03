/*
	*
	* PBDeleteManager.cpp - PiBreeze File/Folder Deleter Class
	*
*/

#include "PBDeleteManager.hpp"

PBDeleteManager::PBDeleteManager( QObject *parent, bool trash ) : QObject( parent ) {

	thread = new QThread;
	deleter = new PBDeleter;
	trasher = new PBTrasher;

	if ( trash == true ) {
		trasher->moveToThread( thread );
		connect(
			trasher, SIGNAL( completed( QStringList, QStringList ) ),
			this, SIGNAL( deleteOperationComplete( QStringList, QStringList ) )
		);
	}

	else {
		deleter->moveToThread( thread );
		connect(
			deleter, SIGNAL( completed( QStringList, QStringList ) ),
			this, SIGNAL( trashOperationComplete( QStringList, QStringList ) )
		);
	}

	thread->start();
};

void PBDeleteManager::deleteFromDisk( QStringList deleteList ) {

	QMetaObject::invokeMethod( deleter, "deleteFilesAndFolders", Qt::QueuedConnection, Q_ARG( QStringList, deleteList ) );
};

void PBDeleteManager::sendToTrash( QStringList trashList ) {

	QMetaObject::invokeMethod( trasher, "trashFilesAndFolders", Qt::QueuedConnection, Q_ARG( QStringList, trashList ) );
};

void PBDeleteManager::failureHandler( QStringList failedFiles, QStringList failedDirs ) {

	emit deleteOperationComplete( failedFiles, failedDirs );
};

PBDeleter::PBDeleter( QObject *parent ) : QObject( parent ) {

};

void PBDeleter::removeDir( QString dirName ) {

	QDir dir( dirName);

    if ( dir.exists( dirName ) ) {
		QDir::Filters dFilters = QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files;
        Q_FOREACH( QFileInfo info, dir.entryInfoList( dFilters, QDir::DirsFirst ) ) {
            if ( info.isDir() )
                removeDir( info.absoluteFilePath() );

            else
                removeFile( info.absoluteFilePath() );
        }

        if ( not dir.rmdir( dirName ) )
			failedDirs << dirName;
    }
};

void PBDeleter::removeFile( QString fileName ) {

	if ( not QFile::remove( fileName ) )
		failedFiles << fileName;
};

void PBDeleter::deleteFilesAndFolders( QStringList deleteList ) {

	foreach( QString item, deleteList ) {
		if ( QFileInfo( item ).isDir() )
			removeDir( item );

		else
			removeFile( item );
	}

	emit completed( failedFiles, failedDirs );
};

PBTrasher::PBTrasher( QObject *parent ) : QObject( parent ) {

};

void PBTrasher::trashFilesAndFolders( QStringList trashList ) {

	QString trashLoc = PBXdg::trashLocation( trashList.at( 0 ) );

	foreach( QString item, trashList ) {
		/* Get the trashed path: $TRASH/files/filename */
		QString newPath = trashLoc + "/files/" + baseName( item );
		QString delTime = QDateTime::currentDateTime().toString( "yyyyMMddThh:mm:ss" );

		/* If it exists, add a date time to it to make it unique */
		if ( access( newPath.toLocal8Bit().data(), R_OK ) == 0 )
			newPath += delTime;

		qDebug() << item << newPath;

		/* Try trashing it. If it fails, intimate the user */
		if ( rename( item.toLocal8Bit().data(), newPath.toLocal8Bit().data() ) ) {
			qDebug() << "Error" << errno << ": Failed to trash " << item << ":" << strerror( errno );
			failedFiles << item;
		}

		/* If it succeeds, we write the meta data */
		else {
			QFile metadata( trashLoc + "/info/" + baseName( newPath ) + ".trashinfo" );
			metadata.open( QIODevice::WriteOnly );
			metadata.write(
				QString(
					"[Trash Info]\n"
					"Path=%1\n"
					"DeletionDate=%2\n"
				).arg( item ).arg( delTime ).toLocal8Bit()
			);
			metadata.close();

			/* An ugly hack: Shortcut for TrashModel listing */
			QSettings trashInfo( "PiBreeze", "TrashInfo" );
			trashInfo.setValue( baseName( item ), QStringList() << item << delTime << newPath );
		}
	}

	emit completed( failedFiles, failedDirs );
};
