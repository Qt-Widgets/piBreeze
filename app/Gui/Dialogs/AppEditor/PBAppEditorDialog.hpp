/*
	*
	* PBAppEditorDialog.hpp - AppEditor class header for PiBreeze
	*
*/

#pragma once

#include "Global.hpp"

typedef QMap<QString, QStringList> MDict;

class PBMimeTypeSelector : public QTreeWidget {
	Q_OBJECT

	public :
		PBMimeTypeSelector( QWidget *parent = 0  );
		QStringList selectedItems();

		void setChecked( QString, bool );
		bool isChecked( QString );

	private :
		void populateTree();

		MDict mdict;
		QStringList checked;

	public slots :
		void changeSelection( QTreeWidgetItem*, int );
};

class PBAppIconButton : public QToolButton {
	Q_OBJECT

	public:
		PBAppIconButton( QWidget *parent );

		void setIcon( QString );
		QString iconPath();

	private:
		QString mIconFile;

	private Q_SLOTS:
		void selectIcon();
};

class PBDesktopFileEditor : public QDialog {
	Q_OBJECT

	public:
		PBDesktopFileEditor( QWidget*, PBAppFile );

	private:
		void createGUI();
		void updateEntries();
		void setDialogProperties();

		QLineEdit *nameLE, *descrLE, *cmtLE, *execLE;
		PBAppIconButton *iconBtn;

		QLineEdit *wrkPthLE;
		QCheckBox *hiddenCB, *termCB;
		PBMimeTypeSelector *mimesTW;

		QPushButton *saveBtn, *cancelBtn;

		PBAppFile mApp;

	public Q_SLOTS:
		void accept();
};
