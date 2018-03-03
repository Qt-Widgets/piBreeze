/*
	*
	* PBIconViewRestricted.hpp - PiBreeze Restricted IconViewer Class Header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBItemViewModel.hpp"
#include "PBIconDelegate.hpp"
#include "PBIconView.hpp"

class PBIconViewRestricted : public PBIconView {
	Q_OBJECT

	public:
		PBIconViewRestricted( PBItemViewModel*, QWidget* );

		PBItemViewModel* mdl;

	protected:
		void mousePressEvent( QMouseEvent * );
		void mouseMoveEvent( QMouseEvent * );
		void mouseReleaseEvent( QMouseEvent * );
		void mouseDoubleClickEvent( QMouseEvent * );

		void dragEnterEvent( QDragEnterEvent* );
		void dragMoveEvent( QDragMoveEvent* );
		void dropEvent( QDropEvent* );
};
