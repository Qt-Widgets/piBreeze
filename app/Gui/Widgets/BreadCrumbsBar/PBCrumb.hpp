/*
	*
	* PBCrumb.hpp - The Segmented Buttons Class Header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBGuiWidgets.hpp"
#include "PBIconManager.hpp"

class PBCrumb : public QWidget {
	Q_OBJECT

	public:
		/* @p1 Full Path until the current crumb */
		PBCrumb( QString, bool currentPath );

		/* Size hint */
		QSize sizeHint() const;

		QString path();

	public slots:
		void showMenu();

	protected:
		/* Path */
		QString mPath;
		QString mCrumbText;
		QDir cwd;

		/* Current Dir? */
		bool m_Current;

		/* Button Size */
		int mWidth;

		/* The crumb was pressed */
		bool m_Pressed;

		/* Menu button was pressed */
		bool m_MenuButtonPressed;

	private slots:
		void onMenuItemClicked();

	protected:
		void mousePressEvent( QMouseEvent* );
		void mouseReleaseEvent( QMouseEvent* );

		void enterEvent( QEvent* );
		void leaveEvent( QEvent* );

		void paintEvent( QPaintEvent* );

	Q_SIGNALS:
		void loadPath( QString );
};
