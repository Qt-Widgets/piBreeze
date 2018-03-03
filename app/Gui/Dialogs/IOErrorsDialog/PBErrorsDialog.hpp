/*
	*
	* PBErrorsDialog.hpp - PBErrorsDialog.cpp header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBDialog.hpp"
#include "PBGuiWidgets.hpp"
#include "PBIconManager.hpp"
#include "PBButtons.hpp"

class PBErrorsDialog : public PBDialog {
	Q_OBJECT

	public:
		PBErrorsDialog( QString title, QString text, QStringList, QWidget *parent = 0 );

	private:
		QLabel *iconLbl, *textLbl;
		PBSegmentButton *segBtns;
		PBButton *okayBtn;
		QTableWidget *table;

		QStringList nodeList;

		QString mTitle, mText;

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
