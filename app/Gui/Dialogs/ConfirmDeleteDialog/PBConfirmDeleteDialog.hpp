/*
	*
	* PBConfirmDeleteDialog.hpp - PBConfirmDeleteDialog.cpp header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBDialog.hpp"
#include "PBGuiWidgets.hpp"
#include "PBIconManager.hpp"
#include "PBButtons.hpp"

class PBConfirmDeleteDialog : public PBDialog {
	Q_OBJECT

	public:
		PBConfirmDeleteDialog( QString, QStringList, bool permanent = false, QWidget *parent = 0 );

	private:
		QLabel *iconLbl, *textLbl;
		PBSegmentButton *segBtns;
		QTableWidget *table;
		QStringList deletePaths;
		QString source;

		void setupGUI( bool );
		void setupTable();
		void addEntry( QString );

		bool deleteFiles;

	protected:
		void keyPressEvent( QKeyEvent *keyEvent );

	public slots:
		void close();
		int exec();

	private slots:
		void handleSegmentClick( int );
		void deleteOk();
		void deleteCancel();

	signals:
		void closed();
};

class PBConfirmDeleteProtectedDialog : public PBDialog {
	Q_OBJECT

	public:
		PBConfirmDeleteProtectedDialog( QString, QStringList, bool permanent = false, QWidget *parent = 0 );

	private:
		QLabel *iconLbl, *textLbl;
		PBSegmentButton *segBtns;
		QLineEdit *confirmPhraseLE;
		QTableWidget *table;
		QStringList deletePaths;
		QString source;

		void setupGUI( bool );
		void setupTable();
		void addEntry( QString );

		int deleteFiles;

	protected:
		void keyPressEvent( QKeyEvent *keyEvent );

	public slots:
		void close();
		int exec();

	private slots:
		void handleSegmentClick( int );
		void handleTextChanged( QString );
		void deleteAll();
		void deleteOthers();
		void deleteCancel();

	signals:
		void closed();
};

class PBDeleteErrorsDialog : public PBDialog {
	Q_OBJECT

	public:
		PBDeleteErrorsDialog( QStringList, QStringList, QWidget *parent = 0 );

	private:
		QLabel *iconLbl, *textLbl;
		PBSegmentButton *segBtns;
		QTableWidget *table;
		QStringList fileList, dirList;

		void setupGUI();
		void setupTable();
		void addEntry( QString );

	protected:
		void keyPressEvent( QKeyEvent *keyEvent );

	public slots:
		void close();
		int exec();

	signals:
		void closed();
};

// namespace PBProtectOSNodeDialog {

	// int {}
// };
