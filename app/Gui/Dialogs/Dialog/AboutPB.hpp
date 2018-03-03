/*
	*
	* AboutPB.hpp - AboutPB.cpp header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBDialog.hpp"
#include "PBButton.hpp"
#include "PBGuiWidgets.hpp"

class AboutPB : public QDialog {
	Q_OBJECT

	public:
		AboutPB( QWidget *parent = NULL );

	private:
		void createGui();
		void setWindowProperties();

		QWidget* createAboutPB();
		QWidget* createLicense();
		QWidget* createCredits();
};

class License : public QDialog {
	Q_OBJECT

	public:
		License( QWidget *parent = NULL );
};
