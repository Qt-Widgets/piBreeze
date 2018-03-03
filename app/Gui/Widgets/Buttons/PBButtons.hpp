/*
	*
	* PBSegmentButton.hpp - The Segmented Buttons Class Header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBButton.hpp"
#include "PBGuiWidgets.hpp"

class PBSegmentButton : public QWidget {
	Q_OBJECT

	public:
		enum SelectionBehavior { SelectOne, SelectAll, SelectNone };

		PBSegmentButton( QWidget* );
		PBSegmentButton( QList<QIcon>, QWidget* );
		PBSegmentButton( QStringList, QWidget* );
		PBSegmentButton( QStringList, QList<QIcon>, QWidget* );

		int count();
		void setCount( int );

		QList<PBButton*> buttons();

		PBButton* segment( int );

		void setSegmentIcon( int, QIcon );
		void setSegmentText( int, QString );

		void setSegmentWidth( int );

		void setSegmentShortcut( int, QKeySequence );

		void setSegmentDisabled( int );
		void setSegmentEnabled( int );

		void insertSegment( PBButton *button, int logicalPos );
		void addSegment( PBButton *button );

		int selectionBehavior();
		void setSelectionBehavior(SelectionBehavior behavior);

		QSize size();
		QSize sizeHint();

	private:
		void resetStyleSheets();
		void autoResize();

		QHBoxLayout *btnsLyt;
		QList<PBButton*> segments;

		int myButtonHeight;
		int myWidgetWidth;
		int mSelectionBehavior;

		QButtonGroup *btnGrp;

	private slots:
		void handleSegmentClick();

	Q_SIGNALS:
		void clicked( int );
};
