/*
	*
	* PiBreeze.hpp - PiBreeze Header
	*
*/

#pragma once

#include "Global.hpp"
#include "PiBreeze.hpp"

class PBTrayIcon : public QSystemTrayIcon {
	Q_OBJECT

	public :
		PBTrayIcon();

	public slots:
		void handleActivation( QSystemTrayIcon::ActivationReason reason );

	private slots:
		void toggleVisible();
		void showInfo();
		void quit();

	Q_SIGNALS:
		void newWindow();
};
