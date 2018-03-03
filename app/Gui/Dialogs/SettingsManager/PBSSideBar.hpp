/*
	*
	* PBSSideBar.hpp - PBSSideBar.cpp Header
	*
*/

#pragma once

#include "Global.hpp"

class PBSSideBar : public QWidget {
	Q_OBJECT

	public:
		PBSSideBar( QWidget *parent  = NULL );

	Q_SIGNALS:
		void loadSettingsCategory( int );
};
