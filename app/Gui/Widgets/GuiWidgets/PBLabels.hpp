/*
	*
	* PBLabels.hpp - PBLabels.cpp header file
	*
*/

#pragma once

#include "Global.hpp"

class PBClickLabel : public QLabel {
	Q_OBJECT

	public:
		PBClickLabel( QPixmap icon = QPixmap(), QWidget *parent = NULL );
		PBClickLabel( QString icon = QString(), QWidget *parent = NULL );

		void setClickable( bool );

	private :
		bool clickEnabled;

	protected:
		void mousePressEvent( QMouseEvent * );
		void mouseReleaseEvent( QMouseEvent * );

	signals:
		void pressed();
		void clicked();
		void released();
};

class PBInfoLabel : public QLabel {
	Q_OBJECT

	public:
		PBInfoLabel();
		void setText( QString, QString, QString, QString );
};
