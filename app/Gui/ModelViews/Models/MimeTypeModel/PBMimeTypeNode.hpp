/*
	*
	* PBMimeTypeNode.hpp - PiBreeze PBMimeTypeNode Class Header
	*
*/

#pragma once

#include "Global.hpp"

class PBMimeTypeNode {

	public:
		PBMimeTypeNode( QMimeType mimeType = QMimeType(), PBMimeTypeNode *parent = 0 );

		int childCount();

		void clearChildren();

		void addChild( PBMimeTypeNode* );
		void removeChild( PBMimeTypeNode* );

		PBMimeTypeNode* child( int );
		PBMimeTypeNode* child( QString );
		QList<PBMimeTypeNode*> children();

		PBMimeTypeNode *parent();
		int row();

		void sort();

		QString name();
		QString category();

		QIcon icon();
		void setIcon( QIcon );

	private:
		QList<PBMimeTypeNode*> childNodes;
		PBMimeTypeNode *parentNode;

		QString mCategory;
		QString mName;
		QIcon mIcon;
};

bool mimeTypeNodeSort( PBMimeTypeNode *first, PBMimeTypeNode *second );
