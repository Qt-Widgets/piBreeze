/*
	*
	* PBSPluginsWidget.hpp - Plugins Manager Widget for Settings Manager
	*
*/

#pragma once

#include "Global.hpp"
#include "PBButtons.hpp"
#include "PBPluginManager.hpp"
#include "PBFileDialog.hpp"

class PBSPluginsWidget : public QWidget {
	Q_OBJECT

	public:
		PBSPluginsWidget( QWidget *parent );

	private:
		void createGUI();

		QListWidget *pPathsLW;
		PBSegmentButton *pathsBtn;

	private Q_SLOTS:
		void buttonClick( int );

		void addPath();
		void removePath();

		void updateSettings();
};
