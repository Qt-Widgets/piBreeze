/*
	*
	* PBDeviceInfo.hpp - PBDeviceInfo Class Header
	*
*/

#pragma once

#include "common.hpp"

class PBDeviceInfoPrivate;

class PBCOMMON_DLLSPEC PBDeviceInfo {

	public:
		PBDeviceInfo();
		explicit PBDeviceInfo(const QString path);
		PBDeviceInfo(const PBDeviceInfo &other);
		~PBDeviceInfo();

		PBDeviceInfo &operator=(const PBDeviceInfo &other);

		QString mountPoint() const;
		QString device() const;
		QString fileSystemType() const;
		QString deviceType() const;
		QString displayName() const;

		qint64 bytesTotal() const;
		qint64 bytesUsed() const;
		qint64 bytesAvailable() const;
		int blockSize() const;

		bool isReadOnly() const;
		bool isValid() const;

	private:
		// friend class PBDeviceInfoPrivate;
		friend class PBDeviceManager;
		friend bool operator==(const PBDeviceInfo &first, const PBDeviceInfo &second);
		QExplicitlySharedDataPointer<PBDeviceInfoPrivate> d;

		PBDeviceInfo( PBDeviceInfoPrivate *infoPriv );
};

inline bool operator==( const PBDeviceInfo &first, const PBDeviceInfo &second ) {
    if ( first.d == second.d )
        return true;

    return first.device() == second.device();
};

inline bool operator!=( const PBDeviceInfo &first, const PBDeviceInfo &second ) {

    return !( first == second );
}

Q_DECLARE_METATYPE(PBDeviceInfo)

class PBCOMMON_DLLSPEC PBDeviceManager : public QObject {
	Q_OBJECT

	public :
		/* List of all mounted volumes: drives + vfs */
		static QList<PBDeviceInfo> allMounts();

		/* List of for all mounted drives */
		static QList<PBDeviceInfo> allDrives();

		/* List of all mounted vfs */
		static QList<PBDeviceInfo> allVirtualMounts();

		/* List of all mounted vfs */
		static QList<PBDeviceInfo> allEncryptedMounts();

		/* Volume info for path */
		static PBDeviceInfo deviceInfoForPath( QString path );

	private:
		/* Reload all the devices */
		static void pollDevices();

		static QHash<QString, PBDeviceInfo> devicesList;
		static bool init;
};
