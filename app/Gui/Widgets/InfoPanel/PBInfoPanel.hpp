/*
	*
	* PBInfoPanel.hpp - InfoPanel class Header
	*
*/

#pragma once

#include "Global.hpp"

class PBInfoPanel : public QWidget {
	Q_OBJECT

	public:
		PBInfoPanel( QWidget *parent );

		void updatePanel( QString, QModelIndexList );

	private:
		QLabel *iconLbl;
		QLabel *infoLbl;

		QLabel *nameLbl;
		QLabel *sizeLbl;
		QLabel *pathLbl;
		QLabel *permLbl;
		QLabel *mimeLbl;

		void setIcon( QPixmap );
		void setIcon( QIcon );

		void setWidgetProperties();

	public slots:

	protected:
		void paintEvent( QPaintEvent* );

	Q_SIGNALS:
};
