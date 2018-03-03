/*
	*
	* PBMimeTypeModel.cpp - PiBreeze PBMimeTypeModel Class
	*
*/

#include "PBMimeTypeModel.hpp"

static QMutex mutex;

inline bool matchesFilter( QStringList filters, QString text ) {

	Q_FOREACH( QString filter, filters )
		if ( text.contains( QRegExp( filter, Qt::CaseInsensitive, QRegExp::Wildcard ) ) )
			return true;

	return false;
};

PBMimeIconUpdater::PBMimeIconUpdater( QString root, QStringList entries, bool *term ) : QThread() {

	mTerminate = term;

	entryList.clear();
	entryList << entries;

	rootPath.clear();
	rootPath = root;
	if ( not root.endsWith( "/" ) )
		rootPath += "/";
};

PBMimeIconUpdater::~PBMimeIconUpdater() {

	terminate();
	wait();
};

void PBMimeIconUpdater::run() {

	if ( rootPath != "/dev/" ) {
		foreach( QString entry, entryList ) {
			if ( *mTerminate )
				break;

			QString mName = mimeDb.mimeTypeForFile( rootPath + entry ).name();
			emit updated( rootPath, entry, PBIconManager::instance()->icon( mName ) );
		}
	}
};

PBMimeTypeModel::PBMimeTypeModel() : QAbstractItemModel() {

	/* Switch for temination of data gathering */
	mTerminate = false;

	/* Useless swtiches */
	currentLoadStatus.loading = false;
	currentLoadStatus.stopLoading = false;

	/* Root node */
	rootNode = new PBMimeTypeNode();
};

PBMimeTypeModel::~PBMimeTypeModel() {

};

bool PBMimeTypeModel::isCategorizationEnabled() {

	return true;
};

int PBMimeTypeModel::rowCount( const QModelIndex & parent ) const {

	if ( parent.column() > 0 )
		return 0;

	if ( not parent.isValid() )
		return rootNode->childCount();

	else
		return static_cast<PBMimeTypeNode*>( parent.internalPointer() )->childCount();
};

int PBMimeTypeModel::rowCount( QString mCategory ) const {

	if ( not mCategoryList.contains( mCategory ) or mCategory.isEmpty() or mCategory.isNull() )
		return 0;

	return categoryRowMap[ mCategory ].count();
};

int PBMimeTypeModel::categoryCount() const {

	return mCategoryList.count();
};

int PBMimeTypeModel::columnCount( const QModelIndex & parent ) const {

	Q_UNUSED( parent );
	return 1;
};

Qt::ItemFlags PBMimeTypeModel::flags( const QModelIndex & index ) const {

	if ( not index.isValid() )
		return Qt::NoItemFlags;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
};

QVariant PBMimeTypeModel::data( const QModelIndex &index, int role ) const {

	if ( not index.isValid() or index.column() )
		return QVariant();

	PBMimeTypeNode *node = static_cast<PBMimeTypeNode*>( index.internalPointer() );
	switch( role ) {

		case Qt::DisplayRole:
			return node->name();

		case Qt::DecorationRole:
			node->icon();

		case Qt::TextAlignmentRole:
			return ( 0x0001 | 0x0080 );

		case Qt::InitialSortOrderRole:
			return Qt::AscendingOrder;

		case Qt::AccessibleTextRole:
			return node->name();

		case Qt::ToolTipRole:
			return QString( "%1/%2" ).arg( node->category() ).arg( node->name() );

		default:
			return QVariant();
	}
};

QModelIndex PBMimeTypeModel::index( int row, int column, const QModelIndex &parent ) const {

    if ( row < 0 || column < 0 )
        return QModelIndex();

    if ( not ( ( row < rowCount( parent ) ) and ( column < columnCount( parent ) ) ) )
		return QModelIndex();

	PBMimeTypeNode *parentNode;
	if ( not parent.isValid() )
		parentNode = rootNode;

	else
		parentNode = (PBMimeTypeNode *)parent.internalPointer();

	PBMimeTypeNode *childNode = parentNode->child( row );
	if ( childNode )
		return createIndex( row, column, childNode );

	else
		return QModelIndex();
};

QModelIndex PBMimeTypeModel::index( QString name, const QModelIndex &parent ) const {

	if ( not mChildNames.contains( name ) )
		return QModelIndex();

	PBMimeTypeNode *parentNode;
	if ( not parent.isValid() )
		parentNode = rootNode;

	else
		parentNode = (PBMimeTypeNode *)parent.internalPointer();

	int row = parentNode->child( name )->row();
	PBMimeTypeNode *childNode = parentNode->child( name );

	if ( childNode )
		return createIndex( row, 0, childNode );

	return QModelIndex();
};

QModelIndex PBMimeTypeModel::parent( const QModelIndex &index ) const {

	if ( not index.isValid() )
		return QModelIndex();

	PBMimeTypeNode *childNode = ( PBMimeTypeNode * )index.internalPointer();
	PBMimeTypeNode *parentNode = childNode->parent();

	if ( parentNode == rootNode )
		return QModelIndex();

	return createIndex( parentNode->row(), 0, parentNode );
};

QString PBMimeTypeModel::category( const QModelIndex &index ) const {

	if ( not index.isValid() )
		return QString( "" );

	PBMimeTypeNode *childItem = ( PBMimeTypeNode * )index.internalPointer();
	return childItem->category();
};

int PBMimeTypeModel::categoryIndex( const QModelIndex &index ) const {

	if ( not index.isValid() )
		return -1;

	PBMimeTypeNode *childItem = ( PBMimeTypeNode * )index.internalPointer();
	return mCategoryList.indexOf( childItem->category() );
};

QStringList PBMimeTypeModel::categories() const {

	return mCategoryList;
};

QPixmap PBMimeTypeModel::pixmapForCategory( QString categoryName ) const {

	return QIcon::fromTheme( categoryName + "-x-generic", QIcon::fromTheme( "application-octet-stream" ) ).pixmap( 24, 24 );
};

void PBMimeTypeModel::foldCategory( QString categoryName ) {

	categoryVisibilityMap[ categoryName ] = false;
};

void PBMimeTypeModel::openCategory( QString categoryName ) {

	categoryVisibilityMap[ categoryName ] = true;
};

bool PBMimeTypeModel::isCategoryOpen( QString categoryName ) const {

	return categoryVisibilityMap[ categoryName ];
};

int PBMimeTypeModel::indexListCountForCategory( QString mCategory ) const {

	if ( not mCategoryList.contains( mCategory ) or mCategory.isEmpty() or mCategory.isNull() )
		return 0;

	return categoryRowMap[ mCategory ].count();
};

QModelIndexList PBMimeTypeModel::indexListForCategory( QString mCategory ) const {

	QModelIndexList mList;

	if ( not mCategoryList.contains( mCategory ) or mCategory.isEmpty() or mCategory.isNull() )
		return mList;

	Q_FOREACH( int row, categoryRowMap[ mCategory ] )
		mList << index( row, 0, QModelIndex() );

	return mList;
};

QModelIndexList PBMimeTypeModel::categorySiblings( QModelIndex idx ) const {

	if ( not idx.isValid() )
		return QModelIndexList();

	QModelIndexList mList = indexListForCategory( category( idx ) );
	mList.removeAll( idx );

	return mList;
};

QStringList PBMimeTypeModel::nameFilters() {

	return __nameFilters;
};

void PBMimeTypeModel::setNameFilters( QStringList filters ) {

	__nameFilters.clear();
	__nameFilters << filters;

	setupModelData();
};

void PBMimeTypeModel::clearNameFilters() {

	__nameFilters.clear();
	setupModelData();
};

void PBMimeTypeModel::sort() {

	rootNode->sort();

	// Create a map of categoryIndex versus rows
	categoryRowMap.clear();
	foreach( PBMimeTypeNode *item, rootNode->children() )
		categoryRowMap[ item->category() ] << item->row();

	emit layoutChanged();
};

void PBMimeTypeModel::reload() {

	setupModelData();
};

void PBMimeTypeModel::setupModelData() {

	mChildNames.clear();
	rootNode->clearChildren();
	currentLoadStatus.loading = true;

	beginResetModel();
	Q_FOREACH( QMimeType mType, mimeDb.allMimeTypes() ) {
		QStringList cnList( mType.name().split( "/" ) );

		if ( not mCategoryList.contains( cnList.at( 0 ) ) )
			mCategoryList << cnList.at( 0 );

		mChildNames << cnList.at( 1 );
		rootNode->addChild( new PBMimeTypeNode( mType, rootNode ) );
	}
	endResetModel();

	currentLoadStatus.loading = false;

	emit modelLoaded();
};

void PBMimeTypeModel::terminateInfoGathering() {

	mTerminate = true;
};

void PBMimeTypeModel::updateAllNodes() {

};

void PBMimeTypeModel::saveInfo( QString root, QString entry, QStringList info ) {

};
