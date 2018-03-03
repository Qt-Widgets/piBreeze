/*
	*
	* PBAppFile.hpp - PBAppFile.cpp header
	*
*/

#pragma once

#include "common.hpp"
#include "PBXdg.hpp"
#include "PBTools.hpp"

class PBCOMMON_DLLSPEC PBAppFile {

	public:
		enum Fields {
			Name,
			Type,
			Exec,
			Icon,
			MimeTypes,
			WorkPath,
			TerminalMode,
			Categories,
			Comment,
			Description,
			NoDisplay
		};

		PBAppFile();
		PBAppFile( QString );

		// Value of various fields
		QVariant value( PBAppFile::Fields ) const;

		// Value of various fields
		void setValue( PBAppFile::Fields, QVariant );

		// Contains only the arguments, expanded wherever necessary accoring to fredesktop.org standards
		QStringList execArgs() const;

		// Can handle multiple input files?
		bool multipleArgs() const;

		// Does it take arguments
		bool takesArgs() const;

		// Grade
		short grade() const;

		// Name of the desktop file
		QString desktopFileName() const;

		// Name of the desktop file
		QString filePath() const;

		// Compare this and other
		// this.grade() - other.grade()
		int compare( PBAppFile ) const;

		// Category if this PBAppFile
		QString category() const;

		// Merge the other PBAppFile with this
		void merge( PBAppFile other );

		// Save this desktop file
		bool save();

		// Merge two PBAppFiles
		static PBAppFile merge( PBAppFile first, PBAppFile second );

		// Does this instance contain vaild data?
		bool isValid();

		// Check if this PBAppFile is equivalent ot @other
		bool operator==( const PBAppFile& ) const;

	private:
		PBAppFile( QVariantList );

		void parseDesktopFile();
		void generateExecArgs();

		QString fileUrl;

		// variables
		QString __name;
		QString __type;
		QString __exec;

		QString __icon;
		QStringList __mimeTypes;
		QString __workPath;
		bool __terminalMode;

		QStringList __categories;
		QString __category;
		QString __description;
		QString __comment;
		bool __nodisplay;

		QStringList __execArgs;
		bool __multipleFiles;
		bool __takesArgs;

		short __grade;

		bool mIsValid;
};

class PBCOMMON_DLLSPEC PBAppsList {
	public:
		PBAppsList();

		void clear();
		int count();
		void move( int, int );

		PBAppFile at( quint64 );
		PBAppFile value( quint64 );
		int indexOf( PBAppFile );
		void insert( int, PBAppFile );

		bool contains( PBAppFile );
		void removeDuplicates();

		void remove( PBAppFile );
		void remove( quint64 );

		QList<PBAppFile> toQList();
		QList<PBAppFile> toQList() const;

		PBAppsList operator<<( PBAppsList );
		PBAppsList operator<<( PBAppFile );
		PBAppsList operator=( PBAppsList );
		bool operator==( PBAppsList );
		PBAppFile operator[]( int );

	private:
		QList<PBAppFile> __appsList;
		bool __clearedOfDuplicates;
};

uint qHash( const PBAppFile &app );
