/*
	*
	* PBItemViewNode.hpp - PiBreeze PBItemViewNode Class Header
	*
*/

#pragma once

#include "Global.hpp"

class PBItemViewNode {

	public:
		PBItemViewNode();
		PBItemViewNode( QVariantList, QString, PBItemViewNode *parent = 0 );

		~PBItemViewNode();

		int childCount();
		int categoryCount();

		void clearChildren();

		void addChild( PBItemViewNode* );
		void removeChild( PBItemViewNode* );

		PBItemViewNode* child( int );
		PBItemViewNode* child( QString );
		QList<PBItemViewNode*> children();

		QVariant data( int column, bool special = false ) const;
		QVariantList allData();
		bool setData( int column, QVariant data, bool special = false );

		QString category();
		void setCategory( QString );

		int categoryIndex();

		QStringList categoryList();

		PBItemViewNode *parent();
		int row();

		void sort( int column = 0, bool cs = false, bool categorized = false );

		void updateIcon();
		void updateCategories();

		QIcon icon();

	private:
		QList<PBItemViewNode*> childNodes;
		QVariantList nodeData;
		PBItemViewNode *parentNode;

		QStringList mCategoryList;
		QString myCategory;
		QIcon mIcon;

		QFuture<void> future;
};

bool columnSort2( PBItemViewNode *first, PBItemViewNode *second );
QStringList sortCategoryList( QStringList& );
