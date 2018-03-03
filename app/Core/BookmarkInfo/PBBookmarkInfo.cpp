/*
	*
	* PBBookmarkInfo.cpp - Booksmarks Info class
	*
*/

#include "PBBookmarkInfo.hpp"

QList<PBBookmarkInfo> PBBookmarkInfo::allBookmarks() {

	QList<PBBookmarkInfo> bookmarkInfoList;

	foreach( QString bookmark, bookmarkSettings.value( "Order" ).toStringList() ) {
		if ( not exists( bookmark ) )
			continue;

		QString label = bookmarkSettings.value( QUrl::toPercentEncoding( bookmark ) ).toString();

		PBBookmarkInfo bmkInfo;
		bmkInfo.displayLabel = label;
		bmkInfo.displayIcon = "bookmarks";
		bmkInfo.mountPoint = bookmark;

		bookmarkInfoList << bmkInfo;
	}

	return bookmarkInfoList;
};
