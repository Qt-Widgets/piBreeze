/*
	*
	* PBIconView.cpp - PiBreeze IconView Class
	*
*/

#include "PBIconView.hpp"
#include "PBStyleOptionViewItem.hpp"

PBIconView::PBIconView( PBItemViewModel *fsModel, QWidget *parent ) : QAbstractItemView( parent ) {

	/* Icon rects */
	idealHeight = 0;
	hashIsDirty = true;

	/* Category Height */
	myCategoryHeight = 24;

	/* Category Spacing */
	myCategorySpacing = 10;

	/* Contents Margins */
	myContentsMargins = QMargins( 10, 10, 10, 10 );

	/* Inlay Margins */
	myInlayMargins = QMargins( 10, 0, 10, 0 );

	/* Grid Size */
	myGridSizeMin = QSize( 120, 80 );
	myGridSize = QSize( 120, 80 );
	myItemSize = QSize( 110, 70 );

	/* Persistent vertical column */
	persistentVCol = 0;
	/* QPair( catIdx, elmInCatIdx */
	curIdx = qMakePair( 0, 0 );

	/* Items per visual row */
	itemsPerRow = 1;
	numberOfRows = 0;
	padding = 0;

	/* Set the Apps Delegate */
	setItemDelegate( new PBIconDelegate() );

	/* Applications Model */
	cModel = fsModel;
	setModel( cModel );

	/* Icon Size */
	setIconSize( Settings->General.IconSize );

	/* Default Selection Rules */
	setSelectionMode( QAbstractItemView::ExtendedSelection );
	setSelectionBehavior( QAbstractItemView::SelectRows );
	mSelectedIndexes = QModelIndexList();

	/* Internal Object Name */
	setObjectName( "mainList" );

	/* Styling */
	setStyleSheet( "#mainList{ border:none; }" );

	/* Minimum Size */
	setMinimumWidth( 640 );

	/* Focus Policy */
	setFocusPolicy( Qt::StrongFocus );

	/* Font */
	setFont( qApp->font() );

	/* Mouse tracking */
	setMouseTracking( true );

	/* No Horizontal ScrollBar */
	setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

	/* DragAndDrop */
	viewport()->setAcceptDrops( true );
	setDragEnabled( true );
	setAcceptDrops( true );
	setDragDropMode( QListView::DragDrop );
	setDropIndicatorShown( true );

	/* Context Menu */
	setContextMenuPolicy( Qt::CustomContextMenu );

	connect( cModel, SIGNAL( directoryLoaded( QString ) ), this, SLOT( reload() ) );
	connect( cModel, SIGNAL( layoutChanged() ), this, SLOT( reload() ) );
	connect( this, SIGNAL( customContextMenuRequested( QPoint ) ), this, SIGNAL( contextMenuRequested( QPoint ) ) );

	/* Zoom In and Out actions */
	QAction *zoomInAct = new QAction( "Zoom In", this );
	zoomInAct->setShortcut( QKeySequence::ZoomIn );
	connect( zoomInAct, SIGNAL( triggered() ), this, SLOT( zoomIn() ) );
	addAction( zoomInAct );

	QAction *zoomOutAct = new QAction( "Zoom Out", this );
	zoomOutAct->setShortcut( QKeySequence::ZoomOut );
	connect( zoomOutAct, SIGNAL( triggered() ), this, SLOT( zoomOut() ) );
	addAction( zoomOutAct );
};

void PBIconView::setModel( QAbstractItemModel *model ) {

	QAbstractItemView::setModel( model );

	hashIsDirty = true;
};

int PBIconView::categoryHeight() const {

	return myCategoryHeight;
};

void PBIconView::setCategoryHeight( int newCategoryHeight ) {

	myCategoryHeight = newCategoryHeight;

	hashIsDirty = true;
	calculateRectsIfNecessary();
};

QSize PBIconView::iconSize() const {

	return myIconSize;
};

void PBIconView::setIconSize( QSize newIconSize ) {

	myIconSize = newIconSize;
	computeGridSize( myIconSize );

	viewport()->update();
};

void PBIconView::setIconSize( int nWidth, int nHeight ) {

	setIconSize( QSize( nWidth, nHeight ) );
};

QMargins PBIconView::contentsMargins() const {

	return myContentsMargins;
};

void PBIconView::setContentsMargins( QMargins newContentsMargins ) {

	myContentsMargins = newContentsMargins;

	hashIsDirty = true;
	calculateRectsIfNecessary();
};

void PBIconView::setContentsMargins( int nLeft, int nTop, int nRight, int nBottom ) {

	setContentsMargins( QMargins( nLeft, nTop, nRight, nBottom ) );
};

QMargins PBIconView::inlayMargins() const {

	return myInlayMargins;
};

void PBIconView::setInlayMargins( QMargins newInlayMargins ) {

	myInlayMargins = newInlayMargins;

	hashIsDirty = true;
	calculateRectsIfNecessary();
};

void PBIconView::setInlayMargins( int nLeft, int nTop, int nRight, int nBottom ) {

	setInlayMargins( QMargins( nLeft, nTop, nRight, nBottom ) );

	hashIsDirty = true;
	calculateRectsIfNecessary();
};

int PBIconView::categorySpacing() const {

	return myCategorySpacing;
};

void PBIconView::setCategorySpacing( int newCategorySpacing ) {

	myCategorySpacing = newCategorySpacing;

	hashIsDirty = true;
	calculateRectsIfNecessary();
};

QRect PBIconView::visualRect( const QModelIndex &index ) const {

	QRect rect;
	if ( index.isValid() )
		rect = viewportRectForRow( index.row() );

	return rect;
};

QRect PBIconView::categoryRect( int categoryIndex ) const {

	calculateRectsIfNecessary();
	QRect rect = rectForCategory.value( categoryIndex );
	if ( not rect.isValid() )
		return QRect();

    return QRect( rect.x(), rect.y() - verticalScrollBar()->value(), rect.width(), rect.height() );
};

void PBIconView::scrollTo( const QModelIndex &index, QAbstractItemView::ScrollHint ) {

	QRect viewRect = viewport()->rect();
	QRect itemRect = visualRect( index );

	if ( itemRect.top() < viewRect.top() ) {
		verticalScrollBar()->setValue( verticalScrollBar()->value() + itemRect.top() - viewRect.top() );
	}

	else if ( itemRect.bottom() > viewRect.bottom() ) {
		verticalScrollBar()->setValue(
			verticalScrollBar()->value() + qMin( itemRect.bottom() - viewRect.bottom(), itemRect.top() - viewRect.top() )
		);
	}

	viewport()->update();
};

QModelIndex PBIconView::indexAt( const QPoint &point_ ) const {

	QPoint point( point_ );
	point.ry() += verticalScrollBar()->value();

	calculateRectsIfNecessary();
	QHashIterator<int, QPoint> i( rectForRow );
	while ( i.hasNext() ) {

		i.next();
		if ( QRect( i.value(), myGridSize ).adjusted( padding / 2, padding / 2, -padding, -padding ).contains( point ) )
			return cModel->index( i.key(), 0, rootIndex() );
	}

	return QModelIndex();
};

QString PBIconView::categoryAt( const QPoint &point_ ) const {

	QPoint point( point_ );
	point.ry() += verticalScrollBar()->value();
	calculateRectsIfNecessary();
	QHashIterator<int, QRect> i( rectForCategory );
	while ( i.hasNext() ) {

		i.next();
		if ( i.value().contains( point ) )
			return categoryList.value( i.key() );
	}

	return QString();
};

void PBIconView::dataChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight ) {

	hashIsDirty = true;
	QAbstractItemView::dataChanged( topLeft, bottomRight );
};

void PBIconView::rowsInserted( const QModelIndex &parent, int start, int end ) {

	hashIsDirty = true;
	QAbstractItemView::rowsInserted( parent, start, end );
};

void PBIconView::rowsAboutToBeRemoved( const QModelIndex &parent, int start, int end ) {

	hashIsDirty = true;
	QAbstractItemView::rowsAboutToBeRemoved( parent, start, end );
};

void PBIconView::updateGeometries() {

	computeRowsAndColumns();

	if ( idealHeight <= viewport()->height() ) {
		verticalScrollBar()->setValue( 0 );
		verticalScrollBar()->hide();
	}

	else {
		verticalScrollBar()->show();
		verticalScrollBar()->setSingleStep( myGridSize.height() );
		verticalScrollBar()->setPageStep( viewport()->height() );
		verticalScrollBar()->setRange( 0, qMax( 0, idealHeight - viewport()->height() ) );
	}
};

void PBIconView::reload() {

	switch( cModel->modelDataType() ) {
		case PBItemViewModel::FileSystem: {

			setSelectionMode( QAbstractItemView::ExtendedSelection );
			break;
		}

		default: {

			setSelectionMode( QAbstractItemView::SingleSelection );
			break;
		}
	}

	/* Change view mode according to the .desktop file */
	QSettings sett( cModel->nodePath( ".directory" ), QSettings::NativeFormat );

	/* We have set per-folder settings */
	int iconSize = sett.value( "PiBreeze/IconSize", Settings->General.IconSize.width() ).toInt();

	setIconSize( iconSize, iconSize );

	hashIsDirty = true;
	persistentVCol = 0;

	mSelectedIndexes.clear();
	hiddenCategories.clear();

	categoryList = cModel->categories();
	calculateRectsIfNecessary();
};

QModelIndex PBIconView::moveCursor( QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers ) {
	/*
		*
		* We still do not handle keyboard modifiers. This is yet to be implemented
		*
	*/

	if ( cModel->isCategorizationEnabled() )
		return moveCursorCategorized( cursorAction );

	else
		return moveCursorNonCategorized( cursorAction );
};

int PBIconView::horizontalOffset() const {

	return horizontalScrollBar()->value();
};

int PBIconView::verticalOffset() const {

	return verticalScrollBar()->value();
};

void PBIconView::scrollContentsBy( int dx, int dy ) {

	scrollDirtyRegion( dx, dy );
	viewport()->scroll( dx, dy );
};

void PBIconView::setSelection( const QRect &rect, QFlags<QItemSelectionModel::SelectionFlag> flags ) {

	QRect rectangle = rect.translated( horizontalScrollBar()->value(), verticalScrollBar()->value() ).normalized();

	calculateRectsIfNecessary();
	QHashIterator<int, QPoint> i( rectForRow );
	int firstRow = cModel->rowCount();
	int lastRow = -1;
	while ( i.hasNext() ) {

		i.next();
		if ( QRect( i.value(), myGridSize ).intersects( rectangle ) ) {
			firstRow = firstRow < i.key() ? firstRow : i.key();
			lastRow = lastRow > i.key() ? lastRow : i.key();
		}
	}

	if ( firstRow != cModel->rowCount() && lastRow != -1 ) {
		QItemSelection selection( cModel->index( firstRow, 0, rootIndex() ), cModel->index( lastRow, 0, rootIndex() ) );
		selectionModel()->select( selection, flags );
	}

	else {
		QModelIndex invalid;
		QItemSelection selection( invalid, invalid );
		selectionModel()->select( selection, flags );
	}
};

QRegion PBIconView::visualRegionForSelection( const QItemSelection &selection ) const {

	QRegion region;
	foreach ( const QItemSelectionRange &range, selection ) {
		for ( int row = range.top(); row <= range.bottom(); ++row ) {
			for ( int column = range.left(); column < range.right(); ++column ) {
				QModelIndex index = cModel->index( row, column, rootIndex() );
				region += visualRect( index );
			}
		}
	}

	return region;
};

QModelIndexList PBIconView::selectedIndexes() {

	QSet<QModelIndex> idxSet;
	idxSet.unite( QSet<QModelIndex>::fromList( mSelectedIndexes ) );
	idxSet.unite( QSet<QModelIndex>::fromList( selectionModel()->selectedIndexes() ) );

	return idxSet.toList();
};

QModelIndexList PBIconView::selection() {

	QSet<QModelIndex> idxSet;
	idxSet.unite( QSet<QModelIndex>::fromList( mSelectedIndexes ) );
	idxSet.unite( QSet<QModelIndex>::fromList( selectionModel()->selectedIndexes() ) );

	return idxSet.toList();
};

bool PBIconView::isIndexVisible( QModelIndex idx ) const {

	/* We consider the space reserved for the category but not the indexes listed under it */
	if ( hiddenCategories.contains( cModel->category( idx ) ) )
		return false;

	/* If the index belongs to a folded category and the index position is greater than itemsPerRow - 1 */
	if ( foldedCategories.contains( cModel->category( idx ) ) and cModel->indexInCategory( idx ) >= itemsPerRow - 1 )
		return false;

	QRect rect = viewportRectForRow( idx.row() );
	if ( !rect.isValid() || rect.bottom() < 0 || rect.y() > viewport()->height() )
		return false;

	else
		return true;
};

void PBIconView::paintEvent( QPaintEvent* event ) {

	QPainter painter( viewport() );
	painter.setRenderHints( QPainter::Antialiasing | QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing );

	painter.fillRect( viewport()->rect(), palette().color( QPalette::Base ) );

	/* We need to draw the categories only if the model is categorization enabled */
	if ( cModel->isCategorizationEnabled() ) {
		for ( int catIdx = 0; catIdx < categoryList.count(); catIdx++ ) {

			QRect rect = categoryRect( catIdx );
			if ( !rect.isValid() || rect.bottom() < 0 || rect.y() > viewport()->height() )
				continue;

			paintCategory( &painter, rect, categoryList.at( catIdx ) );
		}
	}

	for ( int row = 0; row < cModel->rowCount( rootIndex() ); row++ ) {
		QModelIndex idx = cModel->index( row, 0, rootIndex() );
		if ( not canShowIndex( idx ) ) {
			/* If we have the category to which this index belongs, continue to next index */
			if ( hiddenCategories.contains( cModel->category( idx ) ) )
				continue;

			/* If the position of this index is the last in row of a folded category, paint a '+' */
			if ( cModel->indexInCategory( idx ) == itemsPerRow - 1 ) {
				QRect rect = viewportRectForRow( row - 1 );
				rect.translate( myGridSize.width(), 0 );
				if ( !rect.isValid() || rect.bottom() < 0 || rect.y() > viewport()->height() )
					continue;

				paintFolded( &painter, rect, idx );
			}
			continue;
		}

		QRect rect = viewportRectForRow( row );
		if ( !rect.isValid() || rect.bottom() < 0 || rect.y() > viewport()->height() )
			continue;

		PBStyleOptionViewItem option = PBStyleOptionViewItem( viewOptions() );

		/* Rect */
		option.rect = rect;

		/* Selection */
		if ( mSelectedIndexes.contains( idx ) or selectionModel()->isSelected( idx ) )
			option.state |= QStyle::State_Selected;

		/* Focus */
		if ( currentIndex() == idx )
			option.state |= QStyle::State_HasFocus;

		/* Mouse over */
		QPoint mousePos = mapFromGlobal( cursor().pos() );
		if ( rect.contains( mousePos ) )
			option.state |= QStyle::State_MouseOver;

		/* decoration size */
		option.decorationSize = myIconSize;

		/* Palette */
		QPalette pltt = qApp->palette();
		QFileInfo ftype = cModel->nodeInfo( idx );

		if ( ftype.isSymLink() )
			option.nodeType = PBStyleOptionViewItem::SymLink;

		else if ( isExecutable( ftype.absoluteFilePath().toLocal8Bit().data() ) && ftype.isFile() )
			option.nodeType = PBStyleOptionViewItem::Executable;


		/* Dark text colors will suit here */
		if ( isBrightColor( pltt.color( QPalette::Base ), pltt.color( QPalette::Highlight ) ) ) {
			if ( ftype.isSymLink() )
				pltt.setColor( QPalette::Text, QColor( "darkslateblue" ) );

			else if ( isExecutable( ftype.absoluteFilePath().toLocal8Bit().data() ) && ftype.isFile() )
				pltt.setColor( QPalette::Text, QColor( "darkgreen" ) );

			else if ( ftype.isHidden() )
				pltt.setColor( QPalette::Text, pltt.color( QPalette::Text ).lighter() );

			else if ( !ftype.isReadable() and cModel->isRealLocation() )
				pltt.setColor( QPalette::Text, QColor( "darkred" ) );

			else if ( option.state & QStyle::State_Selected )
				pltt.setColor( QPalette::Text, pltt.color( QPalette::HighlightedText ) );

			else
				pltt.setColor( QPalette::Text, palette().color( QPalette::Text ) );

			/* Bright text will be used for drawing the 'current rect' */
			pltt.setColor( QPalette::BrightText, pltt.color( QPalette::Highlight ).darker() );

			/* ButtonText will be used to paint the extra details */
			pltt.setColor( QPalette::ButtonText, pltt.color( QPalette::Text ).lighter( 135 ) );
		}

		/* Light text colors to be used here */
		else {
			if ( ftype.isSymLink() )
				pltt.setColor( QPalette::Text, QColor( "skyblue" ) );

			else if ( isExecutable( ftype.absoluteFilePath().toLocal8Bit().data() ) && ftype.isFile() )
				pltt.setColor( QPalette::Text, QColor( "darkgreen" ) );

			else if ( ftype.isHidden() )
				pltt.setColor( QPalette::Text, pltt.color( QPalette::Text ).darker( 125 ) );

			else if ( !ftype.isReadable() and cModel->isRealLocation() )
				pltt.setColor( QPalette::Text, QColor( "indianred" ) );

			else if ( option.state & QStyle::State_Selected )
				pltt.setColor( QPalette::Text, pltt.color( QPalette::HighlightedText ) );

			else
				pltt.setColor( QPalette::Text, palette().color( QPalette::Text ) );

			/* Bright text will be used for drawing the 'current rect' */
			pltt.setColor( QPalette::BrightText, pltt.color( QPalette::Highlight ).lighter() );

			/* ButtonText will be used to paint the extra details */
			pltt.setColor( QPalette::ButtonText, pltt.color( QPalette::Text ).darker( 135 ) );
		}

		option.palette = pltt;

		PBIconDelegate *dlgt = qobject_cast<PBIconDelegate*>( itemDelegate() );
		dlgt->paintIcons( &painter, option, idx );
	}

	painter.end();
	event->accept();
};

void PBIconView::paintSelection( QPainter *painter, const QModelIndexList indexes ) const {

	if ( not indexes.count() )
		return;

	painter->save();
	painter->setPen( QPen( palette().color( QPalette::Highlight ), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );

	QColor bg = palette().color( QPalette::Highlight );
	bg.setAlpha( 120 );
	painter->setBrush( bg );

	QPainterPath path;
	Q_FOREACH( QModelIndex idx, indexes )
		path.addRoundedRect( QRectF( viewportRectForRow( idx.row() ) ), 4, 4 );

	painter->drawPath( path );
	painter->restore();
};

void PBIconView::resizeEvent( QResizeEvent* ) {

	hashIsDirty = true;
	calculateRectsIfNecessary();
};

void PBIconView::mousePressEvent( QMouseEvent *mpEvent ) {

	if ( mpEvent->button() != Qt::LeftButton ) {
		QAbstractItemView::mousePressEvent( mpEvent );
		return;
	}

	/* Index at mouse position */
	QModelIndex idx = indexAt( mpEvent->pos() );

	/* NoModifier/AltModifier pressed with the mouse */
	if ( qApp->keyboardModifiers().testFlag( Qt::NoModifier ) or qApp->keyboardModifiers().testFlag( Qt::AltModifier ) ) {

		/* Valid index */
		if ( idx.isValid() ) {
			/* Index already selected, start the drag */
			if ( mSelectedIndexes.contains( idx ) or selectionModel()->isSelected( idx ) ) {
				dragStartPosition = mpEvent->pos();
				return;
			}

			/* Index not selected, select it */
			else {
				selectionModel()->clearSelection();
				mSelectedIndexes.clear();

				mSelectedIndexes << idx;
				mSelStartIdx = idx;
			}

			/* Repaint the viewport */
			viewport()->repaint();
		}

		/* Valid category */
		else if ( categoryAt( mpEvent->pos() ).count() ) {
			QRect rct = categoryRect( categoryList.indexOf( categoryAt( mpEvent->pos() ) ) );
			QPoint topLeft = rct.topLeft();

			/* Category arrow clicked */
			if ( QRect( topLeft.x() + 4, topLeft.y() + 4, 16, 16 ).contains( mpEvent->pos() ) )
				toggleFoldCategory( categoryAt( mpEvent->pos() ) );

			/* Repaint the viewport */
			viewport()->repaint();
		}

		/* Click on empty space */
		else {
			/* Clear the selections */
			mSelectedIndexes.clear();
			selectionModel()->clearSelection();

			/* Repaint the viewport */
			viewport()->repaint();
		}
	}

	/* Shift Modifier */
	else if ( qApp->keyboardModifiers() & Qt::ShiftModifier ) {
		/* Current Index */
		QModelIndex cIdx = currentIndex();

		/* Valid index */
		if ( idx.isValid() ) {
			/* Forward selection */
			if ( mSelStartIdx.row() < idx.row() ) {
				mSelectedIndexes.clear();
				for( int i = mSelStartIdx.row(); i <= idx.row(); i++ )
					mSelectedIndexes << cModel->index( i, 0, rootIndex() );
			}

			/* Reverse selection */
			else {
				mSelectedIndexes.clear();
				for( int i = idx.row(); i <= mSelStartIdx.row(); i++ )
					mSelectedIndexes << cModel->index( i, 0, rootIndex() );
			}

			/* Repaint the viewport */
			viewport()->repaint();
		}

		/* Valid category */
		else if ( categoryAt( mpEvent->pos() ).count() ) {
			setCategorySelected( categoryAt( mpEvent->pos() ), true );
			viewport()->repaint();
		}

		/* Click on empty space */
		else {
		}
	}

	/* Control Modifier */
	else if ( qApp->keyboardModifiers() & Qt::ControlModifier ) {
		/* Valid index */
		if ( idx.isValid() ) {
			if ( mSelectedIndexes.contains( idx ) )
				mSelectedIndexes.removeAll( idx );

			else
				mSelectedIndexes << idx;

			viewport()->repaint();
		}

		/* Valid category */
		else if ( categoryAt( mpEvent->pos() ).count() ) {
			setCategorySelected( categoryAt( mpEvent->pos() ), false );
			viewport()->repaint();
		}

		/* Click on empty space */
		else {
		}
	}

	else {

	}

	/* Set the clicked index as the current index */
	if( idx.isValid() ) {
		setCurrentIndex( idx );
	}

	mpEvent->accept();
};

void PBIconView::mouseMoveEvent( QMouseEvent *mmEvent ) {

	/* Left Mouse Button is pressed down */
	if ( mmEvent->buttons() & Qt::LeftButton ) {
		/* Check for drag action: if the index at @dragStartPosition is selected, then its a drag */
		/* If there is no index at the dragStart position, then we start the selection */

		/* Index at @dragStartPosition */
		QModelIndex dIdx;
		if ( dragStartPosition.isNull() )
			dIdx = QModelIndex();

		else
			dIdx = indexAt( dragStartPosition );

		/* Start and execute the drag */
		if ( dIdx.isValid() and mSelectedIndexes.contains( dIdx ) ) {
			qDebug() << "Dragging";
			QDrag *drag = new QDrag( this );

			QList<QUrl> urlList;

			/* Selection based on mSelectedIndexes */
			Q_FOREACH( QModelIndex idx, mSelectedIndexes ) {
				if ( not idx.column() )
					urlList << QUrl::fromLocalFile( cModel->nodePath( idx ) );
			}

			/* Selection based on selectedModel */
			Q_FOREACH( QModelIndex idx, selectionModel()->selectedRows() ) {
				QUrl url = QUrl::fromLocalFile( cModel->nodePath( idx ) );
				if ( not urlList.contains( url ) )
					urlList << url;
			}

			QMimeData *mimedata = new QMimeData();
			mimedata->setUrls( urlList );

			drag->setMimeData( mimedata );

			/* Set the view state to drag */
			setState( QAbstractItemView::DraggingState );
			drag->exec( Qt::CopyAction | Qt::MoveAction | Qt::LinkAction );
			setState( QAbstractItemView::NoState );

			viewport()->repaint();
		}

		/* Start the selection, clear it if necessary */
		else {
			if ( indexAt( mmEvent->pos() ).isValid() ) {
				QModelIndex idx = indexAt( mmEvent->pos() );
				if ( not mSelectedIndexes.contains( idx ) )
						mSelectedIndexes << idx;
			}

			viewport()->repaint();
		}

		mmEvent->accept();
	}

	/* As the mouse button is nearing the left edge, start exposing the sidepanel */
	if ( mmEvent->pos().x() <= 48 )
		emit showSidePanel();

	else
		emit hideSidePanel();

	if ( ( width() - mmEvent->pos().x() ) <= 64 )
		emit showInfoPanel();

	else
		emit hideInfoPanel();

	viewport()->repaint();
	QAbstractItemView::mouseMoveEvent( mmEvent );
};

void PBIconView::mouseDoubleClickEvent( QMouseEvent *mEvent ) {

	if ( mEvent->button() == Qt::LeftButton ) {
		QModelIndex idx = indexAt( mEvent->pos() );
		if ( idx.isValid() ) {
			switch( cModel->modelDataType() ) {
				case PBItemViewModel::SuperStart: {

					/* If we are showing a drive file (Computer) */
					if ( not idx.data( Qt::UserRole + 6 ).toString().count() )
						emit open( idx.data( Qt::UserRole + 2 ).toString() );

					/* If we are showing a place */
					else
						emit open( idx );

					break;
				}

				case PBItemViewModel::Catalogs: {

					emit open( idx.data( Qt::UserRole + 7 ).toString() );
					break;
				}

				case PBItemViewModel::FileSystem: {

					emit open( idx );
					break;
				}
			}
		}

		else if ( not categoryAt( mEvent->pos() ).isEmpty() ) {
			showHideCategory( categoryAt( mEvent->pos() ) );
		}
	}

	mEvent->accept();
};

void PBIconView::dragEnterEvent( QDragEnterEvent *deEvent ) {

	deEvent->acceptProposedAction();
};

void PBIconView::dragMoveEvent( QDragMoveEvent *dmEvent ) {

	/* As the mouse button is nearing the left edge, start exposing the sidepanel */
	if ( dmEvent->pos().x() <= 48 )
		emit showSidePanel();

	else
		emit hideSidePanel();

	if ( ( width() - dmEvent->pos().x() ) <= 300 )
		emit showInfoPanel();

	else
		emit hideInfoPanel();

	const QMimeData *mData = dmEvent->mimeData();
	if ( not mData->hasUrls() )
		return;

	QString source = dirName( mData->urls().at( 0 ).toLocalFile() );
	source += ( source.endsWith( "/" ) ? "" : "/" );

	/* Incoming drop from else where */
	if ( source != cModel->currentDir() ) {
		switch ( cModel->modelDataType() ) {
			case PBItemViewModel::SuperStart: {
				QModelIndex idx = indexAt( dmEvent->pos() );
				if ( idx.isValid() ) {
					if ( not isWritable( cModel->nodePath( idx ) ) or not isDir( cModel->nodePath( idx ) ) )
						return;

					dmEvent->setDropAction( Qt::CopyAction );
					dmEvent->accept();
				}

				break;
			}

			case PBItemViewModel::FileSystem: {
				if ( isWritable( cModel->currentDir() ) ) {
					dmEvent->setDropAction( Qt::CopyAction );
					dmEvent->accept();
				}

				break;
			}

			default : {
				dmEvent->ignore();
				break;
			}
		}
	}

	/* drag and drop in the current folder */
	else if ( indexAt( dmEvent->pos() ).isValid() ) {
		QModelIndex idx = indexAt( dmEvent->pos() );
		QString mtpt = cModel->nodePath( idx );

		Q_FOREACH( QUrl url, mData->urls() ) {
			if ( url.toLocalFile() == mtpt ) {
				dmEvent->ignore();
				return;
			}
		}

		if ( isWritable( mtpt ) and isDir( mtpt ) ) {
			dmEvent->setDropAction( Qt::CopyAction );
			dmEvent->accept();
		}

		else
			dmEvent->ignore();
	}

	else {

		dmEvent->ignore();
	}
};

void PBIconView::dropEvent( QDropEvent *dpEvent ) {

	QModelIndex idx = indexAt( dpEvent->pos() );
	QString mtpt = cModel->nodePath( idx );

	if ( not isDir( mtpt ) ) {
		dpEvent->ignore();
		return;
	}

	else if ( isDir( mtpt ) and not isWritable( mtpt ) ) {
		dpEvent->ignore();
		return;
	}

	const QMimeData *mData = dpEvent->mimeData();
	if ( mData->hasUrls() ) {

		QStringList args;
		foreach( QUrl url, mData->urls() )
			args << url.toLocalFile();

		if ( dpEvent->keyboardModifiers() == Qt::NoModifier ) {
			args << mtpt;

			QMenu *menu = new QMenu( this );

			QAction *mvAct = new QAction( QIcon::fromTheme( "go-jump" ), QString( "&Move Here" ), this );
			mvAct->setData( QVariant( args ) );
			connect( mvAct, SIGNAL( triggered() ), this, SLOT( emitCML() ) );

			QAction *cpAct = new QAction( QIcon::fromTheme( "edit-copy" ), QString( "&Copy Here" ), this );
			cpAct->setData( QVariant( args ) );
			connect( cpAct, SIGNAL( triggered() ), this, SLOT( emitCML() ) );

			QAction *lnAct = new QAction( QIcon::fromTheme( "edit-link" ), QString( "&Link Here" ), this );
			lnAct->setData( QVariant( args ) );
			connect( lnAct, SIGNAL( triggered() ), this, SLOT( emitCML() ) );

			QAction *cancelAct = new QAction( QIcon::fromTheme( "dialog-close" ), QString( "Cancel" ), this );
			connect( cancelAct, SIGNAL( triggered() ), menu, SLOT( close() ) );

			menu->addAction( mvAct );
			menu->addAction( cpAct );
			menu->addAction( lnAct );
			menu->addSeparator();
			menu->addAction( cancelAct );

			menu->exec( mapToGlobal( dpEvent->pos() ) );
			dpEvent->accept();
		}

		else if ( dpEvent->keyboardModifiers() == Qt::ShiftModifier ) {

			emit move( args, mtpt );
		}

		else if ( dpEvent->keyboardModifiers() == Qt::ControlModifier ) {

			emit copy( args, mtpt );
		}

		else if ( dpEvent->keyboardModifiers() == ( Qt::ControlModifier | Qt::ShiftModifier ) ) {

			emit link( args, mtpt );
		}

		else {

			dpEvent->ignore();
			return;
		}
	}

	dpEvent->accept();
};

void PBIconView::keyPressEvent( QKeyEvent *kEvent ) {

	QList<int> retKeys = QList<int>() << Qt::Key_Return << Qt::Key_Enter;

	if ( retKeys.contains( kEvent->key() ) and ( selectionModel()->isSelected( currentIndex() ) ) ) {

		QModelIndex idx = currentIndex();
		switch( cModel->modelDataType() ) {

			case PBItemViewModel::SuperStart: {
				if ( not idx.data( Qt::UserRole + 6 ).toString().count() )
					emit open( idx.data( Qt::UserRole + 2 ).toString() );

				else
					emit open( idx );

				break;
			}

			case PBItemViewModel::Catalogs: {

				emit open( idx.data( Qt::UserRole + 7 ).toString() );
				break;
			}

			case PBItemViewModel::FileSystem: {

				emit open( idx );
				break;
			}
		}
	}

	QAbstractItemView::keyPressEvent( kEvent );
};

void PBIconView::computeGridSize( QSize iconSize ) {

	/*
		* width: 3 * iconSize
		* height: iconSize + iconSize * 2
	*/
	myGridSizeMin = QSize( qMin( 256, qMax( 144, iconSize.width() * 3 ) ), qMax( iconSize.height() + 21, iconSize.height() * 2 ) );

	computeRowsAndColumns();

	hashIsDirty = true;
	calculateRectsIfNecessary();
};

QModelIndex PBIconView::moveCursorCategorized( QAbstractItemView::CursorAction cursorAction ) {

	/* Clear mouse selection */
	mSelectedIndexes.clear();
	mSelectedCategories.clear();

	/* Current Index */
	QModelIndex idx = currentIndex();

	/* If there exists a current index */
	if ( idx.isValid() ) {

		switch( cursorAction ) {
			case QAbstractItemView::MoveNext:
			case QAbstractItemView::MoveRight:
				emit selectionChanged();
				return nextIndex();

			case QAbstractItemView::MovePrevious:
			case QAbstractItemView::MoveLeft:
				emit selectionChanged();
				return prevIndex();

			case QAbstractItemView::MoveDown: {

				emit selectionChanged();
				return belowIndex();
			}

			case QAbstractItemView::MoveUp: {

				emit selectionChanged();
				return aboveIndex();
			}

			case QAbstractItemView::MoveHome: {

				emit selectionChanged();
				return firstIndex();
			}

			case QAbstractItemView::MoveEnd: {

				emit selectionChanged();
				return lastIndex();
			}

			case QAbstractItemView::MovePageUp: {

				emit selectionChanged();
				return indexPageAbove();
			}

			case QAbstractItemView::MovePageDown: {

				emit selectionChanged();
				return indexPageBelow();
			}

			default: {
				emit selectionChanged();
				return QModelIndex();
			}
		}
	}

	/* If there is no current index */
	else {
		switch( cursorAction ) {
			case QAbstractItemView::MoveHome:
			case QAbstractItemView::MoveRight:
			case QAbstractItemView::MoveNext:
			case QAbstractItemView::MoveDown:
				emit selectionChanged();
				return firstIndex();

			case QAbstractItemView::MoveEnd:
			case QAbstractItemView::MoveLeft:
			case QAbstractItemView::MovePrevious:
			case QAbstractItemView::MoveUp:
				emit selectionChanged();
				return firstIndex();

			default:
				emit selectionChanged();
				return firstIndex();
		}
	}

	emit selectionChanged();
	return idx;
};

void PBIconView::setCategorySelected( QString category, bool yes ) {

	if ( yes ) {
		Q_FOREACH( QModelIndex idx, cModel->indexListForCategory( category ) ) {
			if ( not mSelectedIndexes.contains( idx ) )
				mSelectedIndexes << idx;
		}
	}

	else {
		Q_FOREACH( QModelIndex idx, cModel->indexListForCategory( category ) )
			mSelectedIndexes.removeAll( idx );
	}
};

QModelIndex PBIconView::nextIndex() {

	QModelIndex idx = currentIndex();

	/* The next index is in the current category */
	int nextIdx = curIdx.second + 1;
	if ( nextIdx < cModel->indexListCountForCategory( curIdx.first ) ) {
		QString curCat = categoryList.at( curIdx.first );

		/* If the current category is not folded, just return the next index */
		if ( not foldedCategories.contains( curCat ) ) {
			curIdx = qMakePair( curIdx.first, nextIdx );
			persistentVCol = nextIdx % itemsPerRow;
			return cModel->indexListForCategory( curIdx.first )[ nextIdx ];
		}

		/* If the current category is folded, and the next index in in the first row of the current category */
		else if ( foldedCategories.contains( curCat ) and nextIdx < ( itemsPerRow - 1 ) ) {
			curIdx = qMakePair( curIdx.first, nextIdx );
			persistentVCol = nextIdx % itemsPerRow;
			return cModel->indexListForCategory( curIdx.first )[ nextIdx ];
		}
	}

	/* The next index is the first index of the next visible category */
	int nCatIdx = curIdx.first < categoryList.count() - 1 ? curIdx.first + 1 : 0;
	while ( ( nCatIdx < categoryList.count() ) and ( nCatIdx != curIdx.first ) ) {
		if ( not hiddenCategories.contains( categoryList.at( nCatIdx ) ) )
			break;

		nCatIdx += 1;
		if ( nCatIdx == categoryList.count() )
			nCatIdx = 0;
	}

	curIdx = qMakePair( nCatIdx, 0 );
	persistentVCol = 0;
	return cModel->indexListForCategory( nCatIdx ).first();

	return QModelIndex();
};

QModelIndex PBIconView::prevIndex() {

	QModelIndex idx = currentIndex();

	/* The previous index is in the current category */
	int prevIdx = curIdx.second - 1;
	if ( prevIdx >= 0 ) {
		curIdx = qMakePair( curIdx.first, prevIdx );
		persistentVCol = prevIdx % itemsPerRow;
		return cModel->indexListForCategory( curIdx.first )[ prevIdx ];
	}

	/* The prev index is the last index of the previous visible category */
	int pCatIdx = curIdx.first > 0 ? curIdx.first - 1 : categoryList.count() - 1;
	while ( ( pCatIdx < categoryList.count() ) and ( pCatIdx != curIdx.first ) ) {
		if ( not hiddenCategories.contains( categoryList.at( pCatIdx ) ) )
			break;

		pCatIdx -= 1;
		if ( pCatIdx == -1 )
			pCatIdx = categoryList.count() - 1;
	}

	QString prevCat = categoryList.at( pCatIdx );
	/* If the previous category is folded */
	if ( foldedCategories.contains( prevCat ) ) {
		curIdx = qMakePair( pCatIdx, itemsPerRow - 2 );
		persistentVCol = itemsPerRow - 2;
		return cModel->indexListForCategory( pCatIdx )[ itemsPerRow - 2 ];
	}

	/* The category is not folded */
	else {
		QModelIndexList catIdxes = cModel->indexListForCategory( pCatIdx );
		curIdx = qMakePair( pCatIdx, catIdxes.count() - 1 );
		persistentVCol = ( catIdxes.count() - 1 ) % itemsPerRow;
		return catIdxes.last();
	}

	return QModelIndex();
};

QModelIndex PBIconView::belowIndex() {

	QModelIndex idx = currentIndex();

	/* If the current category is not folded, just return the next index */
	if ( not foldedCategories.contains( cModel->category( idx ) ) ) {
		/* The next index is in the current category */
		int nextIdx = curIdx.second + itemsPerRow;
		if ( nextIdx <= cModel->indexListCountForCategory( curIdx.first ) - 1 ) {

			curIdx = qMakePair( curIdx.first, nextIdx );
			return cModel->indexListForCategory( curIdx.first )[ nextIdx ];
		}

		/* If the number of rows is greater than the current row of the current index */
		else {
			int curRow = ceil( ( 1.0 + curIdx.second ) / itemsPerRow );
			int catRows = ceil( 1.0 * cModel->indexListCountForCategory( curIdx.first ) / itemsPerRow );
			if ( catRows > curRow and curRow != 0 ) {
				curIdx = qMakePair( curIdx.first, cModel->indexListCountForCategory( curIdx.first ) - 1 );
				return cModel->indexListForCategory( curIdx.first ).last();
			}
		}
	}

	/* The next index is in the next visible category */
	int nCatIdx = curIdx.first < categoryList.count() - 1 ? curIdx.first + 1 : 0;
	while ( ( nCatIdx < categoryList.count() ) and ( nCatIdx != curIdx.first ) ) {
		if ( not hiddenCategories.contains( categoryList.at( nCatIdx ) ) )
			break;

		nCatIdx += 1;
		if ( nCatIdx == categoryList.count() )
			nCatIdx = 0;
	}

	/* The next category is folded */
	if ( foldedCategories.contains( categoryList.at( nCatIdx ) ) ) {
		if ( persistentVCol <= itemsPerRow - 2 ) {
			curIdx = qMakePair( nCatIdx, persistentVCol );
			return cModel->indexListForCategory( nCatIdx ).at( persistentVCol );
		}

		else {
			curIdx = qMakePair( nCatIdx, itemsPerRow - 2 );
			return cModel->indexListForCategory( nCatIdx ).at( itemsPerRow - 2 );
		}
	}

	if ( persistentVCol < cModel->indexListCountForCategory( nCatIdx ) - 1 ) {
		curIdx = qMakePair( nCatIdx, persistentVCol );
		return cModel->indexListForCategory( nCatIdx ).at( persistentVCol );
	}

	else {
		curIdx = qMakePair( nCatIdx, cModel->indexListCountForCategory( nCatIdx ) - 1 );
		return cModel->indexListForCategory( nCatIdx ).last();
	}

	return QModelIndex();
};

QModelIndex PBIconView::aboveIndex() {

	QModelIndex idx = currentIndex();

	/* The previous index is in the current category */
	/* Note that if this category is folded, the next block will have already put the
	 * current index in the first row of this category. */
	int curRow = ceil( curIdx.second / itemsPerRow );
	if ( curRow > 0 ) {
		int nIdx = ( curRow - 1 ) * itemsPerRow + persistentVCol;
		curIdx = qMakePair( curIdx.first, nIdx );
		return cModel->indexListForCategory( curIdx.first ).at( nIdx );
	}

	/* The previous index is in the previous category */
	int pCatIdx = curIdx.first > 0 ? curIdx.first - 1 : categoryList.count() - 1;
	while ( ( pCatIdx < categoryList.count() ) and ( pCatIdx != curIdx.first ) ) {
		if ( not hiddenCategories.contains( categoryList.at( pCatIdx ) ) )
			break;

		pCatIdx -= 1;
		if ( pCatIdx == -1 )
			pCatIdx = categoryList.count() - 1;
	}

	/* The prev index is the last visible index of the previous visible category */
	QString prevCat = categoryList.at( pCatIdx );
	if ( foldedCategories.contains( prevCat ) ) {
		if ( persistentVCol <= cModel->indexListCountForCategory( prevCat ) - 1 ) {
			if ( persistentVCol <= itemsPerRow - 2 ) {
				curIdx = qMakePair( pCatIdx, persistentVCol );
				return cModel->indexListForCategory( prevCat ).at( persistentVCol );
			}

			else {
				curIdx = qMakePair( pCatIdx, itemsPerRow - 2 );
				return cModel->indexListForCategory( prevCat ).at( itemsPerRow - 2 );
			}
		}

		/* Safegaurd: This should never happen */
		else {
			curIdx = qMakePair( pCatIdx, cModel->indexListCountForCategory( pCatIdx ) - 1 );
			return cModel->indexListForCategory( pCatIdx ).last();
		}
	}

	/* Last index of the previous category is the above index */
	int lIdxCol = cModel->indexListCountForCategory( prevCat ) % itemsPerRow;
	if ( lIdxCol <= persistentVCol and lIdxCol != 0 ) {
		curIdx = qMakePair( pCatIdx, cModel->indexListCountForCategory( pCatIdx ) - 1 );
		return cModel->indexListForCategory( pCatIdx ).last();
	}

	else {
		int nRows = ceil( 1.0 * cModel->indexListCountForCategory( prevCat ) / itemsPerRow );
		int pIdx = ( nRows - 1 ) * itemsPerRow + persistentVCol;
		curIdx = qMakePair( pCatIdx, pIdx );
		return cModel->indexListForCategory( pCatIdx ).at( pIdx );
	}

	return QModelIndex();
};

QModelIndex PBIconView::firstIndex() {

	persistentVCol = 0;

	int firstVisibleCatIdx = -1;
	Q_FOREACH( QString category, categoryList ) {
		if ( not hiddenCategories.contains( category ) ) {
			firstVisibleCatIdx = categoryList.indexOf( category );
			break;
		}
	}

	curIdx = qMakePair( firstVisibleCatIdx, 0 );

	verticalScrollBar()->setValue( 0 );

	if ( firstVisibleCatIdx >= 0 )
		return cModel->indexListForCategory( categoryList.at( firstVisibleCatIdx ) ).first();

	else
		return QModelIndex();
};

QModelIndex PBIconView::lastIndex() {

	int lastVisibleCatIdx = -1;
	for( lastVisibleCatIdx = categoryList.count() - 1; lastVisibleCatIdx >= 0; lastVisibleCatIdx-- ) {
		if ( not hiddenCategories.contains( categoryList.at( lastVisibleCatIdx ) ) )
			break;
	}

	verticalScrollBar()->setValue( verticalScrollBar()->maximum() );

	int categoryNodeCount = cModel->indexListCountForCategory( lastVisibleCatIdx );

	if ( foldedCategories.contains( categoryList.at( lastVisibleCatIdx ) ) ) {
		persistentVCol = ( categoryNodeCount < itemsPerRow ? categoryNodeCount : itemsPerRow - 2 );
		curIdx = qMakePair( lastVisibleCatIdx, persistentVCol );
	}

	else {
		persistentVCol = ( categoryNodeCount - 1 ) % itemsPerRow;
		curIdx = qMakePair( lastVisibleCatIdx, categoryNodeCount - 1 );
	}

	if ( lastVisibleCatIdx >= 0 )
		return cModel->indexListForCategory( categoryList.at( lastVisibleCatIdx ) ).at( curIdx.second );

	return QModelIndex();
};

QModelIndex PBIconView::indexPageBelow() {

	QModelIndex idx = currentIndex();
	QScrollBar *bar = verticalScrollBar();
	bar->setValue( bar->value() + bar->pageStep() );

	QModelIndex nIdx;
	for ( int row = idx.row(); row < cModel->rowCount( rootIndex() ); row++ ) {
		QModelIndex idx = cModel->index( row, 0, rootIndex() );
		if ( not canShowIndex( idx ) )
			continue;

		QRect rect = viewportRectForRow( row );
		if ( !rect.isValid() || rect.bottom() < 0 || rect.y() > viewport()->height() )
			continue;

		nIdx =  cModel->index( row, 0, rootIndex() );
	}

	QString cCategory = cModel->category( idx );
	QModelIndexList cIdxList = cModel->indexListForCategory( cCategory );

	/* Compute the number of rows in the current category */
	int cRows = cIdxList.count() / itemsPerRow;
	if ( cIdxList.count() % itemsPerRow )
		cRows++;

	/* Check which row are we in */
	int cRow = ( 1 + cIdxList.indexOf( idx ) ) / itemsPerRow;
	if ( ( 1 + cIdxList.indexOf( idx ) ) % itemsPerRow )
		cRow++;

	if ( ( cRow - 1 ) * itemsPerRow + persistentVCol < cIdxList.count() )
		return cIdxList.at( ( cRow - 1 ) * itemsPerRow + persistentVCol );

	else
		return cIdxList.last();
};

QModelIndex PBIconView::indexPageAbove() {

	return QModelIndex();
};

QModelIndex PBIconView::moveCursorNonCategorized( QAbstractItemView::CursorAction cursorAction ) {

	QModelIndex idx = currentIndex();
	if ( idx.isValid() ) {

		switch( cursorAction ) {
			case QAbstractItemView::MoveRight: {
				/* If the current index is not the last visible index */
				if ( idx.row() >= 0 and idx.row() < cModel->rowCount() - 1 ) {
					persistentVCol = ( idx.row() + 1 ) % itemsPerRow;
					emit selectionChanged();
					return cModel->index( idx.row() + 1, 0, rootIndex() );
				}

				/* Current index is the last visible index */
				else {
					persistentVCol = 0;
					emit selectionChanged();
					return cModel->index( 0, 0, rootIndex() );
				}
			}

			case QAbstractItemView::MoveLeft: {
				/* The current index is anything but the first one */
				if ( idx.row() > 0 and idx.row() < cModel->rowCount() ) {
					persistentVCol = ( idx.row() - 1 ) % itemsPerRow;
					emit selectionChanged();
					return cModel->index( idx.row() - 1, 0, rootIndex() );
				}

				/* The current index is the first one */
				else {
					persistentVCol = ( cModel->rowCount() - 1 ) % itemsPerRow;
					emit selectionChanged();
					return cModel->index( cModel->rowCount() - 1, 0, rootIndex() );
				}
			}

			case QAbstractItemView::MoveDown: {

				int nextRow = idx.row() + itemsPerRow;
				if ( nextRow >= cModel->rowCount() ) {
					emit selectionChanged();
					return cModel->index( cModel->rowCount() - 1, 0, rootIndex() );
				}

				else {
					emit selectionChanged();
					return cModel->index( nextRow, 0, rootIndex() );
				}
			}

			case QAbstractItemView::MoveUp: {

				int prevRow = idx.row() - itemsPerRow;
				if ( prevRow < 0 ) {
					emit selectionChanged();
					return cModel->index( 0, 0, rootIndex() );
				}

				else {
					emit selectionChanged();
					return cModel->index( prevRow, 0, rootIndex() );
				}
			}

			case QAbstractItemView::MoveHome: {

				persistentVCol = 0;
				verticalScrollBar()->setValue( 0 );
				emit selectionChanged();
				return cModel->index( 0, 0, idx.parent() );
			}

			case QAbstractItemView::MoveEnd: {

				persistentVCol = ( cModel->rowCount() - 1 ) % itemsPerRow;
				emit selectionChanged();
				return cModel->index( cModel->rowCount() - 1, 0, idx.parent() );
			}

			case QAbstractItemView::MoveNext: {
				/* If the current index is not the last visible index */
				if ( idx.row() >= 0 and idx.row() < cModel->rowCount() - 1 ) {
					emit selectionChanged();
					return cModel->index( idx.row() + 1, 0, rootIndex() );
				}

				/* Current index is the last visible index */
				else {
					emit selectionChanged();
					return cModel->index( 0, 0, rootIndex() );
				}
			}

			case QAbstractItemView::MovePrevious: {

				/* The current index is anything but the first one */
				if ( idx.row() > 0 and idx.row() < cModel->rowCount() ) {
					emit selectionChanged();
					return cModel->index( idx.row() - 1, 0, rootIndex() );
				}

				/* The current index is the first one */
				else {
					emit selectionChanged();
					return cModel->index( cModel->rowCount() - 1, 0, rootIndex() );
				}
			}

			case QAbstractItemView::MovePageUp: {
				int viewportHeight = viewport()->height();
				int rowsInVisualArea = viewportHeight / myGridSize.height();
				int itemsInVisualArea = rowsInVisualArea * itemsPerRow;

				int prevIdx = idx.row() - itemsInVisualArea;
				if ( not cModel->index( prevIdx, 0, rootIndex() ).isValid() ) {
					emit selectionChanged();
					return cModel->index( 0, 0, rootIndex() );
				}

				emit selectionChanged();
				return cModel->index( prevIdx, 0, rootIndex() );
			}

			case QAbstractItemView::MovePageDown: {
				int viewportHeight = viewport()->height();
				int rowsInVisualArea = viewportHeight / myGridSize.height();
				int itemsInVisualArea = rowsInVisualArea * itemsPerRow;

				int nextIdx = idx.row() + itemsInVisualArea;
				if ( not cModel->index( nextIdx, 0, rootIndex() ).isValid() ) {
					emit selectionChanged();
					return cModel->index( cModel->rowCount() - 1, 0, rootIndex() );
				}

				emit selectionChanged();
				return cModel->index( nextIdx, 0, rootIndex() );
			}

			default: {
				emit selectionChanged();
				return cModel->index( 0, 0, rootIndex() );
			}
		}
	}

	else {
		switch( cursorAction ) {
			case QAbstractItemView::MoveHome: {

				persistentVCol = 0;
				emit selectionChanged();
				return cModel->index( 0, 0, idx.parent() );
			}

			case QAbstractItemView::MoveEnd: {

				persistentVCol = ( cModel->rowCount() - 1 ) % itemsPerRow;
				emit selectionChanged();
				return cModel->index( cModel->rowCount() - 1, 0, idx.parent() );
			}

			default: {

				emit selectionChanged();
				return cModel->index( 0, 0, rootIndex() );
			}
		}
	}

	emit selectionChanged();
	return idx;
};

void PBIconView::calculateRectsIfNecessary() const {

	if ( not hashIsDirty )
		return;

	QSettings sett( cModel->currentDir() + ".directory", QSettings::NativeFormat );
	hiddenCategories = sett.value( "PiBreeze/HiddenCategories", QStringList() ).toStringList();
	foldedCategories = sett.value( "PiBreeze/FoldedCategories", QStringList() ).toStringList();

	computeRowsAndColumns();

	rectForRow.clear();
	rectForCategory.clear();

	if ( cModel->isCategorizationEnabled() )
		calculateCategorizedRects();

	else
		calculateNonCategorizedRects();

	if ( idealHeight <= viewport()->height() ) {

		verticalScrollBar()->setRange( 0, 0 );
		verticalScrollBar()->hide();
	}

	else {
		verticalScrollBar()->show();

		verticalScrollBar()->setSingleStep( myGridSize.height() );
		verticalScrollBar()->setPageStep( viewport()->height() );
		verticalScrollBar()->setRange( 0, qMax( 0, idealHeight - viewport()->height() ) );
	}
};

void PBIconView::calculateCategorizedRects() const {

	int x = 0, y = 0, prevRows = 0, totalRows = 0;

	for( int catIdx = 0; catIdx < categoryList.count(); catIdx++ ) {
		QModelIndexList mList = cModel->indexListForCategory( categoryList.at( catIdx ) );

		// Minimum X and Y for Category Rectangle
		int minX = myContentsMargins.left();
		int minY = myContentsMargins.top() + catIdx * myCategoryHeight + catIdx * myCategorySpacing + totalRows * myGridSize.height();

		/* Extra Spacing in SuperStart */
		if ( cModel->modelDataType() == PBItemViewModel::SuperStart )
			minY += catIdx * myGridSize.height() / 3;


		int categoryWidth = viewport()->width() - myContentsMargins.left() - myContentsMargins.right();
		rectForCategory[ catIdx ] = QRect( minX, minY, categoryWidth, 24 );

		// We consider the space reserved for the category but not the indexes listed under it
		if ( hiddenCategories.contains( cModel->category( mList.value( 0 ) ) ) )
			continue;

		// We consider the space reserved for the category and one row of indexes
		else if ( foldedCategories.contains( cModel->category( mList.value( 0 ) ) ) ) {
			// Mimimum X and Y for indexes
			minX += myInlayMargins.left();
			minY += myCategoryHeight;

			totalRows++;
			int limit = mList.count() >= itemsPerRow ? itemsPerRow - 1 : mList.count();

			for( int lrow = 0; lrow < limit; lrow++ ) {
				int row = lrow / itemsPerRow;
				int col = lrow % itemsPerRow;

				x = minX + col * myGridSize.width();
				y = minY + row * myGridSize.height();

				rectForRow[ mList[ lrow ].row() ] = QPoint( x, y );
			}
		}

		else {
			// Mimimum X and Y for indexes
			minX += myInlayMargins.left();
			minY += myCategoryHeight;

			prevRows = mList.count() / itemsPerRow;
			if ( mList.count() % itemsPerRow )
				prevRows++;

			totalRows += prevRows;

			for( int lrow = 0; lrow < mList.count(); lrow++ ) {
				int row = lrow / itemsPerRow;
				int col = lrow % itemsPerRow;

				x = minX + col * myGridSize.width();
				y = minY + row * myGridSize.height();

				rectForRow[ mList[ lrow ].row() ] = QPoint( x, y );
			}
		}
	}

	idealHeight = myContentsMargins.top() + myContentsMargins.bottom() + categoryList.count() * ( myCategoryHeight + myCategorySpacing );
	idealHeight += totalRows * myGridSize.height();

	if ( cModel->modelDataType() == PBItemViewModel::SuperStart )
		idealHeight += categoryList.count() * myGridSize.height();

	hashIsDirty = false;
	viewport()->update();
};

void PBIconView::calculateNonCategorizedRects() const {

	int x = 0, y = 0, totalRows = 0;

	for( int lrow = 0; lrow < cModel->rowCount(); lrow++ ) {

		totalRows = ( lrow + 1 ) / itemsPerRow;
		if ( ( lrow + 1 ) % itemsPerRow )
			totalRows++;

		// Minimum X and Y for Category Rectangle
		int minX = myContentsMargins.left();
		int minY = myContentsMargins.top() + ( totalRows - 1 ) * myGridSize.height();

		int col = lrow % itemsPerRow;

		x = minX + col * myGridSize.width() + 5;
		y = minY + 5;

		rectForRow[ lrow ] = QPoint( x, y );
	}

	idealHeight = myContentsMargins.top() + numberOfRows * myGridSize.height() + myContentsMargins.bottom();

	hashIsDirty = false;
	viewport()->update();
};

void PBIconView::computeRowsAndColumns() const {

	int vWidth = viewport()->width() - myContentsMargins.left() - myContentsMargins.right();
	vWidth = vWidth - myInlayMargins.left() - myInlayMargins.right();

	itemsPerRow = qMax( 1, vWidth / myGridSizeMin.width() );
	numberOfRows = ( int )( cModel->rowCount() / itemsPerRow );

	if ( ( cModel->rowCount() % itemsPerRow ) > 0 )
		numberOfRows++;

	int newGridWidth = ( int )( vWidth / itemsPerRow );
	myGridSize = QSize( newGridWidth, myGridSizeMin.height() );
};

QRect PBIconView::viewportRectForRow( int row ) const {

	calculateRectsIfNecessary();
	QPoint pt = rectForRow.value( row );
	if ( !QRect( pt, myGridSize ).isValid() )
		return QRect();

    return QRect( pt.x(), pt.y() - verticalScrollBar()->value(), myGridSize.width(), myGridSize.height() );
};

void PBIconView::paintCategory( QPainter *painter, const QRect &rectangle, const QString &text ) const {

	painter->save();

	QColor textColor = painter->pen().color();

	QLinearGradient hLine( rectangle.topLeft(), rectangle.topRight() );
	hLine.setColorAt( 0, textColor );
	hLine.setColorAt( 1, Qt::transparent );

	QBrush brush( hLine );
	QPen hPen( brush, 1.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );

	painter->setPen( hPen );
	painter->drawLine( rectangle.topLeft(), rectangle.topRight() );

	painter->setPen( QPen( textColor ) );

	QPixmap pix = cModel->pixmapForCategory( text, hiddenCategories.contains( text ) );
	QPoint topLeft = rectangle.topLeft();

	painter->drawPixmap( topLeft.x() + 4, topLeft.y() + 4, 16, 16, pix );
	QFont categoryFont = qApp->font();
	categoryFont.setBold( true );
	painter->setFont( categoryFont );
	painter->drawText( topLeft.x() + 24, topLeft.y(), rectangle.width() - 48, rectangle.height(), Qt::AlignVCenter, text );

	painter->restore();
};

void PBIconView::paintFolded( QPainter *painter, const QRect &rect, const QModelIndex &idx ) {

	painter->save();
	int n = cModel->indexListCountForCategory( cModel->category( idx ) ) - itemsPerRow + 1;

	QPixmap pix( ":/icons/plus.png" );
	pix = pix.scaled( myGridSize, Qt::KeepAspectRatio, Qt::SmoothTransformation );

	QRect r = QRect( rect.x() + abs( rect.width() - pix.width() ) / 2, rect.y() + abs( rect.height() - pix.height() ) / 2, pix.width(), pix.height() );
	painter->drawPixmap( r, pix );

	painter->setPen( Qt::black );
	painter->drawText( rect, Qt::AlignCenter | Qt::TextWordWrap, QString( "%1 more item%2" ).arg( n ).arg( n > 1 ? "s" : "" ) );
	painter->restore();
};

void PBIconView::zoomIn() {

	if ( myIconSize.width() >= 128 )
		setIconSize( QSize( 128, 128 ) );

	else
		setIconSize( myIconSize + QSize( 4, 4 ) );

	if ( cModel->isRealLocation() ) {

		QSettings sett( cModel->nodePath( ".directory" ), QSettings::NativeFormat );
		sett.setValue( "PiBreeze/IconSize", myIconSize.width() );
	}
};

void PBIconView::zoomOut() {

	if ( myIconSize.width() <= 16 )
		setIconSize( QSize( 16, 16 ) );

	else
		setIconSize( myIconSize - QSize( 4, 4 ) );

	if ( cModel->isRealLocation() ) {

		QSettings sett( cModel->nodePath( ".directory" ), QSettings::NativeFormat );
		sett.setValue( "PiBreeze/IconSize", myIconSize.width() );
	}
};

void PBIconView::emitCML() {

	QAction *act = qobject_cast<QAction*>( sender() );

	QStringList args = act->data().toStringList();
	QString mtpt = args.takeLast();

	if ( not mtpt.endsWith( "/" ) )
		mtpt += "/";

	if ( act->text().contains( "Copy" ) ) {

		PBProcess::Progress *progress = new PBProcess::Progress;
		progress->sourceDir = dirName( args.at( 0 ) );
		progress->targetDir = mtpt;

		QStringList srcList;
		foreach( QString path, args )
			srcList << path.replace( progress->sourceDir, "" );

		progress->type = PBProcess::Copy;

		PBIOProcess *proc = new PBIOProcess( srcList, progress );
		PBProcessManager::instance()->addProcess( progress, proc );

		progress->startTime = QTime::currentTime();

		proc->start();
	}

	else if ( act->text().contains( "Move" ) ) {

		PBProcess::Progress *progress = new PBProcess::Progress;
		progress->sourceDir = dirName( args.at( 0 ) );
		progress->targetDir = mtpt;

		QStringList srcList;
		foreach( QString path, args )
			srcList << path.replace( progress->sourceDir, "" );

		progress->type = PBProcess::Move;

		PBIOProcess *proc = new PBIOProcess( srcList, progress );
		PBProcessManager::instance()->addProcess( progress, proc );

		progress->startTime = QTime::currentTime();

		proc->start();
	}

	else {

		QStringList errorNodes;
		Q_FOREACH( QString path, args ) {
			if ( symlink( path.toLocal8Bit().data(), ( mtpt + baseName( path ) ).toLocal8Bit().data() ) != 0 ) {
				errorNodes << path;
				qDebug() << "Failed to create link for:" << baseName( path );
				qDebug() << "   " << strerror( errno );
			}
		}

		if ( errorNodes.count() ) {
			QString title = "PiBreeze - Error Creating links";
			QString text = QString(
				"Some errors were encountered while creating the symlinks you requested. "
				"They are listed in the table below. Please check the debug messages to determine the cause of the errors."
			);

			PBErrorsDialog *errDlg = new PBErrorsDialog( title, text, errorNodes, this );
			errDlg->exec();
		}
	}
};

void PBIconView::showHideCategory( QString category ) {

	QSettings dirSett( cModel->currentDir() + ".directory", QSettings::NativeFormat );
	QStringList hidden = dirSett.value( "PiBreeze/HiddenCategories", QStringList() ).toStringList();
	QStringList folded = dirSett.value( "PiBreeze/FoldedCategories", QStringList() ).toStringList();

	if ( hiddenCategories.contains( category ) ) {
		hiddenCategories.removeAll( category );
		hidden.removeAll( category );
		folded.removeAll( category );
	}

	else{
		hiddenCategories.append( category );
		hidden.append( category );
	}

	hidden.removeDuplicates();
	dirSett.setValue( "PiBreeze/HiddenCategories", hidden );
	dirSett.setValue( "PiBreeze/FoldedCategories", folded );
	dirSett.sync();

	hashIsDirty = true;
	calculateRectsIfNecessary();
};

void PBIconView::toggleFoldCategory( QString category ) {

	QSettings dirSett( cModel->currentDir() + ".directory", QSettings::NativeFormat );
	QStringList hidden = dirSett.value( "PiBreeze/HiddenCategories", QStringList() ).toStringList();
	QStringList folded = dirSett.value( "PiBreeze/FoldedCategories", QStringList() ).toStringList();

	/* If @category is hidden, show it folded */
	if ( hiddenCategories.contains( category ) ) {
		hidden.removeAll( category );
		hiddenCategories.removeAll( category );
		if ( cModel->indexListCountForCategory( category ) > itemsPerRow ) {
			foldedCategories.append( category );
			folded.append( category );
		}
	}

	else if ( foldedCategories.contains( category ) ) {
		hiddenCategories.removeAll( category );
		hidden.removeAll( category );

		foldedCategories.removeAll( category );
		folded.removeAll( category );
	}

	else{
		if ( cModel->indexListCountForCategory( category ) > itemsPerRow ) {
			foldedCategories.append( category );
			folded.append( category );
		}
	}

	folded.removeDuplicates();
	dirSett.setValue( "PiBreeze/HiddenCategories", hidden );
	dirSett.setValue( "PiBreeze/FoldedCategories", folded );
	dirSett.sync();

	hashIsDirty = true;
	calculateRectsIfNecessary();
};

bool PBIconView::canShowIndex( QModelIndex idx ) {

	QString category( cModel->category( idx ) );

	if ( hiddenCategories.contains( category ) )
		return false;

	else if ( foldedCategories.contains( category ) ) {
		QModelIndexList mList = cModel->indexListForCategory( category );
		if ( mList.indexOf( idx ) >= itemsPerRow - 1 )
			return false;

		else
			return true;
	}

	else
		return true;
};

void PBIconView::currentChanged( const QModelIndex &cur, const QModelIndex &prev ) {

	QAbstractItemView::currentChanged( cur, prev );

	curIdx = qMakePair( categoryList.indexOf( cModel->category( cur ) ), cModel->indexInCategory( cur ) );
	persistentVCol = cModel->indexInCategory( cur ) % itemsPerRow;
};
