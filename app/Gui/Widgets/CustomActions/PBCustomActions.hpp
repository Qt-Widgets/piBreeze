/*
	*
	* PBCustomActions.hpp - PBCustomActions.cpp header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBDialog.hpp"
#include "PBButton.hpp"

class PBCustomActions : public PBDialog {
	Q_OBJECT

	public:
		PBCustomActions( QWidget *parent = 0 );
		QString currentAction();

	private:
		void createGUI();
		void setupTable();
		void setWidgetProperties();

		QTableWidget *actionsTable;
		PBButton *addBtn, *delBtn, *quitBtn;

		bool editing;

	private slots:
		void addCustomAction();
		void editCustomAction( int, int );
		void removeCustomActions();

		void updateButtons();
};

class PBNewAction : public PBDialog {
	Q_OBJECT

	public:
		PBNewAction( QWidget *parent = 0 );

		QString actionName();
		QString actionCommand();
		QString actionFilters();

		void setActionName( QString );
		void setActionCommand( QString );
		void setActionFilters( QString );

	private:
		PBLineEdit *actionNameLE, *actionCmdLE, *actionGlobLE;
		PBButton *addBtn, *cancelBtn;
		bool addAction;
};
