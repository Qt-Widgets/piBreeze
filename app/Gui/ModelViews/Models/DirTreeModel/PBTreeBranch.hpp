/*
	*
	* PBTreeBranch.hpp - PiBreeze PBTreeBranch Class Header
	*
*/

#pragma once

#include "Global.hpp"
#include "pibreeze.hpp"

class PBTreeBranch : public QObject {
	Q_OBJECT

	public:
		PBTreeBranch();
		PBTreeBranch( QString, QIcon, PBTreeBranch *parent = 0 );

		int childCount();

		void clearChildren();

		void addChild( PBTreeBranch* );
		void removeChild( PBTreeBranch* );

		PBTreeBranch* child( int );
		PBTreeBranch* child( QString );
		QList<PBTreeBranch*> children();

		QVariant data( int role ) const;
		bool setData( int column, QVariant data );

		/* Filters */
		QStringList nameFilters();
		void setNameFilters( QStringList );
		void clearNameFilters();

		bool showHidden() const;
		void setShowHidden( bool );

		PBTreeBranch *parent();
		int row();

		void sort();

		QString name();
		QIcon icon();

	private:
		QList<PBTreeBranch*> childNodes;
		PBTreeBranch *parentNode;

		QString mPath;
		QIcon mIcon;

		QStringList __nameFilters;
		bool __showHidden;

	public Q_SLOTS:
		void explore();
};

bool caseInsensitiveNameSort( PBTreeBranch *first, PBTreeBranch *second );
bool matchesFilter( QStringList filters, QString text );
