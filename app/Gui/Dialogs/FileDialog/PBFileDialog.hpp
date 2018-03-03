/*
	*
	* PBFileDialog.hpp - PBFileDialog.cpp Header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBAddressBar.hpp"
#include "PBButtons.hpp"
#include "PBIconViewRestricted.hpp"
#include "PBDirectoryView.hpp"
#include "PBSidePanel.hpp"
#include "PBDialog.hpp"
#include "PBMessageDialog.hpp"
#include "PBGuiWidgets.hpp"

class PBFileDialog : public QDialog {
	Q_OBJECT

	public:
		void setNameFilters( QStringList, QString );
		void clearNameFilter();

		QString selectedItem();
		QStringList selectedItems();

		QString selectedFilter();

		static QString getFileName( QWidget*, QString, QString, QStringList, QString );

	private:
		PBFileDialog( QWidget *parent, QString title, QString location );

		void createGUI();
		void createAndSetupActions();
		void setWindowProperties();

		PBItemViewModel *fsModel;

		PBAddressWidget *addressBar;
		PBSidePanel *sidePanel;

		PBIconView *mainView;

		PBLineEdit *nameLE;
		QComboBox *filtersCB;

		QStringList filters;

		QString title;
		QString location;

	private slots:
		void openAddressBar();
		void open( QModelIndex );
		void open( QString );

		void resetFilters();
		void updateToolBar();
};

class PBDirectoryDialog : public PBDialog {
	Q_OBJECT

	public:
		static QString getDirectoryName( QWidget*, QString, QString );

		QString selectedItem();

	private:
		PBDirectoryDialog( QWidget *parent, QString title, QString location );

		void createGUI();
		void setWindowProperties();

		PBAddressWidget *addressBar;
		PBSidePanel *sidePanel;

		PBDirectoryView *dirView;

		PBLineEdit *nameLE;
		QComboBox *filtersCB;

		QStringList filters;

		QString title;
		QString location;
};
