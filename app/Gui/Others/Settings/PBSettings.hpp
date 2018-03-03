/*
    *
    * PBSettings.hpp - PBSettings class header
    *
*/

#pragma once
#ifndef PBSETTINGS_HPP
#define PBSETTINGS_HPP

#include <QString>
#include <QList>
#include <QKeySequence>
#include <QColor>
#include <QRect>
#include <QSize>
#include <QSettings>

class PBSettings {
	public:

		// General Settings
		class GeneralSettings {
			public:
				QString Style;									// Gui theme
				QString IconTheme;								// Icon Theme
				bool TrayIcon;									// Minimize to tray?
				bool OpenWithCatalog;							// Show catalog on PB Open?
				bool ShowHidden;								// Show Hidden files and folders?
				QSize IconSize;									// Default Icon Size
				int SortColumn;									// Default Sort Column
				bool SortCase;									// Is sorting case sensitive
				bool Grouping;									// Enable grouping?
				bool PerFolderViews;							// Enable per folder views?
				bool FilterFolders;								// Filter folders while searching?
				bool SuperStart;								// Use CombiView as Home
				bool SpecialOpen;								// Use CombiView as Home
				bool DirectIO;									// Use direct copy/move
		} General;

		// Special Settings
		class SpecialSettings {
			public:
				bool ClosingDown;
		} Special;

		// Session Settings
		class SessionSettings {
			public:
				QRect Geometry;
				QString LastDir;
				bool Maximized;
		} Session;

		// Shortcut Settings
		class ShortcutSettings {
			public :
				QList<QKeySequence> AboutPB;
				QList<QKeySequence> AboutQt;
				QList<QKeySequence> ToggleCrumbLE;
				QList<QKeySequence> AddCustomAction;
				QList<QKeySequence> GoHome;
				QList<QKeySequence> GoUp;
				QList<QKeySequence> GoLeft;
				QList<QKeySequence> GoRight;
				QList<QKeySequence> NewFolder;
				QList<QKeySequence> NewFile;
				QList<QKeySequence> NewEncFS;
				QList<QKeySequence> Peek;
				QList<QKeySequence> Reload;
				QList<QKeySequence> ToggleHidden;
				QList<QKeySequence> SelectAll;
				QList<QKeySequence> Cut;
				QList<QKeySequence> Copy;
				QList<QKeySequence> Paste;
				QList<QKeySequence> Rename;
				QList<QKeySequence> Delete;
				QList<QKeySequence> Trash;
				QList<QKeySequence> Properties;
				QList<QKeySequence> Permissions;
				QList<QKeySequence> Terminal;
				QList<QKeySequence> PBInfo;
				QList<QKeySequence> Settings;
				QList<QKeySequence> CustomActions;
				QList<QKeySequence> FocusAddressBar;
				QList<QKeySequence> NewWindow;
				QList<QKeySequence> AddBookmark;
				QList<QKeySequence> FocusSearchBar;
				QList<QKeySequence> ClearSearchBar;
				QList<QKeySequence> QuitPiBreeze;
		} Shortcuts;

		static PBSettings* defaultInstance();
		static PBSettings* instance();

		void reload();
		void setValue( QString, QVariant );

	private:
		PBSettings() {};

		bool init;

		static PBSettings *settings;
		static PBSettings *defaultSettings;
};

#endif
