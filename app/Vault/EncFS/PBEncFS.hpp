/*
	*
	* PBDerypt.hpp - PiBreeze Folder Encryption Class Header
	*
*/

#pragma once

#ifdef STANDALONE
	#include "Global2.hpp"
#else
	#include "Global.hpp"
#endif
#include "PBTools.hpp"

class PBEncFS : public QObject {
	Q_OBJECT

	public:
		/* Init */
		PBEncFS( QString, QString target = QString(), QWidget *parent = NULL );

	public Q_SLOTS:
		bool mountDir( QString );
		bool unmountDir();

		bool createEncFS( QString );
		bool changePass( QString, QString );

	private:
		QString mSource;
		QString mTarget;

		QWidget *mParent;
};

class PBCreateEncFS : public QDialog {
	Q_OBJECT

	public:
		/* Init */
		PBCreateEncFS( QWidget *parent = NULL );

		QString directoryName();

	private:
		void createGUI();

		QLineEdit *dirLE;

		QString mSource;
		QString mTarget;

		QPushButton *createBtn;

	private Q_SLOTS:
		void updateButton();
};
