/*
	*
	* PBPBFilterWidget.hpp - PBPBFilterWidget.cpp header for PiBreeze
	*
*/

#pragma once

#include "Global.hpp"

class PBFilterWidget : public QWidget {
	Q_OBJECT

	public:
		PBFilterWidget( QWidget *parent );

		void clear();

	private:
		void setupUI();

		QLineEdit *findLE;
		QLabel *findLbl;

	public slots:
		void setFocus();
		void show();
		void hide();

	protected:
		void keyPressEvent( QKeyEvent* );

	Q_SIGNALS:
		void search( QString );
		void shown();
		void hidden();
};
