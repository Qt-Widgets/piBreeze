/*
    *
    * PBVaultDatabase.hpp - PiBreeze VaultIndex Header
    *
*/

#pragma once

#ifdef STANDALONE
	#include "Global2.hpp"
#else
	#include "Global.hpp"
#endif
#include "Salsa20/PBSalsa20.hpp"

typedef struct _PBVaultRecord {
	QString encPath;
	QString type;
	QByteArray recordPass;
} PBVaultRecord;

static QSettings vaultDB( QDir( PBXdg::userDir( PBXdg::XDG_CONFIG_HOME ) ).filePath( "PiBreeze/Vault.db" ), QSettings::NativeFormat );

namespace PBVaultDatabase {

	bool isEncryptedLocation( QString path );

	/* PBVaultRecord object for a given path */
	PBVaultRecord* recordForPath( QString path, QByteArray vaultPass );

	/* Add a record to the database */
	bool addRecordForPath( QString path, PBVaultRecord *, QByteArray vaultPass );

	/* Remove a record to the database */
	bool removeRecordForPath( QString path );

	/* Check if the vault key is correct */
	bool checkVaultPassword( QByteArray vPass );

	/* Change password of the Vault */
	bool changeVaultPassword( QByteArray oldPass, QByteArray newPass );
};
