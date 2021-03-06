/*
	*
	* PBSettings.cpp - PiBreeze Settings
	*
*/

#include "pibreeze.hpp"
#include "PBSettings.hpp"

inline QList<QKeySequence> getShortcuts( QString actionName ) {

	QSettings sSettings( "PiBreeze", "Shortcuts" );

	QStringList keys = sSettings.value( actionName ).toStringList();
	QList<QKeySequence> shortcuts;

	foreach( QString key, keys )
		shortcuts << QKeySequence( key );

	return shortcuts;
};

PBSettings *PBSettings::settings = NULL;
PBSettings *PBSettings::defaultSettings = NULL;

PBSettings* PBSettings::defaultInstance() {

	defaultSettings = new PBSettings();
	defaultSettings->init = true;

	defaultSettings->General.Style = QString( "Default" );
	defaultSettings->General.IconTheme = PBSystemIconTheme();
	defaultSettings->General.TrayIcon = true;
	defaultSettings->General.OpenWithCatalog = false;
	defaultSettings->General.ShowHidden = false;
	defaultSettings->General.IconSize = QSize( 48, 48 );
	defaultSettings->General.SortColumn = 2;
	defaultSettings->General.SortCase = false;
	defaultSettings->General.Grouping = true;
	defaultSettings->General.PerFolderViews = true;
	defaultSettings->General.FilterFolders = true;
	defaultSettings->General.SuperStart = true;
	defaultSettings->General.SpecialOpen = true;
	defaultSettings->General.DirectIO = false;

	defaultSettings->Special.ClosingDown = false;

	defaultSettings->Session.Geometry = QRect( 900, 600, 0, 0 );
	defaultSettings->Session.LastDir = QDir::homePath();
	defaultSettings->Session.Maximized = true;

	defaultSettings->Shortcuts.AboutPB = QList<QKeySequence>() << QKeySequence( "Shift+F1" );
	defaultSettings->Shortcuts.AboutQt = QList<QKeySequence>() << QKeySequence( "Shift+F2" );
	defaultSettings->Shortcuts.ToggleCrumbLE = QList<QKeySequence>() << QKeySequence( "Ctrl+T" );
	defaultSettings->Shortcuts.AddCustomAction = QList<QKeySequence>() << QKeySequence( "Ctrl+D" );
	defaultSettings->Shortcuts.GoHome = QList<QKeySequence>() << QKeySequence( "Alt+Home" );
	defaultSettings->Shortcuts.GoUp = QList<QKeySequence>() << QKeySequence( "Alt+Up" );
	defaultSettings->Shortcuts.GoLeft = QList<QKeySequence>() << QKeySequence( "Alt+Left" );
	defaultSettings->Shortcuts.GoRight = QList<QKeySequence>() << QKeySequence( "Alt+Right" ) << QKeySequence( "Alt+Down" );
	defaultSettings->Shortcuts.NewFolder = QList<QKeySequence>() << QKeySequence( "F10" );
	defaultSettings->Shortcuts.NewFile = QList<QKeySequence>() << QKeySequence( "Ctrl+Shift+N" );
	defaultSettings->Shortcuts.NewEncFS = QList<QKeySequence>() << QKeySequence( "Ctrl+Shift+E" );
	defaultSettings->Shortcuts.Peek = QList<QKeySequence>() << QKeySequence( "Ctrl+Return" );
	defaultSettings->Shortcuts.Reload = QList<QKeySequence>() << QKeySequence( "F5" ) << QKeySequence( "Ctrl+R" );
	defaultSettings->Shortcuts.ToggleHidden = QList<QKeySequence>() << QKeySequence( "Alt+." ) << QKeySequence( "Ctrl+H" );
	defaultSettings->Shortcuts.SelectAll = QList<QKeySequence>() << QKeySequence( "Ctrl+A" );
	defaultSettings->Shortcuts.Cut = QList<QKeySequence>() << QKeySequence( "Ctrl+X" );
	defaultSettings->Shortcuts.Copy = QList<QKeySequence>() << QKeySequence( "Ctrl+C" );
	defaultSettings->Shortcuts.Paste = QList<QKeySequence>() << QKeySequence( "Ctrl+V" );
	defaultSettings->Shortcuts.Rename = QList<QKeySequence>() << QKeySequence( "F2" );
	defaultSettings->Shortcuts.Delete = QList<QKeySequence>() << QKeySequence( "Shift+Delete" );
	defaultSettings->Shortcuts.Trash = QList<QKeySequence>() << QKeySequence( "Delete" );
	defaultSettings->Shortcuts.Properties = QList<QKeySequence>() << QKeySequence( "Alt+Return" );
	defaultSettings->Shortcuts.Permissions = QList<QKeySequence>() << QKeySequence( "Alt+Shift+Return" );
	defaultSettings->Shortcuts.Terminal = QList<QKeySequence>() << QKeySequence( "F4" );
	defaultSettings->Shortcuts.PBInfo = QList<QKeySequence>() << QKeySequence( "Ctrl+I" );
	defaultSettings->Shortcuts.Settings = QList<QKeySequence>() << QKeySequence( "Ctrl+Shift+S" );
	defaultSettings->Shortcuts.CustomActions = QList<QKeySequence>() << QKeySequence( "Ctrl+Shift+D" );
	defaultSettings->Shortcuts.FocusAddressBar = QList<QKeySequence>() << QKeySequence( "Ctrl+L" ) << QKeySequence( "F6" );
	defaultSettings->Shortcuts.NewWindow = QList<QKeySequence>() << QKeySequence( "Ctrl+N" );
	defaultSettings->Shortcuts.AddBookmark = QList<QKeySequence>() << QKeySequence( "Ctrl+D" );
	defaultSettings->Shortcuts.FocusSearchBar = QList<QKeySequence>() << QKeySequence( "Ctrl+F" );
	defaultSettings->Shortcuts.ClearSearchBar = QList<QKeySequence>() << QKeySequence( "Ctrl+Shift+F" );
	defaultSettings->Shortcuts.QuitPiBreeze = QList<QKeySequence>() << QKeySequence( "Ctrl+Shift+Q" );

	return defaultSettings;
};

PBSettings* PBSettings::instance() {

	if ( settings )
		return settings;

	QSettings gSettings( "PiBreeze", "PiBreeze" );
	QSettings sSettings( "PiBreeze", "Shortcuts" );

	settings = defaultInstance();

	QStringList gaKeys = gSettings.allKeys();
	QStringList saKeys = sSettings.allKeys();

	if ( gaKeys.contains( QString( "Style" ) ) )
		settings->General.Style = gSettings.value( "Style" ).toString();

	if ( gaKeys.contains( QString( "IconTheme" ) ) )
		settings->General.IconTheme = gSettings.value( "IconTheme" ).toString();

	if ( gaKeys.contains( QString( "TrayIcon" ) ) )
		settings->General.TrayIcon = gSettings.value( "TrayIcon" ).toBool();

	if ( gaKeys.contains( QString( "ShowHidden" ) ) )
		settings->General.ShowHidden = gSettings.value( "ShowHidden" ).toBool();

	if ( gaKeys.contains( QString( "IconSize" ) ) )
		settings->General.IconSize = gSettings.value( "IconSize" ).toSize();

	if ( gaKeys.contains( QString( "SortColumn" ) ) )
		settings->General.SortColumn = gSettings.value( "SortColumn" ).toInt();

	if ( gaKeys.contains( QString( "SortCase" ) ) )
		settings->General.SortCase = gSettings.value( "SortCase" ).toBool();

	if ( gaKeys.contains( QString( "Grouping" ) ) )
		settings->General.Grouping = gSettings.value( "Grouping" ).toBool();

	if ( gaKeys.contains( QString( "PerFolderViews" ) ) )
		settings->General.PerFolderViews = gSettings.value( "PerFolderViews" ).toBool();

	if ( gaKeys.contains( QString( "FilterFolders" ) ) )
		settings->General.FilterFolders = gSettings.value( "FilterFolders" ).toBool();

	if ( gaKeys.contains( QString( "OpenWithCatalog" ) ) )
		settings->General.OpenWithCatalog = gSettings.value( "OpenWithCatalog" ).toBool();

	if ( gaKeys.contains( QString( "SuperStart" ) ) )
		settings->General.SuperStart = gSettings.value( "SuperStart" ).toBool();

	if ( gaKeys.contains( QString( "SpecialOpen" ) ) )
		settings->General.SpecialOpen = gSettings.value( "SpecialOpen" ).toBool();

	if ( gaKeys.contains( QString( "DirectIO" ) ) )
		settings->General.DirectIO = gSettings.value( "DirectIO" ).toBool();

	if ( gaKeys.contains( QString( "Session/Geometry" ) ) )
		settings->Session.Geometry = gSettings.value( "Session/Geometry" ).toRect();

	if ( gaKeys.contains( QString( "Session/LastDir" ) ) )
		settings->Session.LastDir = gSettings.value( "Session/LastDir" ).toString();

	if ( gaKeys.contains( QString( "Session/Maximized" ) ) )
		settings->Session.Maximized = gSettings.value( "Session/Maximized" ).toBool();

	if ( saKeys.contains( QString( "AboutPB" ) ) )
		settings->Shortcuts.AboutPB = getShortcuts( "AboutPB" );

	if ( saKeys.contains( QString( "AboutQt" ) ) )
		settings->Shortcuts.AboutQt = getShortcuts( "AboutQt" );

	if ( saKeys.contains( QString( "ToggleCrumbLE" ) ) )
		settings->Shortcuts.ToggleCrumbLE = getShortcuts( "ToggleCrumbLE" );

	if ( saKeys.contains( QString( "AddCustomAction" ) ) )
		settings->Shortcuts.AddCustomAction = getShortcuts( "AddCustomAction" );

	if ( saKeys.contains( QString( "GoHome" ) ) )
		settings->Shortcuts.GoHome = getShortcuts( "GoHome" );

	if ( saKeys.contains( QString( "GoUp" ) ) )
		settings->Shortcuts.GoUp = getShortcuts( "GoUp" );

	if ( saKeys.contains( QString( "GoLeft" ) ) )
		settings->Shortcuts.GoLeft = getShortcuts( "GoLeft" );

	if ( saKeys.contains( QString( "GoRight" ) ) )
		settings->Shortcuts.GoRight = getShortcuts( "GoRight" );

	if ( saKeys.contains( QString( "NewFolder" ) ) )
		settings->Shortcuts.NewFolder = getShortcuts( "NewFolder" );

	if ( saKeys.contains( QString( "NewFile" ) ) )
		settings->Shortcuts.NewFile = getShortcuts( "NewFile" );

	if ( saKeys.contains( QString( "NewEncFS" ) ) )
		settings->Shortcuts.NewEncFS = getShortcuts( "NewEncFS" );

	if ( saKeys.contains( QString( "Peek" ) ) )
		settings->Shortcuts.Peek = getShortcuts( "Peek" );

	if ( saKeys.contains( QString( "Reload" ) ) )
		settings->Shortcuts.Reload = getShortcuts( "Reload" );

	if ( saKeys.contains( QString( "ToggleHidden" ) ) )
		settings->Shortcuts.ToggleHidden = getShortcuts( "ToggleHidden" );

	if ( saKeys.contains( QString( "SelectAll" ) ) )
		settings->Shortcuts.SelectAll = getShortcuts( "SelectAll" );

	if ( saKeys.contains( QString( "Cut" ) ) )
		settings->Shortcuts.Cut = getShortcuts( "Cut" );

	if ( saKeys.contains( QString( "Copy" ) ) )
		settings->Shortcuts.Copy = getShortcuts( "Copy" );

	if ( saKeys.contains( QString( "Paste" ) ) )
		settings->Shortcuts.Paste = getShortcuts( "Paste" );

	if ( saKeys.contains( QString( "Rename" ) ) )
		settings->Shortcuts.Rename = getShortcuts( "Rename" );

	if ( saKeys.contains( QString( "Delete" ) ) )
		settings->Shortcuts.Delete = getShortcuts( "Delete" );

	if ( saKeys.contains( QString( "Trash" ) ) )
		settings->Shortcuts.Trash = getShortcuts( "Trash" );

	if ( saKeys.contains( QString( "Properties" ) ) )
		settings->Shortcuts.Properties = getShortcuts( "Properties" );

	if ( saKeys.contains( QString( "Permissions" ) ) )
		settings->Shortcuts.Permissions = getShortcuts( "Permissions" );

	if ( saKeys.contains( QString( "Terminal" ) ) )
		settings->Shortcuts.Terminal = getShortcuts( "Terminal" );

	if ( saKeys.contains( QString( "PBInfo" ) ) )
		settings->Shortcuts.PBInfo = getShortcuts( "PBInfo" );

	if ( saKeys.contains( QString( "Settings" ) ) )
		settings->Shortcuts.Settings = getShortcuts( "Settings" );

	if ( saKeys.contains( QString( "CustomActions" ) ) )
		settings->Shortcuts.CustomActions = getShortcuts( "CustomActions" );

	if ( saKeys.contains( QString( "FocusAddressBar" ) ) )
		settings->Shortcuts.FocusAddressBar = getShortcuts( "FocusAddressBar" );

	if ( saKeys.contains( QString( "NewWindow" ) ) )
		settings->Shortcuts.NewWindow = getShortcuts( "NewWindow" );

	if ( saKeys.contains( QString( "AddBookmark" ) ) )
		settings->Shortcuts.AddBookmark = getShortcuts( "AddBookmark" );

	if ( saKeys.contains( QString( "FocusSearchBar" ) ) )
		settings->Shortcuts.FocusSearchBar = getShortcuts( "FocusSearchBar" );

	if ( saKeys.contains( QString( "ClearSearchBar" ) ) )
		settings->Shortcuts.ClearSearchBar = getShortcuts( "ClearSearchBar" );

	if ( saKeys.contains( QString( "QuitPiBreeze" ) ) )
		settings->Shortcuts.QuitPiBreeze = getShortcuts( "QuitPiBreeze" );

	return settings;
};

void PBSettings::reload() {

	/* We do not need to reload if this is the first initiation */
	if ( not defaultSettings->init ) {
		defaultInstance();
		instance();
		return;
	}

	/* We do not need to reload if this is the first initiation */
	if ( not settings->init ) {
		instance();
		return;
	}

	QSettings gSettings( "PiBreeze", "PiBreeze" );
	QSettings sSettings( "PiBreeze", "Shortcuts" );

	QStringList gaKeys = gSettings.allKeys();
	QStringList saKeys = sSettings.allKeys();

	if ( gaKeys.contains( QString( "Style" ) ) )
		General.Style = gSettings.value( "Style" ).toString();

	if ( gaKeys.contains( QString( "IconTheme" ) ) )
		General.IconTheme = gSettings.value( "IconTheme" ).toString();

	if ( gaKeys.contains( QString( "TrayIcon" ) ) )
		General.TrayIcon = gSettings.value( "TrayIcon" ).toBool();

	if ( gaKeys.contains( QString( "ShowHidden" ) ) )
		General.ShowHidden = gSettings.value( "ShowHidden" ).toBool();

	if ( gaKeys.contains( QString( "IconSize" ) ) )
		General.IconSize = gSettings.value( "IconSize" ).toSize();

	if ( gaKeys.contains( QString( "SortColumn" ) ) )
		General.SortColumn = gSettings.value( "SortColumn" ).toInt();

	if ( gaKeys.contains( QString( "SortCase" ) ) )
		General.SortCase = gSettings.value( "SortCase" ).toBool();

	if ( gaKeys.contains( QString( "Grouping" ) ) )
		General.Grouping = gSettings.value( "Grouping" ).toBool();

	if ( gaKeys.contains( QString( "PerFolderViews" ) ) )
		General.PerFolderViews = gSettings.value( "PerFolderViews" ).toBool();

	if ( gaKeys.contains( QString( "FilterFolders" ) ) )
		General.FilterFolders = gSettings.value( "FilterFolders" ).toBool();

	if ( gaKeys.contains( QString( "OpenWithCatalog" ) ) )
		General.OpenWithCatalog = gSettings.value( "OpenWithCatalog" ).toBool();

	if ( gaKeys.contains( QString( "SuperStart" ) ) )
		General.SuperStart = gSettings.value( "SuperStart" ).toBool();

	if ( gaKeys.contains( QString( "SpecialOpen" ) ) )
		General.SpecialOpen = gSettings.value( "SpecialOpen" ).toBool();

	if ( gaKeys.contains( QString( "DirectIO" ) ) )
		General.DirectIO = gSettings.value( "DirectIO" ).toBool();

	if ( gaKeys.contains( QString( "Session/Geometry" ) ) )
		Session.Geometry = gSettings.value( "Session/Geometry" ).toRect();

	if ( gaKeys.contains( QString( "Session/LastDir" ) ) )
		Session.LastDir = gSettings.value( "Session/LastDir" ).toString();

	if ( gaKeys.contains( QString( "Session/Maximized" ) ) )
		Session.Maximized = gSettings.value( "Session/Maximized" ).toBool();

	if ( saKeys.contains( QString( "AboutPB" ) ) )
		Shortcuts.AboutPB = getShortcuts( "AboutPB" );

	if ( saKeys.contains( QString( "AboutQt" ) ) )
		Shortcuts.AboutQt = getShortcuts( "AboutQt" );

	if ( saKeys.contains( QString( "ToggleCrumbLE" ) ) )
		Shortcuts.ToggleCrumbLE = getShortcuts( "ToggleCrumbLE" );

	if ( saKeys.contains( QString( "AddCustomAction" ) ) )
		Shortcuts.AddCustomAction = getShortcuts( "AddCustomAction" );

	if ( saKeys.contains( QString( "GoHome" ) ) )
		Shortcuts.GoHome = getShortcuts( "GoHome" );

	if ( saKeys.contains( QString( "GoUp" ) ) )
		Shortcuts.GoUp = getShortcuts( "GoUp" );

	if ( saKeys.contains( QString( "GoLeft" ) ) )
		Shortcuts.GoLeft = getShortcuts( "GoLeft" );

	if ( saKeys.contains( QString( "GoRight" ) ) )
		Shortcuts.GoRight = getShortcuts( "GoRight" );

	if ( saKeys.contains( QString( "NewFolder" ) ) )
		Shortcuts.NewFolder = getShortcuts( "NewFolder" );

	if ( saKeys.contains( QString( "NewFile" ) ) )
		Shortcuts.NewFile = getShortcuts( "NewFile" );

	if ( saKeys.contains( QString( "NewEncFS" ) ) )
		Shortcuts.NewEncFS = getShortcuts( "NewEncFS" );

	if ( saKeys.contains( QString( "Peek" ) ) )
		Shortcuts.Peek = getShortcuts( "Peek" );

	if ( saKeys.contains( QString( "Reload" ) ) )
		Shortcuts.Reload = getShortcuts( "Reload" );

	if ( saKeys.contains( QString( "ToggleHidden" ) ) )
		Shortcuts.ToggleHidden = getShortcuts( "ToggleHidden" );

	if ( saKeys.contains( QString( "SelectAll" ) ) )
		Shortcuts.SelectAll = getShortcuts( "SelectAll" );

	if ( saKeys.contains( QString( "Cut" ) ) )
		Shortcuts.Cut = getShortcuts( "Cut" );

	if ( saKeys.contains( QString( "Copy" ) ) )
		Shortcuts.Copy = getShortcuts( "Copy" );

	if ( saKeys.contains( QString( "Paste" ) ) )
		Shortcuts.Paste = getShortcuts( "Paste" );

	if ( saKeys.contains( QString( "Rename" ) ) )
		Shortcuts.Rename = getShortcuts( "Rename" );

	if ( saKeys.contains( QString( "Delete" ) ) )
		Shortcuts.Delete = getShortcuts( "Delete" );

	if ( saKeys.contains( QString( "Trash" ) ) )
		Shortcuts.Trash = getShortcuts( "Trash" );

	if ( saKeys.contains( QString( "Properties" ) ) )
		Shortcuts.Properties = getShortcuts( "Properties" );

	if ( saKeys.contains( QString( "Permissions" ) ) )
		Shortcuts.Permissions = getShortcuts( "Permissions" );

	if ( saKeys.contains( QString( "Terminal" ) ) )
		Shortcuts.Terminal = getShortcuts( "Terminal" );

	if ( saKeys.contains( QString( "PBInfo" ) ) )
		Shortcuts.PBInfo = getShortcuts( "PBInfo" );

	if ( saKeys.contains( QString( "Settings" ) ) )
		Shortcuts.Settings = getShortcuts( "Settings" );

	if ( saKeys.contains( QString( "CustomActions" ) ) )
		Shortcuts.CustomActions = getShortcuts( "CustomActions" );

	if ( saKeys.contains( QString( "FocusAddressBar" ) ) )
		Shortcuts.FocusAddressBar = getShortcuts( "FocusAddressBar" );

	if ( saKeys.contains( QString( "NewWindow" ) ) )
		Shortcuts.NewWindow = getShortcuts( "NewWindow" );

	if ( saKeys.contains( QString( "AddBookmark" ) ) )
		Shortcuts.AddBookmark = getShortcuts( "AddBookmark" );

	if ( saKeys.contains( QString( "FocusSearchBar" ) ) )
		Shortcuts.FocusSearchBar = getShortcuts( "FocusSearchBar" );

	if ( saKeys.contains( QString( "ClearSearchBar" ) ) )
		Shortcuts.ClearSearchBar = getShortcuts( "ClearSearchBar" );

	if ( saKeys.contains( QString( "QuitPiBreeze" ) ) )
		Shortcuts.QuitPiBreeze = getShortcuts( "QuitPiBreeze" );
};

void PBSettings::setValue( QString key, QVariant value ) {

	QSettings sett( "PiBreeze", "PiBreeze" );
	sett.setValue( key, value );
	sett.sync();

	reload();
};
