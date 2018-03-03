/*
	*
	* PBSGeneralWidget.hpp - PBSGeneralWidget.cpp header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBButton.hpp"
#include "PBIconManager.hpp"

class PBIconThemeChooserWidget;
class PBIconThemeViewerWidget;

class PBIconThemeWidget : public QWidget {
	Q_OBJECT

	public:
		PBIconThemeWidget( QWidget *parent );

	private:
		void createGUI();
		void updateListView();

		PBIconThemeChooserWidget *iconThemesWidget;
		PBIconThemeViewerWidget *folderViewWidget;
};

class PBIconThemeChooserWidget : public QWidget {
	Q_OBJECT

	public:
		PBIconThemeChooserWidget( QWidget * );
		void loadThemes();
		QStringList themesList;

	private:
		int current;
		PBButton *prevBtn, *nextBtn;
		QComboBox *themeCB;

	private slots:
		void switchTheme( int );
		void nextTheme();
		void previousTheme();

	signals:
		void reloadIcons();
};

class PBIconThemeModel: public QAbstractListModel {
	Q_OBJECT

	public:
		PBIconThemeModel( QObject * );

		int rowCount( const QModelIndex &parent = QModelIndex() ) const;
		QVariant data( const QModelIndex &index, int role ) const;

		inline Qt::ItemFlags flags( const QModelIndex ) const {

			return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
		};

	private:
		QStringList mimeNameList;
		QList<QIcon> mimeIconList;
		QStringList mimeTypeList;

	private slots:
		void setupModel();
};

class PBIconThemeViewerWidget: public QListView {
	Q_OBJECT

	public:
		PBIconThemeViewerWidget( QWidget * );

	Q_SIGNALS:
		void setupModel();
};
