/*
    *
    * Global.hpp - Globally used header
    *
*/

#pragma once

#include <QtGui>
#include <QtNetwork>
#include <QtWidgets>

#include "pibreeze.hpp"
#include "PBSettings.hpp"

static QMimeDatabase mimeDb;

// PiBreeze Preferences
static QSettings actionSettings( "PiBreeze", "CustomActions" );
static QSettings bookmarkSettings( "PiBreeze", "Bookmarks" );

typedef QMap<QString, QString> DeviceInfo;
typedef QPair<QString, QString> BookMark;

static PBSettings *Settings = PBSettings::instance();
