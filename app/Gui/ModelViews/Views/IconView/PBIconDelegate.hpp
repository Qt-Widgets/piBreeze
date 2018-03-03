/*
	*
	* PBIconDelegate.hpp - PBIconDelegate.cpp Header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBStyleOptionViewItem.hpp"

class PBIconDelegate : public QItemDelegate {
    Q_OBJECT

	public:
		void paintIcons( QPainter *painter, const PBStyleOptionViewItem &option, const QModelIndex &index ) const;

	private:
		void paintIconTextDetails( QPainter *painter, QRect &textRect, const QModelIndex &index ) const;

		mutable int maxLines;		// Max number of lines that can be written
		mutable int textLines;		// Number of lines the filename occupies
};
