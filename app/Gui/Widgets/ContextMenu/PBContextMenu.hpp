/*
	*
	* PBContextMenu.hpp - PBContextMenu.cpp header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBGuiWidgets.hpp"

class PBActionsMenu : public QMenu {
	Q_OBJECT

	public:
		PBActionsMenu( QList<QModelIndex>, QString, QWidget * );
		QStringList command();

	private:
		void buildDefaultActions();
		void buildPluginsActions();
		void buildCustomActionsMenu();

		QList<QModelIndex> selection;
		QString workingDir;

	private slots:
		void extract();
		void compress();
		void showCustomActionsDialog();
		void doCustomAction();

		void encrypt();
		void decrypt();

	Q_SIGNALS:
		void extractArchive( QString );
		void addToArchive( QStringList );
};

class PBOpenWithMenu : public QMenu {
	Q_OBJECT

	public:
		PBOpenWithMenu( QString, QString, QWidget* );
		void setWorkingDirectory( QString );
		void buildMenu( QList<QModelIndex> );

	private:
		QString workingDir;
};

class PBAddToCatalogMenu : public QMenu {
	Q_OBJECT

	public:
		PBAddToCatalogMenu( QString, QModelIndexList, QWidget * );

	private:
		QString workNode;
		QModelIndexList sNodes;

	private slots:
		void addToCatalog();
		void addToNewCatalog();

	Q_SIGNALS:
		void reloadCatalogs();
};

// class PBSuperStartMenu : public QMenu {
	// Q_OBJECT

	// public:
		// PBSuperStartMenu();
// };
