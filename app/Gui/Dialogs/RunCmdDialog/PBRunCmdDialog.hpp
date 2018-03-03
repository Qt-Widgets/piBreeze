/*
	*
	* PBRunCmdDialog.hpp - PBRunCmdDialog.cpp header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBDialog.hpp"
#include "PBButton.hpp"
#include "PBButtons.hpp"
#include "PBGuiWidgets.hpp"
#include "PBOpenWithView.hpp"

class PBRunCmdDialog : public PBDialog {
	Q_OBJECT

	public :
		PBRunCmdDialog( QString, QWidget *parent = 0 );
		QString commandString();

		bool canRun();

	private:
		void createGUI();
		void createAndSetupActions();
		void setWindowProperties();

		QString execCmd;
		PBLineEdit *le;
		PBButton *okBtn, *cancelBtn, *defaultAndRunBtn;
		PBOpenWithView *appList;
		QString fileName;
		bool runOk;

	private slots :
		void runCommand();
		void runCommand( QModelIndex );
		void cancel();
		void appSelected( QModelIndex );
		void handleTextChanged( QString newText );
		void makeDefaultAndRunCommand();
};
