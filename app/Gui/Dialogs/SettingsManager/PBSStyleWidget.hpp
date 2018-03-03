/*
	*
	* PBSStyleWidget.hpp - PBSStyleWidget.cpp header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBButtons.hpp"

class PBSSPreviewWidget;

class PBSStyleWidget : public QWidget {
	Q_OBJECT

	public:
		PBSStyleWidget( QWidget* );

	private:
		QComboBox *styleListCB;
		PBSegmentButton *segBtn;

		PBSSPreviewWidget *preview;

	private slots:
		void stylePreview( const QString );
		void applyStyle();
};

class PBSSPreviewWidget : public QWidget {

	public:
		PBSSPreviewWidget( QWidget* );
		void showStyle( const QString );
};
