/*
	*
	* PBSettingsManager.hpp - PBSettingsManager.cpp header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBDialog.hpp"
#include "PBGuiWidgets.hpp"
#include "PBSGeneralWidget.hpp"
#include "PBSIconsWidget.hpp"
#include "PBSKeyBindingsWidget.hpp"
#include "PBSStyleWidget.hpp"
#include "PBSSideBar.hpp"

class PBSettingsManager : public PBDialog {
	Q_OBJECT

	public:
		PBSettingsManager( QWidget *parent = 0 );
		void addNewTerminal();

	private:
		void createGUI();

		QHBoxLayout *createBodyLayout();
		QHBoxLayout *createFooterLayout();

		void setWindowProperties();

		QStackedLayout *widgetLyt;
		PBSSideBar *chooserWidget;
		PBSStyleWidget *styleWidget;
		PBIconThemeWidget *iconWidget;
		PBKeyBindingsWidget *keysWidget;

	private slots:
		void settingCategoryChosen( int );

	protected:
		void keyPressEvent( QKeyEvent* );
};
