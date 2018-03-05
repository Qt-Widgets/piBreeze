/*
	*
	* PBDeviceInfo.cpp - PBDeviceInfo Class
	*
*/

#include "PBDeviceInfo.hpp"
#include "PBDeviceInfo_p.hpp"

PBDeviceInfo::PBDeviceInfo() {

	d = new PBDeviceInfoPrivate();
};

PBDeviceInfo::PBDeviceInfo( const QString path ) {

	d = PBDeviceInfo( PBDeviceManager::deviceInfoForPath( path ) ).d;
};

PBDeviceInfo::PBDeviceInfo( const PBDeviceInfo &info ) {

	d = info.d;
};

PBDeviceInfo::PBDeviceInfo( PBDeviceInfoPrivate *infoPriv ) {

	d = QExplicitlySharedDataPointer<PBDeviceInfoPrivate>( infoPriv );
};

PBDeviceInfo::~PBDeviceInfo() {

};

PBDeviceInfo &PBDeviceInfo::operator=(const PBDeviceInfo &other) {

    d = other.d;
    return *this;
};

QString PBDeviceInfo::mountPoint() const {

	return d->mountPoint;
};

QString PBDeviceInfo::device() const {

	return d->device;
};

QString PBDeviceInfo::fileSystemType() const {

	return d->fileSystemType;
};

QString PBDeviceInfo::deviceType() const {

	return d->deviceType;
};

QString PBDeviceInfo::displayName() const {

	return d->label;
};

qint64 PBDeviceInfo::bytesTotal() const {

	return d->bytesTotal;
};

qint64 PBDeviceInfo::bytesUsed() const {

	return d->bytesUsed;
};

qint64 PBDeviceInfo::bytesAvailable() const {

	return d->bytesAvailable;
};

int PBDeviceInfo::blockSize() const {

	return d->blockSize;
};

bool PBDeviceInfo::isReadOnly() const {

	return d->readOnly;
};

bool PBDeviceInfo::isValid() const {

	return d->mIsValid;
};

bool PBDeviceManager::init = false;
QHash<QString, PBDeviceInfo> PBDeviceManager::devicesList = QHash<QString, PBDeviceInfo>();

QList<PBDeviceInfo> PBDeviceManager::allMounts() {

	pollDevices();

	return devicesList.values();
};

QList<PBDeviceInfo> PBDeviceManager::allDrives() {

	pollDevices();

	QList<PBDeviceInfo> devList;
	Q_FOREACH( PBDeviceInfo info, devicesList.values() ){
		if ( info.device().startsWith( "/dev/" ) )
			devList << info;
	}

	return devList;
};

QList<PBDeviceInfo> PBDeviceManager::allVirtualMounts() {

	pollDevices();

	QList<PBDeviceInfo> devList;
	Q_FOREACH( PBDeviceInfo info, devicesList.values() ) {
		if ( info.device().startsWith( "/dev/" ) )
			continue;

		if ( info.mountPoint().startsWith( "/run/" ) )
			continue;

		if ( info.mountPoint().startsWith( "/proc/" ) )
			continue;

		if ( info.mountPoint().startsWith( "/sys/" ) )
			continue;

		devList << info;
	}

	return devList;
};

QList<PBDeviceInfo> PBDeviceManager::allEncryptedMounts() {

	pollDevices();

	QList<PBDeviceInfo> devList;
	Q_FOREACH( PBDeviceInfo info, devicesList.values() ) {
		if ( info.fileSystemType().contains( "encfs" ) )
			devList << info;

		else if ( info.fileSystemType().contains( "cryfs" ) )
			devList << info;

		else
			continue;
	}

	return devList;
};

PBDeviceInfo PBDeviceManager::deviceInfoForPath( QString path ) {

	pollDevices();

	if ( isDir( path ) and not path.endsWith( "/" ) )
		path += "/";

	if ( devicesList.keys().contains( path ) )
		return devicesList.value( path );

	int bestMatch = path.size();
	PBDeviceInfo bestDevice;

	Q_FOREACH( QString mt, devicesList.keys() ) {
		if ( not path.startsWith( mt ) )
			continue;

		int match = path.compare( mt );
		if ( ( match > 0 ) and ( match < bestMatch ) ) {
			bestMatch = match;
			bestDevice = devicesList.value( mt );
		}
	}

	return bestDevice;
};

void PBDeviceManager::pollDevices() {

	/* mtab file path */
	FILE *fp = setmntent( "/etc/mtab", "r" );

	/* Entry struct */
	struct mntent *entry;

	/* Clear devices list */
	devicesList.clear();

	QStringList virtualFS;
	virtualFS << "sysfs" << "cgroup" << "cgroup2" << "proc" << "devtmpfs" << "devpts";
	virtualFS << "tmpfs" << "securityfs" << "pstore" << "autofs" << "mqueue";
	virtualFS << "debugfs" << "hugetlbfs" << "fusectl" << "fuse.gvfsd-fuse";
	virtualFS << "binfmt_misc" << "configfs";

	while( ( entry = getmntent( fp ) ) != NULL ) {
		/* Remove virtual mount points: dev, sys, proc etc */
		if ( virtualFS.contains( entry->mnt_type ) )
			continue;

		/* AppImage fix */
		if ( QString( entry->mnt_type ).contains( "AppImage" ) )
			continue;

		PBDeviceInfo info( new PBDeviceInfoPrivate( entry->mnt_fsname, entry->mnt_dir, entry->mnt_type ) );
		devicesList[ entry->mnt_dir ] = info;
	};

	endmntent( fp );

	init = true;
};
