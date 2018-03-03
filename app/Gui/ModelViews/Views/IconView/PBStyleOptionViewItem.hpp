/*
	*
	* PBStyleOptionViewItem.cpp - PiBreeze IconView StyleOption Class Header
	*
*/

#pragma once

#include "Global.hpp"

class PBStyleOptionViewItem : public QStyleOptionViewItem {
	public:
		enum NodeType {
			Normal = 0x69BA3A,
			SymLink,
			Executable,
		};

		PBStyleOptionViewItem() {

			nodeType = Normal;
		};

		PBStyleOptionViewItem( QStyleOptionViewItem other ) : QStyleOptionViewItem( other ) {

			nodeType = Normal;
		};

		int nodeType;
};
