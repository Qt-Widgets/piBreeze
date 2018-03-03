/*
	*
	* PBItemViewNode.cpp - PiBreeze PBItemViewNode Class
	*
*/

#include "PBItemViewNode.hpp"
#include "PBIconManager.hpp"

inline static QIcon getIcon( QString name, QString type ) {

	QIcon icon;
	if ( type.isEmpty() ) {
		if ( exists( name ) )
			icon.addFile( name );

		else {
			Q_FOREACH( QString icoStr, PBIconManager::instance()->icon( name ) ) {
				QRegExp rx( "[0-9]+" );
				rx.indexIn( icoStr );
				int num = rx.cap( 0 ).toInt();
				icon.addFile( icoStr, QSize( num, num ) );
			}
		}
	}

	else {
		char *newPath[ PATH_MAX ] = { 0 };

		QString path = QString::fromLocal8Bit( realpath( name.toLocal8Bit().constData(), NULL ) );
		Q_FOREACH( QString icoStr, PBIconManager::instance()->iconsForFile( type, path ) ) {
			QRegExp rx( "[0-9]+" );
			rx.indexIn( icoStr );
			int num = rx.cap( 0 ).toInt();
			icon.addFile( icoStr, QSize( num, num ) );
		}
	}

	return icon;
};

PBItemViewNode::PBItemViewNode() {

	for( int i = 0; i < 10; i++ )
		nodeData << "";

	mIcon = QIcon( ":/icons/unknown.png" );

	mCategoryList.clear();
	myCategory = "Uncategorized";
	parentNode = 0;
};

PBItemViewNode::PBItemViewNode( QVariantList data, QString category, PBItemViewNode *parent ) {

	nodeData << data;
	myCategory = category;

	if ( data.at( 0 ).toString() == "Application" )
		mIcon = getIcon( data.at( 7 ).toString(), QString() );

	else if ( data.at( 0 ).toString() == "SuperStart" )
		mIcon = QIcon( data.at( 2 ).toString() );

	else if ( data.at( 0 ).toString() == "dirss" )
		mIcon = getIcon( data.at( 10 ).toString(), QString( "inode/directory" ) );

	else
		mIcon = getIcon( data.at( 3 ).toString(), data.at( 6 ).toString() );

	parentNode = parent;
};

PBItemViewNode::~PBItemViewNode() {

	// delete &future;
};

void PBItemViewNode::addChild( PBItemViewNode *node ) {

	if ( not mCategoryList.contains( node->category() ) )
		mCategoryList << node->category();

	childNodes << node;
};

void PBItemViewNode::removeChild( PBItemViewNode *node ) {

	delete childNodes.takeAt( node->row() );
};

PBItemViewNode* PBItemViewNode::child( int row ) {

	return childNodes.at( row );
};

PBItemViewNode* PBItemViewNode::child( QString name ) {

	foreach( PBItemViewNode* node, childNodes )
		if ( node->data( 0 ) == name )
			return node;

	return new PBItemViewNode();
};

QList<PBItemViewNode*> PBItemViewNode::children() {

	return childNodes;
};

int PBItemViewNode::childCount() {

	return childNodes.count();
};

int PBItemViewNode::categoryCount() {

	return mCategoryList.count();
};

void PBItemViewNode::clearChildren() {

	childNodes.clear();
	mCategoryList.clear();
};

QIcon PBItemViewNode::icon() {

	return mIcon;
};

QString PBItemViewNode::category() {

	return myCategory;
};

void PBItemViewNode::setCategory( QString newCategory ) {

	myCategory = newCategory;
};

int PBItemViewNode::categoryIndex() {

	return parentNode->mCategoryList.indexOf( myCategory );
};

QStringList PBItemViewNode::categoryList() {

	return mCategoryList;
};

QVariant PBItemViewNode::data( int column, bool special ) const {

	/*
		*
		* If we want special data, i.e., we can choose column 0, 1, or 2
		* If we want normal data, then we can choose columns 0 to 6, internally 3 onwards
		*
		* [
		* 	dir/file/system			0
		*   rawsize					1
		*   iconstr					2
		*   name = 0				3
		*   size = 1				4
		*   type = 2				5
		*   mime = 3				6
		*   time = 4				7
		*   perm = 5				8
		*   ownr = 6				9
		*   mtpt = 7				10
		* ]
		*
	*/

	if ( special )
		return nodeData.at( column );

	else
		return nodeData.value( 3 + column, QString() );
};

QVariantList PBItemViewNode::allData() {

	return nodeData;
}

bool PBItemViewNode::setData( int column, QVariant data, bool special ) {

	/*
		*
		* If we want to set special data, i.e., we can choose column 0, 1, or 2
		* If we want to set normal data, then we can choose columns 0 to 6, internally 3 to 9
		*
	*/

	if ( special ) {
		if ( ( column < 0 ) or ( column > 2 ) )
			return false;

		nodeData.replace( column, data );
		if ( column == 2 ) {
			// Icon String
			mIcon = QIcon::fromTheme( data.toString(), QIcon( data.toString() ) );

			if ( mIcon.isNull() )
				mIcon = QIcon( ":/icons/unknown.png" );
		}
	}

	else {
		if ( ( column < 0 ) or ( column > 7 ) )
			return false;

		nodeData.replace( 3 + column, data );
	}

	return true;
};

PBItemViewNode* PBItemViewNode::parent() {

	return parentNode;
};

int PBItemViewNode::row() {

	if ( parentNode )
		return parentNode->childNodes.indexOf( this );

	return 0;
};

void PBItemViewNode::sort( int column, bool cs, bool categorized ) {

	if ( categorized )
		mCategoryList = sortCategoryList( mCategoryList );

	qSort( childNodes.begin(), childNodes.end(), columnSort2 );
};

void PBItemViewNode::updateIcon() {

	mIcon = getIcon( nodeData.at( 3 ).toString(), nodeData.at( 6 ).toString() );
};

void PBItemViewNode::updateCategories() {

	PBItemViewNode::mCategoryList.clear();
	foreach( PBItemViewNode *cNode, childNodes ) {
		QString newCategory = cNode->category();
		if ( not mCategoryList.contains( newCategory ) )
			mCategoryList << newCategory;
	}

	if ( Settings->General.Grouping )
		mCategoryList = sortCategoryList( mCategoryList );
};

bool columnSort2( PBItemViewNode *first, PBItemViewNode *second )  {

	int firstIdx = first->categoryIndex();
	int secondIdx = second->categoryIndex();

	/* If its not a categorized sorting, then both have equal categorization weights */
	if ( not Settings->General.Grouping )
		firstIdx = secondIdx;

	if ( firstIdx == secondIdx ) {
		if ( ( first->data( 0, true ) == "dir" ) and ( second->data( 0, true ) == "dir" ) ) {
			switch( Settings->General.SortColumn ) {
				case 0: {
					if ( Settings->General.SortCase )
						return first->data( 0 ).toString() < second->data( 0 ).toString();

					else
						return first->data( 0 ).toString().compare( second->data( 0 ).toString(), Qt::CaseInsensitive ) < 0;
				}

				case 1: {
					return first->data( 1, true ).toLongLong() < second->data( 1, true ).toLongLong();
				}

				default: {
					if ( first->data( Settings->General.SortColumn ).toString() < second->data( Settings->General.SortColumn ).toString() )
						return true;

					else if ( first->data( Settings->General.SortColumn ).toString() == second->data( Settings->General.SortColumn ).toString() )
						if ( Settings->General.SortCase )
							return first->data( 0 ).toString().compare( second->data( 0 ).toString(), Qt::CaseSensitive ) < 0;

						else
							return first->data( 0 ).toString().compare( second->data( 0 ).toString(), Qt::CaseInsensitive ) < 0;

					else
						return false;
				}
			}
		}

		else if ( ( first->data( 0, true ) == "dir" ) and ( second->data( 0, true ) == "file" ) ) {

			return true;
		}

		else if ( ( first->data( 0, true ) == "dir" ) and ( second->data( 0, true ) == "system" ) ) {

			return true;
		}

		else if ( ( first->data( 0, true ) == "file" ) and ( second->data( 0, true ) == "dir" ) ) {

			return false;
		}

		else if ( ( first->data( 0, true ) == "file" ) and ( second->data( 0, true ) == "file" ) ) {

			switch( Settings->General.SortColumn ) {
				case 0: {
					if ( Settings->General.SortCase )
						return first->data( 0 ).toString() < second->data( 0 ).toString();

					else
						return first->data( 0 ).toString().compare( second->data( 0 ).toString(), Qt::CaseInsensitive ) < 0;
				}

				case 1: {
					return first->data( 1, true ).toLongLong() < second->data( 1, true ).toLongLong();
				}

				default: {
					if ( first->data( Settings->General.SortColumn ).toString() < second->data( Settings->General.SortColumn ).toString() )
						return true;

					else if ( first->data( Settings->General.SortColumn ).toString() == second->data( Settings->General.SortColumn ).toString() )
						if ( Settings->General.SortCase )
							return first->data( 0 ).toString().compare( second->data( 0 ).toString(), Qt::CaseSensitive ) < 0;

						else
							return first->data( 0 ).toString().compare( second->data( 0 ).toString(), Qt::CaseInsensitive ) < 0;

					else
						return false;
				}
			}
		}

		else if ( ( first->data( 0, true ) == "file" ) and ( second->data( 0, true ) == "system" ) ) {

			return true;
		}

		else if ( ( first->data( 0, true ) == "system" ) and ( second->data( 0, true ) == "dir" ) ) {

			return false;
		}

		else if ( ( first->data( 0, true ) == "system" ) and ( second->data( 0, true ) == "file" ) ) {

			return false;
		}

		else if ( ( first->data( 0, true ) == "system" ) and ( second->data( 0, true ) == "system" ) ) {

			switch( Settings->General.SortColumn ) {
				case 0: {
					if ( Settings->General.SortCase )
						return first->data( 0 ).toString() < second->data( 0 ).toString();

					else
						return first->data( 0 ).toString().compare( second->data( 0 ).toString(), Qt::CaseInsensitive ) < 0;
				}

				case 1: {
					return first->data( 1, true ).toLongLong() < second->data( 1, true ).toLongLong();
				}

				default: {
					if ( first->data( Settings->General.SortColumn ).toString() < second->data( Settings->General.SortColumn ).toString() )
						return true;

					else if ( first->data( Settings->General.SortColumn ).toString() == second->data( Settings->General.SortColumn ).toString() )
						if ( Settings->General.SortCase )
							return first->data( 0 ).toString().compare( second->data( 0 ).toString(), Qt::CaseSensitive ) < 0;

						else
							return first->data( 0 ).toString().compare( second->data( 0 ).toString(), Qt::CaseInsensitive ) < 0;

					else
						return false;
				}
			}
		}
	}

	else if ( firstIdx < secondIdx ) {
		/* Means first item is in a category earlier than the second category */
		return true;
	}

	else {
		/* Means first item is in a category later than the second category */
		return false;
	}

	return false;
};

/* function: less than */
inline bool listLessThanA( QString a, QString b ) {

	if ( a.count() != b.count() )
		return ( a.count() < b.count() );
	else
		return ( a < b );
};

inline bool listLessThanB( QString a, QString b ) {

	return a.toLower() < b.toLower();
};

QStringList sortCategoryList( QStringList& cList ) {

	switch( Settings->General.SortColumn ) {
		/* Name sort */
		case 0: {
			qSort( cList.begin(), cList.end(), listLessThanA );
			return cList;
		};

		/* Size sort */
		/* The order should be: Folders, Tiny, Small, Medium, Large, Massive */
		case 1: {
			QStringList nList;
			if ( cList.contains( "Folders" ) )
				nList << "Folders";

			if ( cList.contains( "Tiny" ) )
				nList << "Tiny";

			if ( cList.contains( "Small" ) )
				nList << "Small";

			if ( cList.contains( "Medium" ) )
				nList << "Medium";

			if ( cList.contains( "Large" ) )
				nList << "Large";

			if ( cList.contains( "Massive" ) )
				nList << "Massive";

			return nList;
		};

		/* Type sort */
		/* Folder must be the first item Then come the files */
		case 2: {
			if ( cList.contains( "Folders" ) ) {
				cList.removeAll( "Folders" );
				qSort( cList.begin(), cList.end(), listLessThanB );
				cList.insert( 0, "Folders" );
			}

			else {
				if ( cList.contains( "Computer  " ) ) {
					QStringList catList;
					catList << "Computer  ";
					if ( cList.contains( "Places  " ) )
						catList << "Places  ";
					if ( cList.contains( "Files  " ) )
						catList << "Files  ";

					return catList;
				}
				qSort( cList.begin(), cList.end(), listLessThanB );
			}

			return cList;
		};

		/* Date sort */
		/* We convert all the String dates to QDates, sort them and then convert them back to String */
		case 4: {
			QStringList alphaDates = QStringList() << "Today" <<"This Week" << "Last Week" << "This Month" << "Last Month";

			QList<QDate> dList;
			QStringList nList;
			Q_FOREACH( QString date, cList ) {
				if ( not alphaDates.contains( date ) )
					dList << QDate::fromString( date, "MMMM yyyy" );
			}

			qSort( dList.begin(), dList.end() );
			Q_FOREACH( QDate date, dList )
				nList << date.toString( "MMMM yyyy" );

			/* This ensures that the user sees the Alphabetic dates at first */
			if ( cList.contains( "Last Month" ) )
				nList.insert( 0, "Last Month" );

			if ( cList.contains( "This Month" ) )
				nList.insert( 0, "This Month" );

			if ( cList.contains( "Last Week" ) )
				nList.insert( 0, "Last Week" );

			if ( cList.contains( "This Week" ) )
				nList.insert( 0, "This Week" );

			if ( cList.contains( "Today" ) )
				nList.insert( 0, "Today" );

			return nList;
		};

		default: {
			return cList;
		}
	};
};
