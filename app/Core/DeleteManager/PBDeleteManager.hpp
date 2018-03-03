/*
	*
	* PBDeleteManager.hpp - PBDeleteManager.cpp header
	*
*/

#pragma once

#include "Global.hpp"

class PBDeleter;
class PBTrasher;

class PBDeleteManager : public QObject {
	Q_OBJECT

	public:
		PBDeleteManager( QObject*, bool );

		void deleteFromDisk( QStringList );
		void sendToTrash( QStringList );

	private:
		PBDeleter *deleter;
		PBTrasher *trasher;
		QThread *thread;

	private slots:
		void failureHandler( QStringList, QStringList );

	Q_SIGNALS:
		void deleteOperationComplete( QStringList, QStringList );
		void trashOperationComplete( QStringList, QStringList );
};

class PBDeleter : public QObject {
	Q_OBJECT

	public:
		explicit PBDeleter( QObject *parent = 0 );

	private:
		void removeDir( QString );
		void removeFile( QString );

		QStringList failedFiles;
		QStringList failedDirs;

	public slots:
		void deleteFilesAndFolders( QStringList );

	Q_SIGNALS:
		void completed( QStringList, QStringList );
};

class PBTrasher : public QObject {
	Q_OBJECT

	public:
		explicit PBTrasher( QObject *parent = 0 );

	private:
		void removeDir( QString );
		void removeFile( QString );

		QStringList failedFiles;
		QStringList failedDirs;

	public slots:
		void trashFilesAndFolders( QStringList );

	Q_SIGNALS:
		void completed( QStringList, QStringList );
};
