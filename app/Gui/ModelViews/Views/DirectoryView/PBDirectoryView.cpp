/*
	*
	* PBDirectoryView.cpp - PiBreeze DirectoryView class
	*
*/

#include "PBDirectoryView.hpp"

void PBDirViewDelegate::paint( QPainter* painter, const QStyleOptionViewItem &styleOpts, const QModelIndex &idx ) const {

	if ( not idx.isValid() )
		QStyledItemDelegate::paint( painter, styleOpts, idx );

	QStyleOptionViewItemV4 newOpts( styleOpts );

	QString path( idx.data( Qt::UserRole +1 ).toString() );
	if ( isReadable( path ) and isWritable( path ) ) {
		newOpts.palette.setColor( QPalette::WindowText, Qt::black );
		newOpts.palette.setColor( QPalette::Text, Qt::black );
	}

	else if ( isReadable( path ) ) {
		newOpts.palette.setColor( QPalette::WindowText, Qt::gray );
		newOpts.palette.setColor( QPalette::Text, Qt::gray );
	}

	else {
		newOpts.palette.setColor( QPalette::WindowText, Qt::darkRed );
		newOpts.palette.setColor( QPalette::Text, Qt::darkRed );
	}

	QStyledItemDelegate::paint( painter, newOpts, idx );
};

PBDirectoryView::PBDirectoryView( QWidget *parent ) : QTreeView( parent ) {

	// Model
	mdl = new PBDirTreeModel();
	setModel( mdl );

	// Internal Object Name
	setObjectName( "mainList" );

	/* Styling */
	setStyleSheet( "#mainList{ border:none; }" );

	/* ItemDelegate */
	setItemDelegate( new PBDirViewDelegate() );
};

QString PBDirectoryView::currentBranch() const {

	return currentIndex().data( Qt::UserRole + 1 ).toString();
};

void PBDirectoryView::setCurrentBranch( QString path ) {

	if ( not exists( path ) or not isReadable( path ) )
		return;

	QStringList nodeList;
	nodeList << path.split( "/", QString::SkipEmptyParts );

	/* First Index is the root index, we expand it */
	QModelIndex idx = mdl->index( 0, 0, QModelIndex() );
	expand( idx );

	Q_FOREACH( QString node, nodeList ) {
		idx = mdl->index( node, idx );
		expand( idx );
	}

	setCurrentIndex( idx );
};
