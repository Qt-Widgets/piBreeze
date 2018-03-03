/*
	*
	* PBTrashModel.cpp - PiBreeze PBTrashModel Class
	*
*/

#include "PBTrashModel.hpp"

PBTrashModel::PBTrashModel() : QAbstractItemModel() {

	mCategorizationEnabled = true;
	updatedNodes = 0;

	currentLoadStatus.loading = false;
	currentLoadStatus.stopLoading = false;

	oldRoots.clear();
	curIndex = 0;

	rootNode = new PBTrashNode();

	setupModelData();
};

PBTrashModel::~PBTrashModel() {

};

bool PBTrashModel::isCategorizationEnabled() {

	return mCategorizationEnabled;
};

void PBTrashModel::setCategorizationEnabled( bool enabled ) {

	Q_UNUSED( enabled );

	mCategorizationEnabled = true;
	// sort();
};

int PBTrashModel::rowCount( const QModelIndex & parent ) const {

	if ( parent.column() > 0 )
		return 0;

	if ( not parent.isValid() )
		return rootNode->childCount();

	else
		return static_cast<PBTrashNode*>( parent.internalPointer() )->childCount();
};

int PBTrashModel::rowCount( QString mCategory ) const {

	if ( not rootNode->categoryList().contains( mCategory ) or mCategory.isEmpty() or mCategory.isNull() )
		return 0;

	return categoryRowMap[ mCategory ].count();
};

int PBTrashModel::categoryCount() const {

	return rootNode->categoryCount();
};

int PBTrashModel::columnCount( const QModelIndex & parent ) const {

	Q_UNUSED( parent );
	return 4;
};

Qt::ItemFlags PBTrashModel::flags( const QModelIndex & index ) const {

	if ( not index.isValid() )
		return Qt::NoItemFlags;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
};

QVariant PBTrashModel::data( const QModelIndex &index, int role ) const {

	if ( not index.isValid() )
		return QVariant();

	PBTrashNode *node = static_cast<PBTrashNode*>( index.internalPointer() );
	switch( role ) {

		case Qt::DisplayRole: {
			return node->name();
		}

		case Qt::DecorationRole: {
			return node->icon();
		}

		case Qt::TextAlignmentRole: {
			if ( index.column() == 0 )
				return ( 0x0001 | 0x0080 );

			else if ( index.column() == 1 )
				return ( 0x0002 | 0x0080 );

			else
				return Qt::AlignCenter;
		}

		case Qt::InitialSortOrderRole: {
			return Qt::AscendingOrder;
		}

		case Qt::AccessibleTextRole: {
			return node->data( index.column() );
		}

		case Qt::ToolTipRole: {
			return QString(
						"<center>%1<br>%2<br>%3</center>"
					).arg( node->name() ).arg( node->size() ).arg( node->deletionDate().toString( "MMM dd, yyyy hh:mm:ss" ) );
		}

		/* Original Location */
		case Qt::UserRole + 1: {
			return node->originalPath();
		}

		/* Size */
		case Qt::UserRole + 2: {
			return node->size();
		}

		/* Deletion Date */
		case Qt::UserRole + 3: {
			return node->deletionDate().toString( "MMM dd, yyyy hh:mm:ss" );
		}

		/* Trash Path */
		case Qt::UserRole + 4: {
			return node->trashPath();
		}

		/* Trash Info Path */
		case Qt::UserRole + 5: {
			return node->trashInfoPath();
		}

		default: {
			return QVariant();
		}
	}
};

QVariant PBTrashModel::headerData( int section, Qt::Orientation orientation, int role ) const {

	QStringList headerList = QStringList() << "FileName" << "Original Path" << "Size" << "Deleteion Date";

	if ( ( orientation == Qt::Horizontal ) and ( role == Qt::DisplayRole ) )
		return headerList.at( section );

	else if ( ( orientation == Qt::Horizontal ) and ( role == Qt::TextAlignmentRole ) )
		return ( 0x0001 | 0x0080 );

	else
		return QVariant();
};

bool PBTrashModel::setData( const QModelIndex &index, QString value, int role ) {

	bool ok = rootNode->child( index.row() )->setData( role, value );
	if ( ok )
		emit dataChanged( index, index );

	return ok;
};

bool PBTrashModel::insertNode( QString nodeName ) {

	Q_UNUSED( nodeName );

	return true;
};

void PBTrashModel::updateNode( QString nodeName ) {

	Q_UNUSED( nodeName );

	return;
};

bool PBTrashModel::removeNode( QString nodeName ) {

	Q_UNUSED( nodeName );

	return false;
};

QModelIndex PBTrashModel::index( int row, int column, const QModelIndex &parent ) const {

	if ( not hasIndex( row, column, parent ) )
		return QModelIndex();

	PBTrashNode *parentNode;
	if ( not parent.isValid() )
		parentNode = rootNode;

	else
		parentNode = (PBTrashNode *)parent.internalPointer();

	PBTrashNode *childNode = parentNode->child( row );
	if ( childNode )
		return createIndex( row, column, childNode );

	else
		return QModelIndex();
};

QModelIndex PBTrashModel::index( QString name, const QModelIndex &parent ) const {

	if ( not __childNames.contains( name ) )
		return QModelIndex();

	PBTrashNode *parentNode;
	if ( not parent.isValid() )
		parentNode = rootNode;

	else
		parentNode = (PBTrashNode *)parent.internalPointer();

	int row = parentNode->child( name )->row();
	PBTrashNode *childNode = parentNode->child( name );

	if ( childNode )
		return createIndex( row, 0, childNode );

	return QModelIndex();
};

QModelIndex PBTrashModel::parent( const QModelIndex &index ) const {

	if ( not index.isValid() )
		return QModelIndex();

	PBTrashNode *childNode = ( PBTrashNode * )index.internalPointer();
	PBTrashNode *parentNode = childNode->parent();

	if ( parentNode == rootNode )
		return QModelIndex();

	return createIndex( parentNode->row(), 0, parentNode );
};

QString PBTrashModel::category( const QModelIndex &index ) const {

	if ( not index.isValid() )
		return QString();

	PBTrashNode *childItem = ( PBTrashNode * )index.internalPointer();
	return childItem->category();
};

int PBTrashModel::categoryIndex( const QModelIndex &index ) const {

	if ( not index.isValid() )
		return -1;

	PBTrashNode *childItem = ( PBTrashNode * )index.internalPointer();
	return childItem->categoryIndex();
};

QStringList PBTrashModel::categories() const {

	return rootNode->categoryList();
};

void PBTrashModel::hideCategory( QString mCategory ) {

	if ( rootNode->categoryList().contains( mCategory ) )
		categoryVisibilityMap[ mCategory ] = false;
};

void PBTrashModel::showCategory( QString mCategory ) {

	if ( rootNode->categoryList().contains( mCategory ) )
		categoryVisibilityMap[ mCategory ] = true;
};

bool PBTrashModel::isCategoryVisible( QString mCategory ) const {

	if ( rootNode->categoryList().contains( mCategory ) )
		return categoryVisibilityMap[ mCategory ];

	return false;
};

int PBTrashModel::indexListCountForCategory( QString mCategory ) const {

	if ( not rootNode->categoryList().contains( mCategory ) or mCategory.isEmpty() or mCategory.isNull() )
		return 0;

	return categoryRowMap[ mCategory ].count();
};

QModelIndexList PBTrashModel::indexListForCategory( QString mCategory ) const {

	QModelIndexList mList;

	if ( not rootNode->categoryList().contains( mCategory ) or mCategory.isEmpty() or mCategory.isNull() )
		return mList;

	foreach( int row, categoryRowMap[ mCategory ] ) {
		mList << index( row, 0, QModelIndex() );
	}

	return mList;
};

Qt::DropActions PBTrashModel::supportedDragActions() const {

	return Qt::CopyAction | Qt::MoveAction | Qt::LinkAction;
};

Qt::DropActions PBTrashModel::supportedDropActions() const {

	return Qt::CopyAction | Qt::MoveAction | Qt::LinkAction;
};

Qt::ItemFlags PBTrashModel::flags( const QModelIndex index ) const {

	Q_UNUSED( index );

	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
};

QStringList PBTrashModel::mimeTypes() const {

	QStringList types;
	types << "text/uri-list";
	return types;
};

bool PBTrashModel::filter( Filters filter ) {

	Q_UNUSED( filter );

	return false;
};

void PBTrashModel::setFilter( Filters filter, bool on ) {

	Q_UNUSED( filter );
	Q_UNUSED( on );

	return;
};

QStringList PBTrashModel::nameFilters() {

	return __nameFilters;
};

void PBTrashModel::setNameFilters( QStringList filters ) {

	__nameFilters.clear();
	__nameFilters << filters;

	// setupModelData();
};

void PBTrashModel::clearNameFilters() {

	__nameFilters.clear();
	setupModelData();
};

void PBTrashModel::reload() {

	setupModelData();
};

void PBTrashModel::restore( QModelIndexList toBeRestored ) {

	QModelIndexList failed;
	QSettings trashInfo( "PiBreeze", "TrashInfo" );

	Q_FOREACH( QModelIndex idx, toBeRestored ) {
		PBTrashNode *node = static_cast<PBTrashNode*>( idx.internalPointer() );
		__childNames.removeOne( node->name() );

		if ( exists( node->originalPath() ) ) {
			failed << idx;
			continue;
		}

		if ( rename( node->trashPath().toLocal8Bit().data(), node->originalPath().toLocal8Bit().data() ) ) {
			failed << idx;
			continue;
		}

		remove( node->trashInfoPath().toLocal8Bit().data() );
		trashInfo.remove( QUrl::toPercentEncoding( node->name() ) );
		trashInfo.sync();

		rootNode->removeChild( node );
	};

	emit restored( failed );
	setupModelData();
};

void PBTrashModel::removeFromDisk( QModelIndexList toBeDeleted ) {

	QModelIndexList failed;
	QSettings trashInfo( "PiBreeze", "TrashInfo" );

	Q_FOREACH( QModelIndex idx, toBeDeleted ) {
		PBTrashNode *node = static_cast<PBTrashNode*>( idx.internalPointer() );
		__childNames.removeOne( node->name() );

		if( remove( node->trashPath().toLocal8Bit().data() ) ) {
			failed << idx;
			continue;
		}

		remove( node->trashInfoPath().toLocal8Bit().data() );
		trashInfo.remove( QUrl::toPercentEncoding( node->name() ) );
		trashInfo.sync();

		rootNode->removeChild( node );
	};

	emit deleted( failed );
	setupModelData();
};

void PBTrashModel::setupModelData() {

	__childNames.clear();
	rootNode->clearChildren();
	currentLoadStatus.loading = true;

	emit trashLoading();

	QSettings trashInfo( "PiBreeze", "TrashInfo" );

	beginResetModel();
	/* Loading home trash */
	QString trashLoc = PBXdg::homeTrashLocation();
	if ( not trashLoc.isEmpty() ) {
		QDir trash( trashLoc + "/info/" );
		trash.setNameFilters( QStringList() << "*.trashinfo" );
		Q_FOREACH( QString entry, trash.entryList() ) {

			if ( currentLoadStatus.stopLoading ) {
				endResetModel();
				currentLoadStatus.stopLoading = false;
				return;
			}

			QSettings trashInfo( trash.absoluteFilePath( entry ), QSettings::NativeFormat );
			QString origPath = trashInfo.value( "Trash Info/Path" ).toString();
			QString delDate = trashInfo.value( "Trash Info/DeletionDate" ).toString();
			QString trashPath = QString( trashInfo.fileName() ).replace( "/info/", "/files/" ).replace( ".trashinfo", "" );

			QStringList data = QStringList() << baseName( origPath ) << origPath << delDate << trashPath;

			rootNode->addChild( new PBTrashNode( data, getCategory( data.at( 2 ) ), rootNode ) );
			__childNames << baseName( origPath );
		}
	}

	/* Loading all other trash */
	Q_FOREACH( PBDeviceInfo devInfo, PBDeviceManager::allMounts() ) {
		/* We don't want to check the home directory for trash once more */
		if ( devInfo.mountPoint() == PBXdg::home() )
			continue;

		trashLoc = PBXdg::trashLocation( devInfo.mountPoint() );
		if ( PBXdg::homeTrashLocation() == trashLoc )
			continue;

		if ( not trashLoc.isEmpty() ) {
			QDir trash( trashLoc + "/info/" );
			trash.setNameFilters( QStringList() << "*.trashinfo" );
			Q_FOREACH( QString entry, trash.entryList() ) {

				if ( currentLoadStatus.stopLoading ) {
					endResetModel();
					currentLoadStatus.stopLoading = false;
					return;
				}

				QSettings trashInfo( trash.absoluteFilePath( entry ), QSettings::NativeFormat );
				QString origPath = trashInfo.value( "Trash Info/Path" ).toString();
				QString delDate = trashInfo.value( "Trash Info/DeletionDate" ).toString();
				QString trashPath = QString( trashInfo.fileName() ).replace( "/info/", "/files/" ).replace( ".trashinfo", "" );

				QStringList data = QStringList() << baseName( origPath ) << origPath << delDate << trashPath;

				rootNode->addChild( new PBTrashNode( data, getCategory( data.at( 2 ) ), rootNode ) );
				__childNames << baseName( origPath );
			}
		}
	}
	endResetModel();

	sort();

	currentLoadStatus.loading = false;

	emit trashLoaded();
};

QString PBTrashModel::getCategory( QString dateStr ) {

	QDateTime date = QDateTime::fromString( dateStr, "yyyyMMddThh:mm:ss" );
	if ( date.isValid() ) {
		if ( date.date() == QDate::currentDate() )
			return "Today";

		else if ( date.date().weekNumber() == QDate::currentDate().weekNumber() )
			return "This Week";

		else if (  date.date().weekNumber() == QDate::currentDate().weekNumber() - 1 )
			return "Last Week";

		else if (  date.date().month() == QDate::currentDate().month() )
			return "This Month";

		else if (  date.date().month()== QDate::currentDate().month() - 1 )
			return "Last Month";

		else
			return date.toString( "MMMM yyyy" );
	}

	return QString( "Uncategorized" );
};

void PBTrashModel::recategorize() {

	foreach( PBTrashNode *node, rootNode->children() )
		node->setCategory( getCategory( node->allData().at( 2 ) ) );

	rootNode->updateCategories();
};

void PBTrashModel::handleNodeCreated( QString node ) {

	insertNode( baseName( node ) );
};

void PBTrashModel::handleNodeChanged( QString node ) {

	updateNode( baseName( node ) );
};

void PBTrashModel::handleNodeDeleted( QString node ) {

	removeNode( baseName( node ) );
};

void PBTrashModel::sort() {

	recategorize();
	rootNode->sort();

	// Create a map of categoryIdndex versus rows
	categoryRowMap.clear();
	foreach( PBTrashNode *item, rootNode->children() )
		categoryRowMap[ item->category() ] << item->row();

	foreach( QString mCategory, rootNode->categoryList() )
		categoryVisibilityMap[ mCategory ] = true;

	emit trashLoaded();
};
