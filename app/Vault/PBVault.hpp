/*
    *
    * PBVault.hpp - PiBreeze Vault Header
    *
*/

#pragma once

#ifdef STANDALONE
	#include "Global2.hpp"
#else
	#include "Global.hpp"
#endif
#include "PBVaultDatabase.hpp"

class PBVault {

	public:
		enum Options {
			StoreVaultKeyForSession = 0x39F064,					// Store the vault key in memory
			AskVaultKeyEverytime								// Ask the key from the user everytime
		};

		static PBVault* instance();

		static void setVaultOption( PBVault::Options opt );

		/* Decrypt and view/edit file */
		bool decryptFile( QString path );

		/* Encrypt and store the file */
		bool encryptFile( QString path );

		/* Decrypt and mount the directory at @path */
		bool decryptDirectory( QString path );

		/* Unmount and encrypt the directory at @path */
		bool encryptDirectory( QString path );

		/* Check if the directory @path is mounted */
		bool isDirectoryDecrypted( QString path );

		/* Get vault pass */
		QByteArray vaultPassword();

		/* Change Vault Password */
		bool changeVaultPassword();

		/* Generate a random password */
		QByteArray generatePassword();

	private:
		PBVault();

		static PBVault* vault;
		bool init;

		static QByteArray vaultPass;
		static PBVault::Options mKeyStoreOption;
};
