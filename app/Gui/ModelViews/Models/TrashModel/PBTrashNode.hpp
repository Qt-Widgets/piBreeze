/*
	*
	* PBTrashNode.hpp - PiBreeze PBTrashNode Class Header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBIconManager.hpp"

class PBTrashNode {

	public:
		PBTrashNode();
		PBTrashNode( QStringList, QString, PBTrashNode *parent = 0 );

		int childCount();
		int categoryCount();

		void clearChildren();

		void addChild( PBTrashNode* );
		void removeChild( PBTrashNode* );

		PBTrashNode* child( int );
		PBTrashNode* child( QString );
		QList<PBTrashNode*> children();

		QVariant data( int column ) const;
		QStringList allData();
		bool setData( int column, QString data );

		/* Info functions */
		QString name();
		QIcon icon();
		QString size();
		QDateTime deletionDate();
		QString originalPath();
		QString trashPath();
		QString trashInfoPath();
		QString type();

		QString category();
		void setCategory( QString );

		int categoryIndex();

		QStringList categoryList();

		PBTrashNode *parent();
		int row();

		void sort();

		void updateCategories();
		QStringList mCategoryList;

	private:
		QList<PBTrashNode*> childNodes;
		QStringList nodeData;
		PBTrashNode *parentNode;

		QString myCategory;
		QString mType;
		QString mSize;
		QDateTime mDate;
		QIcon mIcon;
};

bool columnSort( PBTrashNode *first, PBTrashNode *second );
QStringList sortDateList( QStringList& );
