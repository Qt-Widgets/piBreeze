/*
	*
	* PBDirectoryView.hpp - PiBreeze DirectoryView class header
	*
*/

#pragma once

#include "Global.hpp"

#include "PBDirTreeModel.hpp"

class PBDirViewDelegate : public QStyledItemDelegate {
	Q_OBJECT

	public:
		void paint( QPainter*, const QStyleOptionViewItem&, const QModelIndex& ) const;
};

class PBDirectoryView : public QTreeView {
	Q_OBJECT

	public:
		PBDirectoryView( QWidget *parent = 0 );

		QString currentBranch() const;

	public Q_SLOTS:
		void setCurrentBranch( QString );

	private:
		PBDirTreeModel *mdl;
};
