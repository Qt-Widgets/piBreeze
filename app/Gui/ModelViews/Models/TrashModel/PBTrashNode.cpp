/*
	*
	* PBTrashNode.cpp - PiBreeze PBTrashNode Class
	*
*/

#include "PBTrashNode.hpp"

PBTrashNode::PBTrashNode() {

	for( int i = 0; i < 10; i++ )
		nodeData << "";

	mCategoryList.clear();
	myCategory = "Uncategorized";
	parentNode = 0;
};

PBTrashNode::PBTrashNode( QStringList data, QString category, PBTrashNode *parent ) {

	nodeData << data;
	myCategory = category;

	if ( isDir( data.last() ) )
		mType = "dir";

	else if ( isFile( data.last() ) )
		mType = "file";

	else
		mType = "system";

	mSize = formatSize( getSize( data.last() ) );
	mDate = QDateTime::fromString( data.at( 2 ), "yyyyMMddThh:mm:ss" );

	QStringList icoStrs = PBIconManager::instance()->iconsForFile( QString(), data.last() );
	Q_FOREACH( QString icoStr, icoStrs ) {
		if ( icoStr.contains( dirName( data.at( 1 ) ) ) )
			icoStr.replace( dirName( data.at( 1 ) ), dirName( data.at( 3 ) ) );

		mIcon.addFile( icoStr );
	}

	parentNode = parent;
};

void PBTrashNode::addChild( PBTrashNode *node ) {

	if ( not mCategoryList.contains( node->category() ) )
		mCategoryList << node->category();

	childNodes << node;
};

void PBTrashNode::removeChild( PBTrashNode *node ) {

	delete childNodes.takeAt( node->row() );
};

PBTrashNode* PBTrashNode::child( int row ) {

	return childNodes.at( row );
};

PBTrashNode* PBTrashNode::child( QString name ) {

	foreach( PBTrashNode* node, childNodes )
		if ( node->data( 0 ) == name )
			return node;

	return new PBTrashNode();
};

QList<PBTrashNode*> PBTrashNode::children() {

	return childNodes;
};

int PBTrashNode::childCount() {

	return childNodes.count();
};

int PBTrashNode::categoryCount() {

	return mCategoryList.count();
};

void PBTrashNode::clearChildren() {

	childNodes.clear();
	mCategoryList.clear();
};

QString PBTrashNode::category() {

	return myCategory;
};

void PBTrashNode::setCategory( QString newCategory ) {

	myCategory = newCategory;
};

int PBTrashNode::categoryIndex() {

	return parentNode->mCategoryList.indexOf( myCategory );
};

QStringList PBTrashNode::categoryList() {

	return mCategoryList;
};

QVariant PBTrashNode::data( int column ) const {

	/*
		*
		* If we want special data, i.e., we can choose column 0, 1, or 2
		* If we want normal data, then we can choose columns 0 to 6, internally 3 to 9
		*
	*/

	if ( ( column < 0 ) or ( column > 7 ) )
		return QVariant();

	return nodeData.at( column );
};

QStringList PBTrashNode::allData() {

	return nodeData;
}

bool PBTrashNode::setData( int column, QString data ) {

	/*
		*
		* If we want to set special data, i.e., we can choose column 0, 1, or 2
		* If we want to set normal data, then we can choose columns 0 to 6, internally 3 to 9
		*
	*/

	if ( ( column < 0 ) or ( column > 7 ) )
		return false;

	nodeData.replace( column, data );

	return true;
};

QString PBTrashNode::name() {

	return nodeData.at( 0 );
};

QString PBTrashNode::type() {

	return mType;
};

QDateTime PBTrashNode::deletionDate() {

	return mDate;
};

QString PBTrashNode::originalPath() {

	return nodeData.at( 1 );
};

QString PBTrashNode::trashPath() {

	return nodeData.last();
};

QString PBTrashNode::trashInfoPath() {

	return nodeData.last().replace( "/files/", "/info/" ) + ".trashinfo";
};

QString PBTrashNode::size() {

	return mSize;
};

QIcon PBTrashNode::icon() {

	return mIcon;
};

PBTrashNode* PBTrashNode::parent() {

	return parentNode;
};

int PBTrashNode::row() {

	if ( parentNode )
		return parentNode->childNodes.indexOf( this );

	return 0;
};

void PBTrashNode::sort() {

	mCategoryList = sortDateList( mCategoryList );
	qSort( childNodes.begin(), childNodes.end(), columnSort );
};

void PBTrashNode::updateCategories() {

	PBTrashNode::mCategoryList.clear();
	foreach( PBTrashNode *cNode, childNodes ) {
		QString newCategory = cNode->category();
		if ( not mCategoryList.contains( newCategory ) )
			mCategoryList << newCategory;
	}

	mCategoryList = sortDateList( mCategoryList );
}

bool columnSort( PBTrashNode *first, PBTrashNode *second )  {

	int firstIdx = first->categoryIndex();
	int secondIdx = second->categoryIndex();

	if ( firstIdx == secondIdx ) {
		if ( ( first->type() == "dir" ) and ( second->type() == "dir" ) ) {

			if ( first->deletionDate() == second->deletionDate() )
				return first->name().compare( second->name(), Qt::CaseInsensitive ) < 0;

			else
				return first->deletionDate() < second->deletionDate();
		}

		else if ( ( first->type() == "dir" ) and ( second->type() == "file" ) ) {

			return true;
		}

		else if ( ( first->type() == "dir" ) and ( second->type() == "system" ) ) {

			return true;
		}

		else if ( ( first->type() == "file" ) and ( second->type() == "dir" ) ) {

			return false;
		}

		else if ( ( first->type() == "file" ) and ( second->type() == "file" ) ) {

			if ( first->deletionDate() == second->deletionDate() )
				return first->name().compare( second->name(), Qt::CaseInsensitive ) < 0;

			else
				return first->deletionDate() < second->deletionDate();
		}

		else if ( ( first->type() == "file" ) and ( second->type() == "system" ) ) {

			return true;
		}

		else if ( ( first->type() == "system" ) and ( second->type() == "dir" ) ) {

			return false;
		}

		else if ( ( first->type() == "system" ) and ( second->type() == "file" ) ) {

			return false;
		}

		else if ( ( first->type() == "system" ) and ( second->type() == "system" ) ) {

			if ( first->deletionDate() == second->deletionDate() )
				return first->name().compare( second->name(), Qt::CaseInsensitive ) < 0;

			else
				return first->deletionDate() < second->deletionDate();
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


QStringList sortDateList( QStringList& cList ) {

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
