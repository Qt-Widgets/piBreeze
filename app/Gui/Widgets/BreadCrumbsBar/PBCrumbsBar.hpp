/*
	*
	* PBCrumbsBar.hpp - PBCrumbsBar.cpp Header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBCrumb.hpp"
#include "FlowLayout.hpp"
#include "PBExtraCrumb.hpp"
#include "PBGuiWidgets.hpp"

/* Menu Popup to show the full path */
class PBCrumbsBarX : public QWidget {
	Q_OBJECT

	public:
		PBCrumbsBarX( QString path, QString cPath, QWidget *parent );

		int heightForWidth( int );

	private:
		FlowLayout *lyt;

	protected:
		void paintEvent( QPaintEvent *pEvent );

	Q_SIGNALS:
		void loadPath( QString );
};

class PBCrumbsBar : public QWidget {
	Q_OBJECT

	public:
		PBCrumbsBar( QWidget *parent );
		void setCurrentDirectory( QString );

	private:
		QString mPath;
		QString mCurrentPath;

		int m_CurrentCrumb;

		QWidget *crumbsHolder;
		PBExtraCrumb *xCrumb;

		/* Ugly hack to handle sidepanel animation */
		QTimer *resizeTimer;

	private slots:
		void doLayout();

		void showCrumbsBarX();

	protected:
		void resizeEvent( QResizeEvent *rEvent );

	Q_SIGNALS:
		void openLocation( QString );
};
