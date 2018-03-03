/*
	*
	* PBAddressBar.hpp - PBAddressBar.cpp Header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBGuiWidgets.hpp"
#include "PBCrumbsBar.hpp"
#include "PBButton.hpp"
#include "PBButtons.hpp"
#include "PBProcessManagerUI.hpp"

class PBToggleButton : public QWidget {
	Q_OBJECT

	public:
		PBToggleButton( QWidget *parent = NULL );

		bool isChecked();
		void setChecked( bool );

		void setShortcut( QString );

	private:
		bool checked;
		QAction *shortcut;

	public slots:
		void toggle();

	protected:
		void mousePressEvent( QMouseEvent* );
		void paintEvent( QPaintEvent* );

	Q_SIGNALS:
		void clicked();
};

class PBAddressEdit : public QLineEdit {
	Q_OBJECT

	public:
		PBAddressEdit( QWidget* );
};

class PBAddressWidget : public QWidget {
	Q_OBJECT

	public:
		PBAddressWidget( QWidget *parent = NULL );
		void setShowHidden( bool );

		PBAddressEdit *addressEdit;
		PBCrumbsBar *crumbsBar;

	public slots:
		void setFocus();

	private :
		void setWidgetProperties();

		PBToggleButton *toggleBtn;
		QDirModel *dModel;

	private slots:
		void toggleCrumbsBarAndEdit();
		void revertToCrumbsBar();

	Q_SIGNALS:
		void openLocation( QString );
};

class PBAddressBar : public QWidget {
	Q_OBJECT

	public:
		PBAddressBar( QWidget *parent = NULL );

		QString address();
		void setAddress( QString );

		void updateNavigationButtons( bool, bool );

		int checkedAction();

	private:
		PBButton *backBtn, *forwardBtn, *homeBtn, *ssBtn;
		PBButton *settingsBtn, *filterBtn;
		PBAddressWidget *addressWidget;
		PBProcessManagerMini *mProcWidget;

	public slots:
		void focusAddressEdit();

		void showSearchButton();
		void hideSearchButton();

	Q_SIGNALS:
		void openLocation( QString );
		void openSearch();

		void goBack();
		void goHome();
		void openSuperStart();
		void goForward();
};
