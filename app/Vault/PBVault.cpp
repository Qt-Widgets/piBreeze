/*
    *
    * PBVault.hpp - PiBreeze Vault Class
    *
*/

#include "PBVault.hpp"
#include "PBPasswordDialog.hpp"
#include "PBPasswordInput.hpp"
#include "EncFS/PBEncFS.hpp"

QByteArray PBVault::vaultPass = QByteArray();
PBVault::Options PBVault::mKeyStoreOption = PBVault::AskVaultKeyEverytime;

PBVault *PBVault::vault = NULL;

PBVault::PBVault() {

	init = true;
	mKeyStoreOption = PBVault::AskVaultKeyEverytime;
};

PBVault* PBVault::instance() {

	if ( not vault or vault->init )
		vault = new PBVault();

	return vault;
};

void PBVault::setVaultOption( PBVault::Options opt ) {

	mKeyStoreOption = opt;
};

bool PBVault::decryptFile( QString path ) {

	QByteArray pass = vaultPassword();
	if ( not pass.count() )
		return false;

	if ( path.endsWith( ".s20" ) )
		path.chop( 4 );

	PBVaultRecord *record = PBVaultDatabase::recordForPath( path, pass );
	if ( not record->recordPass.size() )
		return false;

	if ( not exists( path + ".s20" ) )
		return false;

	PBVaultDatabase::removeRecordForPath( path );
	if ( not record->recordPass.size() )
		return false;

	PBSalsa20 s20( record->encPath );
	s20.decrypt( record->recordPass );

	return true;
};

bool PBVault::encryptFile( QString path ) {

	QByteArray pass = vaultPassword();
	if ( not pass.count() )
		return false;

	if ( not isFile( path ) )
		return false;

	PBVaultRecord *record = new PBVaultRecord();
	record->encPath = path + ".s20";
	record->type = QString( "file" );
	record->recordPass = generatePassword();

	PBSalsa20 s20( path );
	s20.encrypt( record->recordPass );

	PBVaultDatabase::addRecordForPath( path, record, pass );

	return true;
};

bool PBVault::decryptDirectory( QString path ) {

	QByteArray pass = vaultPassword();
	if ( not pass.count() )
		return false;

	PBVaultRecord *record = PBVaultDatabase::recordForPath( path, pass );
	if ( not record->recordPass.size() )
		return false;

	PBEncFS encfs( record->encPath, path );
	return encfs.mountDir( QString::fromLocal8Bit( record->recordPass.toHex() ) );
};

bool PBVault::encryptDirectory( QString path ) {

	if ( PBVaultDatabase::isEncryptedLocation( path ) ) {
		PBEncFS encfs( QString( " " ), path );
		return encfs.unmountDir();
	}

	else {
		PBVault *vlt = PBVault::instance();
		QByteArray pass = vlt->vaultPassword();
		if ( not pass.count() )
			return false;

		QString mtPt = baseName( path );

		PBVaultRecord *record = new PBVaultRecord();
		record->encPath = dirName( path ) + "." + mtPt;
		record->type = QString( "dir" );
		record->recordPass = vlt->generatePassword();

		PBEncFS encfs( record->encPath, path );
		if ( encfs.createEncFS( record->recordPass.toHex() ) )
			return PBVaultDatabase::addRecordForPath( path, record, pass );

		else
			return false;
	}
};

bool PBVault::isDirectoryDecrypted( QString path ) {

	if ( path.endsWith( "/" ) )
		path.chop( 1 );

	QProcess proc;
	proc.start( "df -h" );
	proc.waitForFinished();

	QString output = QString::fromLocal8Bit( proc.readAllStandardOutput() );

	if ( output.contains( " " + path + "\n" ) )
		return true;

	return false;
};

QByteArray PBVault::vaultPassword() {

	if ( not vaultDB.value( "Password" ).toByteArray().size() ) {
		QMessageBox *msgBox = new QMessageBox( NULL );
		msgBox->setWindowTitle( "PiBreeze - PBVault" );
		msgBox->setIcon( QMessageBox::Information );
		msgBox->setText( "PiBreeze Vault First Use" );
		msgBox->setInformativeText(
			"<p>It seems that this is the first time you are using PiBreeze Vault. Please read the following disclaimer carefully.</p>"
			"<p><i>This implementation of PBVault is given to you as is without any without any warranty under the terms of the "
			"GNU General Public License (version 3 or later). This implementation is <b>NOT CERTIFIED</b> by any cryptography expert. "
			"PBVault is meant for keeping your private/sensitive stuff from prying eyes and not for secure storage of important data.</i></p>"
			"<p>Please accept that you are using PBVault at your own risk and the developer is not responsible for any data loss.</p>"
		);
		msgBox->addButton( "&Accept", QMessageBox::AcceptRole );
		msgBox->addButton( "&Reject", QMessageBox::RejectRole );

		if ( msgBox->exec() == QMessageBox::RejectRole )
			return QByteArray();

		QMessageBox::information( NULL, "Newbreeze - PBVault", "Please set the vault password and remember it. If the vault password is forgotten, "
		"you <b>cannot</b> retrieve the encrypted data from files/folders. You may set a pattern or use a text password." );

		PBPasswordDialog *pDlg = new PBPasswordDialog( NULL );
		if ( pDlg->exec() )
			vaultDB.setValue( "Password", QCryptographicHash5::hash( pDlg->password(), QCryptographicHash5::Sha3_512 ) );
	}

	if ( ( mKeyStoreOption == PBVault::StoreVaultKeyForSession ) and ( vaultPass.count() ) )
		return vaultPass;

	PBPasswordInput *pIn = new PBPasswordInput( NULL );
	pIn->exec();

	QByteArray passwd = pIn->password().toLocal8Bit();
	pIn->clear();

	if ( not PBVaultDatabase::checkVaultPassword( passwd ) ) {
		QMessageBox::information( NULL, "PiBreeze Vault Error", "You have entered the wrong password!" );
		return QByteArray();
	}

	else {
		if ( mKeyStoreOption == PBVault::StoreVaultKeyForSession )
			vaultPass = passwd;

		return passwd;
	}
};

bool PBVault::changeVaultPassword() {

	QByteArray oldPass = vaultPassword();
	QByteArray newPass;

	PBPasswordDialog *pDlg = new PBPasswordDialog( NULL );
	if ( pDlg->exec() ) {
		newPass = pDlg->password();
		vaultDB.setValue( "Password", QCryptographicHash5::hash( pDlg->password(), QCryptographicHash5::Sha3_512 ) );

		return PBVaultDatabase::changeVaultPassword( oldPass, newPass );
	}

	return false;
};

QByteArray PBVault::generatePassword() {

	FILE *randF = fopen( "/dev/urandom", "r" );
	char randData[ 1025 ] = { 0 };
	fread( randData, 1, 1024, randF );
	fclose( randF );

	return QCryptographicHash5::hash( QByteArray( randData, 1024 ), QCryptographicHash5::Sha3_512 );
};
