/*
	*
	* PBIconViewRestricted.cpp - PiBreeze IconView Class
	*
*/

#include "PBIconViewRestricted.hpp"

PBIconViewRestricted::PBIconViewRestricted( PBItemViewModel *fsModel, QWidget *parent ) : PBIconView( fsModel, parent ) {

	mdl = fsModel;

	setSelectionMode( QAbstractItemView::NoSelection );
	setContextMenuPolicy( Qt::NoContextMenu );

	viewport()->setAcceptDrops( false );
	setDragEnabled( false );
	setAcceptDrops( false );
	setDragDropMode( QListView::NoDragDrop );
	setDropIndicatorShown( false );
};

void PBIconViewRestricted::mousePressEvent( QMouseEvent *mpEvent ) {

	/* Disable all the mouse press events */
	QAbstractItemView::mousePressEvent( mpEvent );
};

void PBIconViewRestricted::mouseMoveEvent( QMouseEvent *mmEvent ) {

	mmEvent->accept();
};

void PBIconViewRestricted::mouseReleaseEvent( QMouseEvent *mrEvent ) {

	mrEvent->accept();
};

void PBIconViewRestricted::mouseDoubleClickEvent( QMouseEvent *mEvent ) {

	if ( mEvent->button() == Qt::LeftButton ) {
		QModelIndex idx = indexAt( mEvent->pos() );
		if ( idx.isValid() ) {
			switch( mdl->modelDataType() ) {
				case PBItemViewModel::SuperStart:
				case PBItemViewModel::Applications:
				case PBItemViewModel::Catalogs:
					break;

				case PBItemViewModel::FileSystem: {

					emit open( idx );
					break;
				}
			}
		}
	}

	mEvent->accept();
};

void PBIconViewRestricted::dragEnterEvent( QDragEnterEvent *deEvent ) {

	deEvent->ignore();
};

void PBIconViewRestricted::dragMoveEvent( QDragMoveEvent *dmEvent ) {

	dmEvent->ignore();
};

void PBIconViewRestricted::dropEvent( QDropEvent *dpEvent ) {

	dpEvent->ignore();
};
