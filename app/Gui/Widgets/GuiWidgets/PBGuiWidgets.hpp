/*
	*
	* PBGuiWidgets.hpp - PBGuiWidgets.cpp header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBLabels.hpp"

class PBLineEdit : public QLineEdit {
	Q_OBJECT

	public:
		PBLineEdit( QWidget* );

	protected:
		void paintEvent( QPaintEvent * );
};

class PBDriveInfo : public QWidget {
	Q_OBJECT

	public:
		PBDriveInfo( PBDeviceInfo, QWidget * );

	private:
		QString name;
		QString disk;
		QString info;
		QString icon;
		float percent;

	protected:
		virtual void paintEvent( QPaintEvent * );
};

class Separator : public QWidget {
	Q_OBJECT

	public:
		static QWidget* vertical( QWidget *parent = 0 );
		static QWidget* horizontal( QWidget *parent = 0 );

	private:
		enum Mode {
			Horizontal,
			Vertical
		};

		Separator( Separator::Mode mode, QWidget *parent = 0 );

		QLinearGradient vGrad, hGrad;
		Separator::Mode mMode;

	protected:
		void paintEvent( QPaintEvent * );
		void resizeEvent( QResizeEvent * );
};
