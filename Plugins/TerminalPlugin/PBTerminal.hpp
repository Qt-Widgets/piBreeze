/*
	*
	* PBTerminal.hpp - PBTerminal.cpp Header
	*
*/

#ifndef QTMAIN_HPP
#define QTMAIN_HPP

#include "Global.hpp"
#include "TtyTabWidget.hpp"

class PBTerminal : public QMainWindow {
	Q_OBJECT

	public :
		PBTerminal( QString workdir, QWidget *parent = 0 );
		~PBTerminal();

	private :
		void createGUI();
		void setupActions();
		void setWindowProperties();

		TtyTabWidget *TabWidget;

	public slots :
		void showHide();

	private slots:
		void showSettings();

		void openFMgr();

	protected :
		void closeEvent( QCloseEvent *cEVent );
};

#endif
