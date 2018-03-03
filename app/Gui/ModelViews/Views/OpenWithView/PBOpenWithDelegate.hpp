/*
	*
	* PBIconDelegate.hpp - PBIconDelegate.cpp Header
	*
*/

#pragma once

#include "Global.hpp"

class PBOpenWithDelegate : public QItemDelegate {
    Q_OBJECT

	public:
		void paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
};
