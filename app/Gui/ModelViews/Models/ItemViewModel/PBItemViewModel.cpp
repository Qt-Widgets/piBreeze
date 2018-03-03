/*
	*
	* PBItemViewModel.cpp - PiBreeze PBItemViewModel Class
	*
*/

#include "PBItemViewModel.hpp"

static QMutex mutex;
QStringList lambdaUseFilterList;

inline int matchesFilter( QStringList filters, QString text ) {

	Q_FOREACH( QString filter, filters )
		if ( text.contains( QRegExp( filter, Qt::CaseInsensitive, QRegExp::Wildcard ) ) )
			return 1;

	return 0;
};

inline int scandirCallback( const struct dirent* entry ) {

	/* Always filter . and .. */
	if ( not strcmp( entry->d_name, "." ) or not strcmp( entry->d_name, ".." ) )
		return 0;

	/* If we are not to show hidden files/folders, filter all names starting with . */
	if ( not Settings->General.ShowHidden and not strncmp( entry->d_name, ".", 1 ) )
		return 0;

	/* Name filter */
	if ( lambdaUseFilterList.count() ) {
		/* Do not filter folders, if the flag is not set */
		if ( not Settings->General.FilterFolders and ( entry->d_type == DT_DIR ) )
			return 1;

		/* If the name matches any one of the nameFilters, show it */
		return matchesFilter( lambdaUseFilterList, entry->d_name );
	}

	/* If no filter, select all */
	return 1;
};

PBItemViewModel::PBItemViewModel( QObject *parent ) : QAbstractItemModel( parent ) {

	/* Do we show the special directories */
	mModelDataType = PBItemViewModel::FileSystem;
	mVirtualData = false;

	/* Categrorization enabled by default */
	mCategorizationEnabled = Settings->General.Grouping;

	/* Useless swtiches */
	currentLoadStatus.loading = false;
	currentLoadStatus.stopLoading = false;

	prevSort.column = Settings->General.SortColumn;
	prevSort.cs = Settings->General.SortCase;
	prevSort.categorized = Settings->General.Grouping;

	oldRoots.clear();
	curIndex = 0;
	prevFolder = Settings->Session.LastDir;

	/* Root node */
	rootNode = new PBItemViewNode();

	quickDataGatherer = new PBQuickFileInfoGatherer();

	/* PBFileSystemWatcher */
	watcher = new PBFileSystemWatcher();
	connect( watcher, SIGNAL( nodeCreated( QString ) ), this, SLOT( handleNodeCreated( QString ) ) );
	connect( watcher, SIGNAL( nodeChanged( QString ) ), this, SLOT( handleNodeChanged( QString ) ) );
	connect( watcher, SIGNAL( nodeDeleted( QString ) ), this, SLOT( handleNodeDeleted( QString ) ) );
	connect( watcher, SIGNAL( nodeRenamed( QString, QString ) ), this, SLOT( handleNodeRenamed( QString, QString ) ) );
	connect( watcher, SIGNAL( watchPathDeleted() ), this, SLOT( loadHome() ) );
};

PBItemViewModel::~PBItemViewModel() {

};

bool PBItemViewModel::isCategorizationEnabled() {

	return mCategorizationEnabled;
};

void PBItemViewModel::setCategorizationEnabled( bool enabled ) {

	mCategorizationEnabled = enabled;
	sort( prevSort.column, prevSort.cs, enabled );
};

int PBItemViewModel::rowCount( const QModelIndex & parent ) const {

	if ( parent.column() > 0 )
		return 0;

	if ( not parent.isValid() )
		return rootNode->childCount();

	else
		return static_cast<PBItemViewNode*>( parent.internalPointer() )->childCount();
};

int PBItemViewModel::rowCount( QString mCategory ) const {

	if ( not rootNode->categoryList().contains( mCategory ) or mCategory.isEmpty() or mCategory.isNull() )
		return 0;

	return categoryRowMap[ mCategory ].count();
};

int PBItemViewModel::categoryCount() const {

	return rootNode->categoryCount();
};

int PBItemViewModel::columnCount( const QModelIndex & parent ) const {

	Q_UNUSED( parent );
	return 7;
};

Qt::ItemFlags PBItemViewModel::flags( const QModelIndex & index ) const {

	if ( not index.isValid() )
		return Qt::NoItemFlags;

	if ( isDir( nodePath( index ) ) )
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

	else
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
};

QVariant PBItemViewModel::data( const QModelIndex &index, int role ) const {

	if ( not index.isValid() )
		return QVariant();

	PBItemViewNode *node = static_cast<PBItemViewNode*>( index.internalPointer() );
	switch( role ) {

		case Qt::DisplayRole: {
			if ( ( index.column() >= 0 ) and ( index.column() <= 9  ) )
				return node->data( index.column() );

			else
				return QVariant();
		}

		case Qt::DecorationRole: {
			if ( index.column() == 0 ) {

				/* Icon stored in the node */
				return node->icon();
			}

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
			return node->data( index.column() );
		}

		case Qt::ToolTipRole: {
			switch( index.column() ) {
				case 0:	{
					if ( mModelDataType == PBItemViewModel::Applications )
						return QString( "<center>%1<br>%2</center>" ).arg( node->data( 0 ).toString() ).arg( node->data( 2 ).toString() );

					else
						return QString( "<center>%1<br>%2</center>" ).arg( node->data( 0 ).toString() ).arg( node->data( 1 ).toString() );

				}
				default: {

					return QString( "<b>%1</b>" ).arg( node->data( index.column() ).toString() );
				}
			}
		}

		case Qt::UserRole + 0: {
			return node->data( 0 );
		}

		case Qt::UserRole + 1: {
			return node->data( 1 );
		}

		case Qt::UserRole + 2: {
			return node->data( 2 );
		}

		case Qt::UserRole + 3: {
			return node->data( 3 );
		}

		case Qt::UserRole + 4: {
			return node->data( 4 );
		}

		case Qt::UserRole + 5: {
			return node->data( 5 );
		}

		case Qt::UserRole + 6: {
			return node->data( 6 );
		}

		case Qt::UserRole + 7: {
			return node->data( 7 );
		}

		case Qt::UserRole + 8: {
			return node->data( 8 );
		}

		case Qt::UserRole + 9: {
			return node->data( 9 );
		}

		default: {
			return QVariant();
		}
	}
};

QVariant PBItemViewModel::headerData( int section, Qt::Orientation orientation, int role ) const {

	QVariantList headerList = QVariantList() << "Name" << "Size" << "Type" << "MimeType" << "Modified" << "Permissions" << "Owner";

	if ( ( orientation == Qt::Horizontal ) and ( role == Qt::DisplayRole ) )
		return headerList.at( section );

	else if ( ( orientation == Qt::Horizontal ) and ( role == Qt::TextAlignmentRole ) )
		if ( section == 0 )
			return ( 0x0001 | 0x0080 );

		else if ( section == 1 )
			return ( 0x0002 | 0x0080 );

		else
			return Qt::AlignCenter;

	else
		return QVariant();
};

bool PBItemViewModel::setData( const QModelIndex &index, QVariant value, int role ) {

	bool ok = rootNode->child( index.row() )->setData( role, value );
	if ( ok )
		emit dataChanged( index, index );

	return ok;
};

bool PBItemViewModel::isRealLocation() const {

	return not mVirtualData;
};

quint64 PBItemViewModel::modelDataType() const {

	return mModelDataType;
};

bool PBItemViewModel::insertNode( QString nodeName ) {

	/*
		*
		* Quick Fix: Append the newly added file at the end of the model.
		* Then sort the model.
		*
	*/

	if ( not exists( mRootPath + nodeName ) )
		return false;

	beginResetModel();
	beginInsertRows( parent(), mChildNames.count(), mChildNames.count() );

	QVariantList data = quickDataGatherer->getQuickFileInfo( mRootPath + nodeName );

	if ( Settings->General.ShowHidden ) {
		if ( mNameFilters.count() ) {
			if ( matchesFilter( mNameFilters, nodeName ) ) {
				rootNode->addChild( new PBItemViewNode( data, getCategory( data ), rootNode ) );
				mChildNames << nodeName;
			}
		}
		else {
			rootNode->addChild( new PBItemViewNode( data, getCategory( data ), rootNode ) );
			mChildNames << nodeName;
		}
	}
	else {
		if ( not nodeName.startsWith( "." ) ) {
			if ( mNameFilters.count() ) {
				if ( matchesFilter( mNameFilters, nodeName ) ) {
					rootNode->addChild( new PBItemViewNode( data, getCategory( data ), rootNode ) );
					mChildNames << nodeName;
				}
			}
			else {
				rootNode->addChild( new PBItemViewNode( data, getCategory( data ), rootNode ) );
				mChildNames << nodeName;
			}
		}
	}
	endInsertRows();

	sort( prevSort.column, prevSort.cs, prevSort.categorized );
	endResetModel();

	return true;
};

void PBItemViewModel::updateNode( QString nodeName ) {

	QMutexLocker locker( &mutex );
	if ( not exists( mRootPath + nodeName ) )
		return;

	if ( not mChildNames.contains( nodeName ) )
		return;

	PBItemViewNode *node = rootNode->child( nodeName );
	if ( isDir( mRootPath + nodeName ) ) {
		node->setData( 1, nChildren( mRootPath + nodeName ), true );
		node->setData( 1, QString( "%1 items" ).arg( node->data( 1, true ).toLongLong() ), false );
	}

	else {
		node->setData( 1, getSize( mRootPath + nodeName ), true );
		node->setData( 1, formatSize( node->data( 1, true ).toLongLong() ), false );
	}

	sort( prevSort.column, prevSort.cs, prevSort.categorized );
};

bool PBItemViewModel::removeNode( QString nodeName ) {

	if ( not mChildNames.contains( nodeName ) )
		return false;

	mChildNames.removeAll( nodeName );
	PBItemViewNode *removedNode = rootNode->child( nodeName );

	beginResetModel();
	beginRemoveRows( parent(), removedNode->row(), removedNode->row() );

	rootNode->removeChild( removedNode );

	endRemoveRows();
	endResetModel();

	if ( mCategorizationEnabled )
		recategorize();

	sort( prevSort.column, prevSort.cs, prevSort.categorized );

	return false;
};

void PBItemViewModel::nodeUpdated( QString nodeName ) {

	PBItemViewNode *node = rootNode->child( baseName( nodeName ) );
	node->updateIcon();

	emit dataChanged( index( nodeName ), index( nodeName ) );
};

QModelIndex PBItemViewModel::index( int row, int column, const QModelIndex &parent ) const {

    if (row < 0 || column < 0)
        return QModelIndex();

    if ( not ( ( row < rowCount( parent ) ) and ( column < columnCount( parent ) ) ) )
		return QModelIndex();

	PBItemViewNode *parentNode;
	if ( not parent.isValid() )
		parentNode = rootNode;

	else
		parentNode = (PBItemViewNode *)parent.internalPointer();

	PBItemViewNode *childNode = parentNode->child( row );
	if ( childNode )
		return createIndex( row, column, childNode );

	else
		return QModelIndex();
};

QModelIndex PBItemViewModel::index( QString name, const QModelIndex &parent ) const {

	if ( not mChildNames.contains( name ) )
		return QModelIndex();

	PBItemViewNode *parentNode;
	if ( not parent.isValid() )
		parentNode = rootNode;

	else
		parentNode = (PBItemViewNode *)parent.internalPointer();

	int row = parentNode->child( name )->row();
	PBItemViewNode *childNode = parentNode->child( name );

	if ( childNode )
		return createIndex( row, 0, childNode );

	return QModelIndex();
};

QModelIndex PBItemViewModel::parent( const QModelIndex &index ) const {

	if ( not index.isValid() )
		return QModelIndex();

	PBItemViewNode *childNode = ( PBItemViewNode * )index.internalPointer();
	PBItemViewNode *parentNode = childNode->parent();

	if ( parentNode == rootNode )
		return QModelIndex();

	return createIndex( parentNode->row(), 0, parentNode );
};

QString PBItemViewModel::category( const QModelIndex &index ) const {

	if ( not index.isValid() )
		return QString( "" );

	PBItemViewNode *childItem = ( PBItemViewNode * )index.internalPointer();
	return childItem->category();
};

int PBItemViewModel::categoryIndex( const QModelIndex &index ) const {

	if ( not index.isValid() )
		return -1;

	PBItemViewNode *childItem = ( PBItemViewNode * )index.internalPointer();
	return childItem->categoryIndex();
};

QStringList PBItemViewModel::categories() const {

	return rootNode->categoryList();
};

QPixmap PBItemViewModel::pixmapForCategory( QString categoryName, bool folded ) const {

	switch ( mModelDataType ) {

		case PBItemViewModel::Applications: {

			if ( categoryName == "Accessories" )
				return QIcon::fromTheme( "applications-accessories" ).pixmap( 24, 24 );

			else if ( categoryName == "Development" )
				return QIcon::fromTheme( "applications-development" ).pixmap( 24, 24 );

			else if ( categoryName == "Education" )
				return QIcon::fromTheme( "applications-education" ).pixmap( 24, 24 );

			else if ( categoryName == "Games" )
				return QIcon::fromTheme( "applications-games" ).pixmap( 24, 24 );

			else if ( categoryName == "Graphics" )
				return QIcon::fromTheme( "applications-graphics" ).pixmap( 24, 24 );

			else if ( categoryName == "Internet" )
				return QIcon::fromTheme( "applications-internet" ).pixmap( 24, 24 );

			else if ( categoryName == "Multimedia" )
				return QIcon::fromTheme( "applications-multimedia" ).pixmap( 24, 24 );

			else if ( categoryName == "Office" )
				return QIcon::fromTheme( "applications-office" ).pixmap( 24, 24 );

			else if ( categoryName == "Science and Math" )
				return QIcon::fromTheme( "applications-science" ).pixmap( 24, 24 );

			else if ( categoryName == "Settings" )
				return QIcon::fromTheme( "preferences-system" ).pixmap( 24, 24 );

			else if ( categoryName == "System" )
				return QIcon::fromTheme( "applications-system" ).pixmap( 24, 24 );

			else
				return QIcon::fromTheme( "preferences-other" ).pixmap( 24, 24 );
		}

		case PBItemViewModel::Catalogs: {

			if ( categoryName == "Documents" )
				return QIcon( ":/icons/documents.png" ).pixmap( 16, 16 );

			else if ( categoryName == "Music" )
				return QIcon::fromTheme( "folder-sound" ).pixmap( 16, 16 );

			else if ( categoryName == "Pictures" )
				return QIcon::fromTheme( "folder-image" ).pixmap( 16, 16 );

			else if ( categoryName == "Videos" )
				return QIcon::fromTheme( "folder-video" ).pixmap( 16, 16 );

			else
				return QIcon( ":/icons/catalogs-alt.png" ).pixmap( 16, 16 );
		}

		case PBItemViewModel::SuperStart: {
			if ( categoryName == "Places  " )
				return QIcon( ":/icons/folder.png" ).pixmap( 16, 16 );

			else if ( categoryName == "Computer  " )
				return QIcon( ":/icons/comp.png" ).pixmap( 16, 16 );

			else
				return QIcon( ":/icons/files.png" ).pixmap( 16, 16 );
		}

		default: {
			QIcon icon = QIcon::fromTheme( mCategoryIconMap[ categoryName ] );
			if ( icon.isNull() ) {
				if ( folded )
					return QIcon::fromTheme( "arrow-right" ).pixmap( 16, 16 );

				else
					return QIcon::fromTheme( "arrow-down" ).pixmap( 16, 16 );
			}
			else {

				return icon.pixmap( 16, 16 );
			}
		}
	}
};

int PBItemViewModel::indexListCountForCategory( QString mCategory ) const {

	if ( not rootNode->categoryList().contains( mCategory ) or mCategory.isEmpty() or mCategory.isNull() )
		return 0;

	return categoryRowMap[ mCategory ].count();
};

QModelIndexList PBItemViewModel::indexListForCategory( QString mCategory ) const {

	QModelIndexList mList;

	if ( not rootNode->categoryList().contains( mCategory ) or mCategory.isEmpty() or mCategory.isNull() )
		return mList;

	foreach( int row, categoryRowMap[ mCategory ] ) {
		mList << index( row, 0, QModelIndex() );
	}

	return mList;
};

int PBItemViewModel::indexListCountForCategory( int mCategoryIdx ) const {

	return indexListCountForCategory( rootNode->categoryList().at( mCategoryIdx ) );
};

QModelIndexList PBItemViewModel::indexListForCategory( int mCategoryIdx ) const {

	return indexListForCategory( rootNode->categoryList().at( mCategoryIdx ) );
};

QModelIndexList PBItemViewModel::categorySiblings( QModelIndex idx ) const {

	if ( not idx.isValid() )
		return QModelIndexList();

	QModelIndexList mList = indexListForCategory( category( idx ) );
	mList.removeAll( idx );

	return mList;
};

int PBItemViewModel::indexInCategory( QModelIndex idx ) const {

	return indexListForCategory( category( idx ) ).indexOf( idx );
};

Qt::DropActions PBItemViewModel::supportedDragActions() const {

	return Qt::CopyAction | Qt::MoveAction | Qt::LinkAction;
};

Qt::DropActions PBItemViewModel::supportedDropActions() const {

	return Qt::CopyAction | Qt::MoveAction | Qt::LinkAction;
};

Qt::ItemFlags PBItemViewModel::flags( const QModelIndex index ) const {

	if ( isReadable( nodePath( index ) ) )
		return Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEnabled;

	else
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
};

QStringList PBItemViewModel::mimeTypes() const {

	QStringList types;
	types << "text/uri-list";
	return types;
};

bool PBItemViewModel::filter( Filters filter ) {

	Q_UNUSED( filter );

	return false;
};

void PBItemViewModel::setFilter( Filters filter, bool on ) {

	Q_UNUSED( filter );
	Q_UNUSED( on );

	return;
};

QStringList PBItemViewModel::nameFilters() {

	return mNameFilters;
};

void PBItemViewModel::setNameFilters( QStringList filters ) {

	mNameFilters.clear();
	mNameFilters << filters;

	setupModelData();
};

void PBItemViewModel::clearNameFilters() {

	mNameFilters.clear();
	setupModelData();
};

void PBItemViewModel::sort( int column, bool cs, bool categorized ) {

	prevSort.column = column;
	prevSort.cs = cs;
	prevSort.categorized = categorized;

	recategorize();
	rootNode->sort( column, cs, false );

	// Create a map of categoryIndex versus rows
	categoryRowMap.clear();
	foreach( PBItemViewNode *item, rootNode->children() )
		categoryRowMap[ item->category() ] << item->row();

	emit layoutChanged();
};

void PBItemViewModel::reload() {

	setupModelData();
};

bool PBItemViewModel::rename( QString oldName, QString newName ) {

	// Do nothing: This should never happen
	if ( mVirtualData )
		return true;

	/* If the file @oldName is not from the current directory */
	if ( dirName( oldName ) != mRootPath ) {
		insertNode( baseName( newName ) );
		return true;
	}

	/* If the file @newName is not in the current dir */
	else if ( dirName( newName ) != mRootPath ) {
		removeNode( baseName( oldName ) );
		return true;
	}

	else {
		/* Same folder */
		removeNode( baseName( oldName ) );
		insertNode( baseName( newName ) );

		return true;
	}

	/* This can never happen! */
	return false;
};

QString PBItemViewModel::nodeName( const QModelIndex idx ) const {

	return idx.data( 0 ).toString();
};

QString PBItemViewModel::nodePath( const QModelIndex idx ) const {

	switch ( mModelDataType ) {

		case PBItemViewModel::Applications: {

			return idx.data( Qt::UserRole + 9 ).toString();
		}

		case PBItemViewModel::SuperStart: {

			QString path = idx.data( Qt::UserRole + 2 ).toString();
			if ( exists( path ) )
				return path;

			else
				return idx.data( Qt::UserRole + 7 ).toString();
		}

		case PBItemViewModel::Catalogs:
		case PBItemViewModel::FileSystem:
			return idx.data( Qt::UserRole + 7 ).toString();
	}
};

QString PBItemViewModel::nodePath( const QString path ) const {

	return nodePath( index( path ) );
};

QFileInfo PBItemViewModel::nodeInfo( const QModelIndex idx ) const {

	switch ( mModelDataType ) {

		case PBItemViewModel::Applications: {

			return QFileInfo( idx.data( Qt::UserRole + 9 ).toString() );
		}

		case PBItemViewModel::SuperStart: {

			QString path = idx.data( Qt::UserRole + 2 ).toString();
			if ( exists( path ) )
				return QFileInfo( path );

			else
				return QFileInfo( idx.data( Qt::UserRole + 7 ).toString() );
		}

		case PBItemViewModel::Catalogs:
		case PBItemViewModel::FileSystem:
			return QFileInfo( idx.data( Qt::UserRole + 7 ).toString() );
	}
};

QString PBItemViewModel::rootPath() const {

	return mRootPath;
};

void PBItemViewModel::setRootPath( QString path ) {

	if ( path.startsWith( "/" ) )
		prevFolder = path;

	mRootPath = path;

	if ( path.startsWith( "PB://SuperStart" ) ) {
		mVirtualData = true;
		mModelDataType = (quint64)PBItemViewModel::SuperStart;
	}

	else if ( path.startsWith( "PB://Applications" ) ) {
		mVirtualData = true;
		mModelDataType = (quint64)PBItemViewModel::Applications;
	}

	else if ( path.startsWith( "PB://Catalogs" ) ) {
		mVirtualData = true;
		mModelDataType = (quint64)PBItemViewModel::Catalogs;
	}

	else if ( path.startsWith( "PB://Folders" ) ) {
		mVirtualData = false;
		mModelDataType = (quint64)PBItemViewModel::FileSystem;
		mRootPath = prevFolder;
	}

	else {
		mVirtualData = false;
		mModelDataType = (quint64)PBItemViewModel::FileSystem;

		mRootPath = ( path.endsWith( "/" ) ? path : path + "/" );
	}

	/* Navigation: If we are in the middle, remove all 'forawrd' roots */
	if ( oldRoots.count() )
		oldRoots.erase( oldRoots.begin() + curIndex + 1, oldRoots.end() );

	/* Append this root to navigation list */
	oldRoots << mRootPath;
	curIndex = oldRoots.count() - 1;

	/* chdir( mRootPath ) to set the correct link to /pwd/<pid>/cwd */
	chdir( mRootPath.toLocal8Bit().constData() );

	/* We have set per-folder settings */
	QSettings sett( mRootPath + ".directory", QSettings::NativeFormat );

	/* Check per folder view settings */
	// Settings->General.ShowHidden = sett.value( "PiBreeze/Hidden", false ).toBool();

	prevSort.column = sett.value( "PiBreeze/SortColumn", Settings->General.SortColumn ).toInt();
	prevSort.cs = sett.value( "PiBreeze/SortCase", Settings->General.SortCase ).toBool();
	prevSort.categorized = sett.value( "PiBreeze/Grouping", Settings->General.Grouping ).toBool();
	mCategorizationEnabled = prevSort.categorized;

	/* Delete the old node */
	delete rootNode;

	/* Create a fresh root node */
	rootNode = new PBItemViewNode( quickDataGatherer->getQuickFileInfo( path ), "" );

	/* Stop loading thumbs, or even nodes */
	if ( currentLoadStatus.loading )
		currentLoadStatus.stopLoading = true;

	/* If the root path is /dev/, then stop the watcher */
	if ( mRootPath.startsWith( "/dev/" ) )
		watcher->stopWatch();

	/* For all the other folders, we can happily start the watcher */
	else
		watcher->startWatch( path );

	setupModelData();
};

void PBItemViewModel::goBack() {

	if ( canGoBack() ) {
		curIndex--;

		mRootPath = oldRoots.at( curIndex );

		if ( mRootPath.startsWith( "PB://SuperStart" ) ) {
			mVirtualData = true;
			mModelDataType = (quint64)PBItemViewModel::SuperStart;
		}

		else if ( mRootPath.startsWith( "PB://Applications" ) ) {
			mVirtualData = true;
			mModelDataType = (quint64)PBItemViewModel::Applications;
		}

		else if ( mRootPath.startsWith( "PB://Catalogs" ) ) {
			mVirtualData = true;
			mModelDataType = (quint64)PBItemViewModel::Catalogs;
		}

		else {
			mVirtualData = false;
			mModelDataType = (quint64)PBItemViewModel::FileSystem;
		}

		delete rootNode;
		rootNode = new PBItemViewNode( quickDataGatherer->getQuickFileInfo( mRootPath ), "" );

		setupModelData();
	}
};

void PBItemViewModel::goForward() {

	if ( canGoForward() ) {
		curIndex++;

		mRootPath = oldRoots.at( curIndex );

		if ( mRootPath.startsWith( "PB://SuperStart" ) ) {
			mVirtualData = true;
			mModelDataType = (quint64)PBItemViewModel::SuperStart;
		}

		else if ( mRootPath.startsWith( "PB://Applications" ) ) {
			mVirtualData = true;
			mModelDataType = (quint64)PBItemViewModel::Applications;
		}

		else if ( mRootPath.startsWith( "PB://Catalogs" ) ) {
			mVirtualData = true;
			mModelDataType = (quint64)PBItemViewModel::Catalogs;
		}

		else {
			mVirtualData = false;
			mModelDataType = (quint64)PBItemViewModel::FileSystem;
		}

		delete rootNode;
		rootNode = new PBItemViewNode( quickDataGatherer->getQuickFileInfo( mRootPath ), "" );

		setupModelData();
	}
};

void PBItemViewModel::goUp() {

	if ( canGoUp() ) {
		QString newRoot = mRootPath.section( "/", 0, -3 );
		newRoot = ( newRoot.endsWith( "/" ) ? newRoot : newRoot + "/" );
		setRootPath( newRoot );
	}
};

void PBItemViewModel::goHome() {

	if ( Settings->General.SpecialOpen and Settings->General.SuperStart )
		setRootPath( "PB://SuperStart" );

	else if ( Settings->General.SpecialOpen and Settings->General.OpenWithCatalog )
		setRootPath( "PB://Catalogs" );

	else
		setRootPath( QDir::homePath() );
};

bool PBItemViewModel::canGoBack() const {

	return ( curIndex > 0 );
};

bool PBItemViewModel::canGoForward() const {

	return ( curIndex < ( oldRoots.count() - 1 ) ? true : false);
};

bool PBItemViewModel::canGoUp() const {

	if ( oldRoots.at( curIndex ).startsWith( "PB://" ) )
		return false;

	return ( mRootPath != "/" );
};

QString PBItemViewModel::previousDir() const {

	if ( canGoBack() )
		return oldRoots.at( curIndex - 1 );

	else
		return QString( "" );
};

QString PBItemViewModel::nextDir() const {

	if ( canGoForward() )
		return oldRoots.at( curIndex + 1 );

	else
		return QString( "" );
};

QString PBItemViewModel::currentDir() const {

	return mRootPath;
};

QString PBItemViewModel::parentDir() const {

	QString path = mRootPath.section( "/", 0, -3 );
	return ( path.endsWith( "/" ) ? path : path + "/" );
};

QString PBItemViewModel::lastOpenedFolder() const {

	return prevFolder;
};

void PBItemViewModel::setupModelData() {

	switch( mModelDataType ) {
		case PBItemViewModel::SuperStart: {
			setupSuperStartData();
			return;
		}

		case PBItemViewModel::Applications: {
			setupApplicationsData();
			return;
		}

		case PBItemViewModel::Catalogs: {
			setupCatalogData();
			return;
		}

		case PBItemViewModel::FileSystem: {
			setupFileSystemData();
			return;
		}
	}
};

void PBItemViewModel::setupFileSystemData() {

	mChildNames.clear();
	rootNode->clearChildren();
	currentLoadStatus.loading = true;

	/* These are useful only when we do not refresh or change into some directory. */
	lastUpdatedNodes.clear();
	lastUpdatedTimes.clear();
	delayedUpdateList.clear();

	emit directoryLoading( mRootPath );

	beginResetModel();

	/* Get file list */
	struct dirent **fileList;
	lambdaUseFilterList << mNameFilters;
	int numFiles = scandir( mRootPath.toStdString().c_str(), &fileList, scandirCallback, NULL );
	lambdaUseFilterList.clear();

	/* Add the files to the model */
	if ( numFiles >= 0 ) {
		for( int i = 0; i < numFiles; i++ ) {
			QString _nodeName = QString::fromLocal8Bit( fileList[ i ]->d_name );
			QVariantList data = quickDataGatherer->getQuickFileInfo( mRootPath + _nodeName );
			QString _category = getCategory( data );
			rootNode->addChild( new PBItemViewNode( data, _category, rootNode ) );
			mChildNames << _nodeName;
			mCategoryIconMap[ _category ] = data.at( 2 ).toString();
			free( fileList[ i ] );
		}
		free( fileList );
	}

	endResetModel();

	/* We make all the categories visible by default, except the saved ones */
	QSettings dirSett( mRootPath + ".directory", QSettings::NativeFormat );

	/* Sort the contents */
	sort( prevSort.column, prevSort.cs, prevSort.categorized );

	/* Switch off the loading flag */
	currentLoadStatus.loading = false;

	/* Emit directory loaded */
	emit directoryLoaded( mRootPath );
};

void PBItemViewModel::setupSuperStartData() {

	mChildNames.clear();
	rootNode->clearChildren();
	currentLoadStatus.loading = true;

	emit directoryLoading( mRootPath );

	QSettings superStart( "PiBreeze", "SuperStart" );

	beginResetModel();

	Q_FOREACH( PBDeviceInfo info, PBDeviceManager::allMounts() ) {
		QVariantList data;
		data << "SuperStart" << 0 << ":/icons/" + info.deviceType() + ".png";
		data << info.displayName();
		data << QString( "%1% used" ).arg( info.bytesUsed() * 100 / info.bytesTotal() );
		data << info.mountPoint();

		rootNode->addChild( new PBItemViewNode( data, "Computer  ", rootNode ) );
		mChildNames << info.displayName();
	};

	/* Bookmarked Folders */
	superStart.beginGroup( "Places" );
	foreach( QString key, superStart.childKeys() ) {
		QString location = superStart.value( key ).toString();
		if ( not exists( location ) )
			continue;

		QVariantList data = quickDataGatherer->getQuickFileInfo( location );
		data[ 0 ] = "dirss";				// dirss Needed for getting the right icon
		data[ 3 ] = key;
		rootNode->addChild( new PBItemViewNode( data, "Places  ", rootNode ) );
		mChildNames << key;
	}
	superStart.endGroup();

	/* Recent Applications */
	superStart.beginGroup( "Files" );
	foreach( QString key, superStart.childKeys() ) {
		QString path = superStart.value( key ).toString();
		QVariantList data = quickDataGatherer->getQuickFileInfo( path );
		data[ 3 ] = key;
		rootNode->addChild( new PBItemViewNode( data, "Files  ", rootNode ) );
		mChildNames << key;
	}
	superStart.endGroup();

	endResetModel();

	sort( prevSort.column, prevSort.cs, prevSort.categorized );

	currentLoadStatus.loading = false;

	emit directoryLoaded( mRootPath );
};

void PBItemViewModel::setupApplicationsData() {

	mChildNames.clear();
	rootNode->clearChildren();
	currentLoadStatus.loading = true;

	emit directoryLoading( mRootPath );

	beginResetModel();
	PBAppEngine *appEngine = PBAppEngine::instance();
	foreach( PBAppFile app, appEngine->allDesktops().toQList() ) {
		if ( ( app.value( PBAppFile::Type ) != "Application" ) or ( app.value( PBAppFile::NoDisplay ).toBool() ) )
			continue;

		QVariantList data;

		/* Special Data */
		data << "Application" << 0 << app.value( PBAppFile::Icon );

		/* Normal Data */
		data << app.value( PBAppFile::Name );												/* Qt::UserRole + 0 */
		data << app.value( PBAppFile::Exec );												/* Qt::UserRole + 1 */
		data << app.value( PBAppFile::Comment );											/* Qt::UserRole + 2 */
		data << app.execArgs();																/* Qt::UserRole + 3 */
		data << app.value( PBAppFile::Icon );												/* Qt::UserRole + 4 */
		data << app.value( PBAppFile::WorkPath );											/* Qt::UserRole + 5 */
		data << app.value( PBAppFile::MimeTypes );											/* Qt::UserRole + 6 */
		data << app.value( PBAppFile::TerminalMode );										/* Qt::UserRole + 7 */
		data << app.value( PBAppFile::Categories );											/* Qt::UserRole + 8 */
		data << app.filePath();																/* Qt::UserRole + 9 */

		rootNode->addChild( new PBItemViewNode( data, app.category(), rootNode ) );
	}
	endResetModel();

	sort( prevSort.column, prevSort.cs, prevSort.categorized );

	currentLoadStatus.loading = false;

	emit directoryLoaded( mRootPath );
};

void PBItemViewModel::setupCatalogData() {

	mChildNames.clear();
	rootNode->clearChildren();
	currentLoadStatus.loading = true;

	QStringList dirs;

	emit directoryLoading( mRootPath );

	QSettings catalogsSettings( "PiBreeze", "Catalogs" );
	/* Default Catalogs */
	beginResetModel();
	foreach( QString ctlg, catalogsSettings.childKeys() ) {
		QStringList locations = catalogsSettings.value( ctlg ).toStringList();
		foreach( QString location, locations ) {
			if ( not exists( location ) )
				continue;

			QVariantList data = quickDataGatherer->getQuickFileInfo( location );
			rootNode->addChild( new PBItemViewNode( data, ctlg, rootNode ) );
			mChildNames << data.at( 3 ).toString();
			dirs << location;
		}
	}

	/* Custom Catalogs */
	catalogsSettings.beginGroup( "Custom" );
	foreach( QString ctlg, catalogsSettings.childKeys() ) {
		QStringList locations = catalogsSettings.value( ctlg ).toStringList();
		foreach( QString location, locations ) {
			if ( not exists( location ) )
				continue;

			QVariantList data = quickDataGatherer->getQuickFileInfo( location );
			rootNode->addChild( new PBItemViewNode( data, ctlg, rootNode ) );
			mChildNames << data.at( 3 ).toString();
			dirs << location;
		}
	}
	catalogsSettings.endGroup();
	endResetModel();

	sort( prevSort.column, prevSort.cs, prevSort.categorized );

	currentLoadStatus.loading = false;

	emit directoryLoaded( mRootPath );
};

void PBItemViewModel::newWatch( QString path ) {

	/* Path is not empty and exists, start the watch */
	if ( path.count() and exists( path ) )
		watcher->startWatch( path );
};

QString PBItemViewModel::getCategory( QVariantList data ) {

	switch( prevSort.column ) {
		/* Name Sorting */
		case 0: {
			if ( data.at( 0 ).toString() == "dir" )
				return data.at( 3 ).toString().toUpper().at( 0 );

			else
				return data.at( 3 ).toString().toUpper().at( 0 ) + QString( " " );
		}

		/* Size Sorting */
		/* Separate directories from files */
		case 1: {
			/* Folders */
			if ( data.at( 0 ).toString() == "dir" )
				return "Folders";
			/* 0 - 100 kB */
			if ( data.at( 1 ).toLongLong() < 102400 )
				return "Tiny";
			/* 100 kB - 10 MB */
			if ( data.at( 1 ).toLongLong() < 10485760 )
				return "Small";
			/* 10 MB - 100 MB */
			if ( data.at( 1 ).toLongLong() < 104857600 )
				return "Medium";
			/* 100 MB - 1 GB */
			if ( data.at( 1 ).toLongLong() < 1073741824 )
				return "Large";
			/* 0 - 100 kB */
			if ( data.at( 1 ).toLongLong() >= 1073741824 )
				return "Massive";
		}

		/* Type Sorting */
		case 2: {
			if ( data.at( 0 ).toString() == "dir" ) {
				return "Folders";
			}

			if ( data.at( 0 ).toString() == "file" ) {
				return data.at( 5 ).toString();
			}

			if ( data.at( 0 ).toString() == "system" ) {
				return "System Files";
			}
		}

		case 4: {
			QDate date = QDate::fromString( data.at( 7 ).toString(), "ddd, dd MMM, yyyy" );
			if ( date.isValid() ) {
				if ( date == QDate::currentDate() )
					return "Today";

				else if ( date.weekNumber() == QDate::currentDate().weekNumber() )
					return "This Week";

				else if ( date.weekNumber() == QDate::currentDate().weekNumber() - 1 )
					return "Last Week";

				else if ( date.month() == QDate::currentDate().month() )
					return "This Month";

				else if ( date.month()== QDate::currentDate().month() - 1 )
					return "Last Month";

				else
					return date.toString( "MMMM yyyy" );
			}

			return "Uncategorized";
		}

		default: {
			return QString( "Uncategorized" );
		}
	}

	return QString( "Uncategorized" );
};

void PBItemViewModel::recategorize() {

	foreach( PBItemViewNode *node, rootNode->children() )
		if ( not mVirtualData )
			node->setCategory( getCategory( node->allData() ) );

	rootNode->updateCategories();
};

void PBItemViewModel::handleNodeCreated( QString node ) {

	if ( baseName( node ).startsWith( "." ) and not Settings->General.ShowHidden )
		return;

	if ( dirName( node ) == currentDir() )
		insertNode( baseName( node ) );
};

/*
	*
	* We have updated the three function to ignore rapid inotify events triggered
	* due to file copy. When a file is copied, it is rapidly changed. If an event is
	* triggered for the same node in succession, then we quarantine it for a second.
	*
*/
void PBItemViewModel::handleNodeChanged( QString node ) {

	if ( baseName( node ).startsWith( "." ) )
		return;

	if ( dirName( node ) == currentDir() ) {
		qint64 idx = lastUpdatedNodes.indexOf( node );
		if ( idx == -1 ) {
			updateNode( baseName( node ) );
			lastUpdatedNodes << node;
			lastUpdatedTimes << QTime::currentTime();
		}

		/* To handle copying or moving, or any continually changing node */
		else {
			/* Past update was more than ten second ago */
			if ( lastUpdatedTimes.at( idx ).secsTo( QTime::currentTime() ) > 10000 ) {
				lastUpdatedTimes.replace( idx, QTime::currentTime() );
				updateNode( baseName( node ) );
			}
		}
	}
};

void PBItemViewModel::handleNodeDeleted( QString node ) {

	if ( baseName( node ).startsWith( "." ) and not Settings->General.ShowHidden )
		return;

	if ( dirName( node ) == currentDir() )
		removeNode( baseName( node ) );
};

void PBItemViewModel::handleNodeRenamed( QString oldNode, QString newNode ) {

	rename( oldNode, newNode );
};

void PBItemViewModel::loadHome() {

	emit runningHome( currentDir() );
	goHome();
};
