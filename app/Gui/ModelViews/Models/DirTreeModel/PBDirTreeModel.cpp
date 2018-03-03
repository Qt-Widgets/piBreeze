/*
	*
	* PBDirTreeModel.cpp - PiBreeze PBDirTreeModel Class
	*
*/

#include "PBDirTreeModel.hpp"

PBDirTreeModel::PBDirTreeModel() : QAbstractItemModel() {

	/* By default we don't show hidden files */
	__showHidden = false;

	/* Switch for temination of data gathering */
	__terminate = false;

	/* Useless swtiches */
	currentLoadStatus.loading = false;
	currentLoadStatus.stopLoading = false;

	/* Root node */
	plantTree();
};

PBDirTreeModel::~PBDirTreeModel() {

};

int PBDirTreeModel::rowCount( const QModelIndex &parent ) const {

	PBTreeBranch *parentItem;
	if ( parent.column() > 0 )
		return 0;

	if ( not parent.isValid() )
		parentItem = tree;

	else
		parentItem = static_cast<PBTreeBranch*>( parent.internalPointer() );

	if ( parent.isValid() and not exploredIndexes.contains( parent ) ) {

		parentItem->explore();
		exploredIndexes << parent;
	}

	return parentItem->childCount();
};

int PBDirTreeModel::columnCount( const QModelIndex & ) const {

	return 1;
};

Qt::ItemFlags PBDirTreeModel::flags( const QModelIndex &idx ) const {

	if ( not idx.isValid() )
		return Qt::NoItemFlags;

	QString path( idx.data( Qt::UserRole + 1 ).toString() );
	if ( isReadable( path ) and isWritable( path ) )
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable;

	else if ( isReadable( path ) )
		return Qt::ItemIsEnabled;

	else
		return Qt::NoItemFlags;
};

QVariant PBDirTreeModel::data( const QModelIndex &index, int role ) const {

	if ( not index.isValid() )
		return QVariant();

	PBTreeBranch *node = static_cast<PBTreeBranch*>( index.internalPointer() );
	switch( role ) {

		case Qt::DisplayRole: {
			if ( index.column() == 0 )
				return node->name();

			return QVariant();
		}

		case Qt::DecorationRole: {
			if ( index.column() == 0 )
				return node->icon();

			return QVariant();
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
			return node->name();
		}

		case Qt::ToolTipRole: {
			if ( index.column() == 0 )
				return node->data( Qt::UserRole + 1 );

			return QString();
		}

		case Qt::UserRole + 1: {

			return node->data( Qt::UserRole + 1 );
		}

		default: {

			return QVariant();
		}
	}
};

QVariant PBDirTreeModel::headerData( int section, Qt::Orientation orientation, int role ) const {

	if ( section or orientation != Qt::Horizontal )
		return QVariant();

	switch( role ) {
		case Qt::DisplayRole:
			return "Name";

		case Qt::TextAlignmentRole:
			return Qt::AlignCenter;

		default:
			return QVariant();
	}
};

QModelIndex PBDirTreeModel::index( int row, int column, const QModelIndex &parent ) const {

    if ( row < 0 or column < 0 )
        return QModelIndex();

    if ( ( row > rowCount( parent ) ) and ( column > columnCount( parent ) ) )
		return QModelIndex();

	PBTreeBranch *parentNode;
	if ( not parent.isValid() )
		parentNode = tree;

	else
		parentNode = (PBTreeBranch *)parent.internalPointer();

	if ( parent.isValid() and not exploredIndexes.contains( parent ) ) {

		parentNode->explore();
		exploredIndexes << parent;
	}

	PBTreeBranch *childNode = parentNode->child( row );
	if ( childNode )
		return createIndex( row, column, childNode );

	else
		return QModelIndex();
};

QModelIndex PBDirTreeModel::index( QString name, const QModelIndex &parent ) const {

	PBTreeBranch *parentNode;
	if ( not parent.isValid() )
		parentNode = tree;

	else
		parentNode = static_cast<PBTreeBranch*>( parent.internalPointer() );

	if ( parent.isValid() and not exploredIndexes.contains( parent ) ) {

		parentNode->explore();
		exploredIndexes << parent;
	}

	PBTreeBranch *childNode = parentNode->child( name );
	if ( childNode )
		return createIndex( childNode->row(), 0, childNode );

	return QModelIndex();
};

QModelIndex PBDirTreeModel::parent( const QModelIndex &idx ) const {

	if ( not idx.isValid() )
		return QModelIndex();

	PBTreeBranch *childNode = ( PBTreeBranch * )idx.internalPointer();
	PBTreeBranch *parentNode = childNode->parent();

	if ( parentNode == tree )
		return QModelIndex();

	return createIndex( parentNode->row(), 0, parentNode );
};

bool PBDirTreeModel::hasChildren( const QModelIndex &idx ) const {

	if ( not idx.isValid() )
		return true;

	else {
		if ( exploredIndexes.contains( idx ) ) {
			PBTreeBranch *branch = static_cast<PBTreeBranch*>( idx.internalPointer() );
			return ( branch->childCount() > 0 );
		}

		if ( not isReadable( idx.data( Qt::UserRole + 1 ).toString() ) ) {

			return false;
		}

		return true;
	}
};

bool PBDirTreeModel::showHidden() const {

	return __showHidden;
};

void PBDirTreeModel::setShowHidden( bool shown ) {

	// plantTree();
};

Qt::DropActions PBDirTreeModel::supportedDragActions() const {

	return Qt::IgnoreAction;
};

Qt::DropActions PBDirTreeModel::supportedDropActions() const {

	return Qt::IgnoreAction;
};

Qt::ItemFlags PBDirTreeModel::flags( const QModelIndex idx ) const {

	QString path( idx.data( Qt::UserRole + 1 ).toString() );

	/* We have read and write access */
	if ( isReadable( path ) and isWritable( path ) )
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

	/* We have just read access */
	else if ( isReadable( path ) )
		return Qt::ItemIsEnabled;

	/* We have no access */
	else
		return Qt::NoItemFlags;
};

QStringList PBDirTreeModel::mimeTypes() const {

	QStringList types;
	types << "text/uri-list";
	return types;
};

QStringList PBDirTreeModel::nameFilters() {

	return __nameFilters;
};

void PBDirTreeModel::setNameFilters( QStringList filters ) {

	__nameFilters.clear();
	__nameFilters << filters;
};

void PBDirTreeModel::clearNameFilters() {

	__nameFilters.clear();
};

void PBDirTreeModel::reload() {

	// plantTree();
};

QString PBDirTreeModel::nodeName( const QModelIndex idx ) const {

	return idx.data( 0 ).toString();
};

QString PBDirTreeModel::nodePath( const QModelIndex idx ) const {

	if ( not idx.isValid() )
		return QString();

	return idx.data( Qt::UserRole + 1 ).toString();
};

QFileInfo PBDirTreeModel::nodeInfo( const QModelIndex idx ) const {

	if ( not idx.isValid() )
		return QFileInfo();

	return QFileInfo( idx.data( Qt::UserRole + 1 ).toString() );
};

void PBDirTreeModel::plantTree() {

	beginResetModel();

	tree = new PBTreeBranch( "Name", QIcon::fromTheme( "folder" ), 0 );
	tree->addChild( new PBTreeBranch( "/", QIcon::fromTheme( "folder" ), tree ) );

	endResetModel();
};

void PBDirTreeModel::terminateInfoGathering() {

	__terminate = true;
};
