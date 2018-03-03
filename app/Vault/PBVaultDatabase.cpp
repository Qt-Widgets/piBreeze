/*
    *
    * PBVaultDatabase.hpp - PiBreeze VaultIndex Class
    *
*/

#include "PBVaultDatabase.hpp"
#include <QDebug>

bool PBVaultDatabase::isEncryptedLocation( QString path ) {

	/* Check if the path exists */
	QStringList paths = vaultDB.value( "Paths" ).toStringList();

	if ( isFile( path ) and path.endsWith( ".s20" ) )
		path.chop( 4 );

	if ( paths.contains( path ) )
		return true;

	else {
		/* Typically, */
		QString cpath = dirName( path ) + baseName( path ).remove( 0, 1 );
	}

	return false;
};

PBVaultRecord* PBVaultDatabase::recordForPath( QString path, QByteArray vaultPass ) {

	/* Prepare the PBVault Record Object */
	PBVaultRecord *rec = new PBVaultRecord();
	rec->encPath = QString();
	rec->type = QString();
	rec->recordPass = QByteArray();

	/* Check if the path exists */
	if ( not vaultDB.value( "Paths" ).toStringList().contains( path ) )
		return rec;

	/* Get the hash under which the data is stored */
	QString pathHash = QString( QCryptographicHash5::hash( path.toLocal8Bit(), QCryptographicHash5::Md5 ).toHex() );

	/* Get the data: @pathHash/Data */
	QByteArray data = vaultDB.value( pathHash + "/Data" ).toByteArray();

	/* Ready the Salsa20 decryptor */
	PBSalsa20 s20;

	/* Decrypt the data */
	QByteArray info = s20.decryptData( data, vaultPass );
	QList<QByteArray> infoList = info.split( '\n' );

	/* Store the info in the PBVault Record object */
	rec->encPath = QString::fromLocal8Bit( infoList.at( 0 ) );
	rec->type = QString::fromLocal8Bit( infoList.at( 1 ) );
	rec->recordPass = infoList.at( 2 );

	/* Return the Record */
	return rec;
};

bool PBVaultDatabase::addRecordForPath( QString path, PBVaultRecord *rec, QByteArray vaultPass ) {

	/* Check if the path exists */
	QStringList paths = vaultDB.value( "Paths" ).toStringList();
	if ( paths.contains( path ) )
		return ( recordForPath( path, vaultPass ) == rec );

	paths << path;

	/* Create data from @rec */
	QByteArray data;
	data += rec->encPath + "\n";
	data += rec->type + "\n";
	data += rec->recordPass + "\n";

	/* Encrypt the data */
	PBSalsa20 s20;

	/* Overwrite the data by the encrypted bytes */
	data = s20.encryptData( data, vaultPass );

	/* Get the hash under which the data will be stored */
	QString pathHash = QString( QCryptographicHash5::hash( path.toLocal8Bit(), QCryptographicHash5::Md5 ).toHex() );

	/* Store the updated @paths and the encrypted record */
	vaultDB.setValue( "Paths", paths );
	vaultDB.setValue( pathHash + "/Data", data );

	return true;
};

bool PBVaultDatabase::removeRecordForPath( QString path ) {

	/* Check if the path exists */
	QStringList paths = vaultDB.value( "Paths" ).toStringList();
	if ( not paths.contains( path ) )
		return true;

	/* Remove @path from the Paths list */
	paths.removeAll( path );

	/* Get the hash under which the data is stored */
	QString pathHash = QString( QCryptographicHash5::hash( path.toLocal8Bit(), QCryptographicHash5::Md5 ).toHex() );

	/* Store the update @paths */
	vaultDB.setValue( "Paths", paths );

	/* Remove the data under @pathHash */
	vaultDB.remove( pathHash );

	return true;
};

bool PBVaultDatabase::checkVaultPassword( QByteArray vPass ) {

	/* Password hash */
	QByteArray vpHash = QCryptographicHash5::hash( vPass, QCryptographicHash5::Sha3_512 );

	return ( vaultDB.value( "Password" ).toByteArray() == vpHash );
};

bool PBVaultDatabase::changeVaultPassword( QByteArray oldPass, QByteArray newPass ) {

	PBSalsa20 old20;
	PBSalsa20 new20;

	Q_FOREACH( QString path, vaultDB.value( "Paths" ).toStringList() ) {
		QString pathHash = QString( QCryptographicHash5::hash( path.toLocal8Bit(), QCryptographicHash5::Md5 ).toHex() );
		QByteArray data = old20.decryptData( vaultDB.value( pathHash + "/data" ).toByteArray(), oldPass );
		data = new20.decryptData( data, newPass );

		vaultDB.setValue( pathHash + "/Data", data );
	}

	return true;
};
