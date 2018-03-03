/*
	*
	* PBRenameDialog.hpp - PBRenameDialog.cpp header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBDialog.hpp"
#include "PBButtons.hpp"

class PBRenameDialog : public PBDialog {
	Q_OBJECT

	public :
		PBRenameDialog( QString origName, QDir curDir, QWidget *parent = 0 );
		QString newName();
		bool canRename();

	private:
		void createGUI();

		PBSegmentButton *segBtns;

		PBLineEdit *le;

		QCheckBox *replaceCB;
		QString name;
		QDir dir;
		bool renameOk;

	private slots :
		void handleSegmentClick( int );
		void rename();
		void cancel();
		void handleTextChanged( QString newText );
		void handleCBStateChanged();
};

class PBSuperStartRenameDialog : public PBDialog {
	Q_OBJECT

	public :
		PBSuperStartRenameDialog( QString origName, QString category, QWidget *parent = 0 );
		QString newName();
		bool canRename();

	private:
		void createGUI();

		PBSegmentButton *segBtns;

		PBLineEdit *le;

		QString name;
		QString category;

		bool renameOk;

	private slots :
		void handleSegmentClick( int );
		void rename();
		void cancel();
		void handleTextChanged( QString newText );
};
