/*
	*
	* PBMimeTypeNode.cpp - PiBreeze PBMimeTypeNode Class
	*
*/

#include "PBMimeTypeNode.hpp"

PBMimeTypeNode::PBMimeTypeNode( QMimeType mimeType, PBMimeTypeNode *parent ) {

	mCategory = mimeType.name().split( "/" ).value( 0 );
	mName = mimeType.name().split( "/" ).value( 1 );
	mIcon = QIcon::fromTheme( "application-octet-stream" );

	parentNode = parent;
};

void PBMimeTypeNode::addChild( PBMimeTypeNode *node ) {

	childNodes << node;
};

void PBMimeTypeNode::removeChild( PBMimeTypeNode *node ) {

	delete childNodes.takeAt( node->row() );
};

PBMimeTypeNode* PBMimeTypeNode::child( int row ) {

	return childNodes.at( row );
};

PBMimeTypeNode* PBMimeTypeNode::child( QString name ) {

	foreach( PBMimeTypeNode* node, childNodes )
		if ( node->name() == name )
			return node;

	return new PBMimeTypeNode();
};

QList<PBMimeTypeNode*> PBMimeTypeNode::children() {

	return childNodes;
};

int PBMimeTypeNode::childCount() {

	return childNodes.count();
};

void PBMimeTypeNode::clearChildren() {

	childNodes.clear();
};

QString PBMimeTypeNode::name() {

	return mName;
}

QString PBMimeTypeNode::category() {

	return mCategory;
};

QIcon PBMimeTypeNode::icon() {

	return mIcon;
}

void PBMimeTypeNode::setIcon( QIcon newIcon ) {

	mIcon = newIcon;
}

PBMimeTypeNode* PBMimeTypeNode::parent() {

	return parentNode;
};

int PBMimeTypeNode::row() {

	if ( parentNode )
		return parentNode->childNodes.indexOf( this );

	return 0;
};

void PBMimeTypeNode::sort() {

	qSort( childNodes.begin(), childNodes.end(), mimeTypeNodeSort );
};

bool mimeTypeNodeSort( PBMimeTypeNode *first, PBMimeTypeNode *second ) {

	if ( first->category() == second->category() ) {

		return first->name().toLower() < second->name().toLower();
	}

	else {

		return first->category() < second->category();
	}
};
