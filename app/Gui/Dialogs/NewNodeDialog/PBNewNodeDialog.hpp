/*
	*
	* PBNewNodeDialog.hpp - PBNewNodeDialog.cpp header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBDialog.hpp"
#include "PBButton.hpp"
#include "PBButtons.hpp"
#include "PBMessageDialog.hpp"

class PBNewNodeDialog : public PBDialog {
	Q_OBJECT

	public :
		PBNewNodeDialog( QString, QDir, QString text = QString(), QWidget *parent = 0 );
		PBLineEdit *le;
		PBButton *okBtn, *cancelBtn;
		QDir dir;
		QString data;

	private:
		void setWindowProperties();

	private slots :
		void createFileFolder();
		void cancel();
		void handleTextChanged( QString newText );

	Q_SIGNALS:
		void nodeCreated( QString );
};

class PBNewFileDialog : public PBDialog {
	Q_OBJECT

	public :
		PBNewFileDialog( QDir, QWidget *parent = 0 );
		QComboBox *cb;
		PBLineEdit *le;
		PBButton *okBtn, *cancelBtn;
		QDir dir;
		QStringList mimetypes;

	private:
		void setWindowProperties();

	private slots :
		void createFile();
		void cancel();
		void handleTextChanged( QString newText );

	Q_SIGNALS:
		void nodeCreated( QString );
};
