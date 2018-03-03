/*
	*
	* PBDialog.hpp - PBDialog.cpp header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBGuiWidgets.hpp"

class PBDialog : public QDialog {
	Q_OBJECT

	public:
		PBDialog( QWidget *parent = 0 );

		void setLayout( QLayout* );
		void setDialogTitle( QString );
		void setDialogIcon( QIcon );

	private:
		void setupGUI();
		void setupActions();
		void setupDialogProperties();

	protected:
		void closeEvent( QCloseEvent* );

	Q_SIGNALS:
		void closed();
};
