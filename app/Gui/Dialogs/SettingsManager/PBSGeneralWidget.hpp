/*
	*
	* PBSGeneralWidget.hpp - PBSGeneralWidget.cpp header
	*
*/

#pragma once

#include "Global.hpp"

class PBSGeneralWidget : public QWidget {
	Q_OBJECT

	public:
		PBSGeneralWidget( QWidget* );

	private:
		/* Grouping */
		QGroupBox *perFolderEnabled;
		QComboBox *defaultViewModeCB;			// Icon, Tiles or Details view
		QSlider *defaultIconSizeS;				// 16 - 128
		QLabel *iconSizeL;						// Show the current icon size
		QCheckBox *defaultGroupingCB;			// Show in groups enabled?
		QCheckBox *defaultSortCaseCB;			// Case sensitive sort?
		QComboBox *defaultSortClmnCB;			// Sort column: Name, Size, Type or Date?

		/* Others */
		QCheckBox *filterFoldersCB;				// Should we filter folders while searching?
		QCheckBox *showTrayIconCB;				// Show tray icon?
		QCheckBox *openWithCB;					// Open with special locations?
		QRadioButton *openWithCatalogRB;		// Show home folder or catalogs?
		QRadioButton *openWithSuperStartRB;		// Show home folder or catalogs?
		QCheckBox *directIOCB;					// Direct Copy/Move

	private slots:
		/*  */
		void handlePerFolderChanged( bool );
		void handleIconSizeChanged( int );
		void handleGroupingChanged( bool );
		void handleSortCaseChanged( bool );
		void handleSortColumnChanged( int );

		/*  */
		void handleFilterDirsChanged( bool );
		void handleTrayIconChanged( bool );
		void handleOpenWithToggled();
		void handleDirectIOChanged( bool );
};
