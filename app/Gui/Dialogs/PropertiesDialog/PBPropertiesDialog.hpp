/*
	*
	* PBPropertiesDialog.hpp - PBPropertiesDialog.cpp header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBDialog.hpp"
#include "PBGuiWidgets.hpp"
#include "PBLabels.hpp"
#include "PBIconManager.hpp"
#include "PBFileDialog.hpp"

#if QT_VERSION >= 0x050000
	#include <QtConcurrent>
#endif

class PBPropertiesBase: public QWidget {
	Q_OBJECT

	public:
		PBPropertiesBase( QStringList, QWidget * );
		void setNewIcon( QString );

	private:
		PBClickLabel *iconLabel;
		QLabel *infoLabel;
		QStringList pathsList;

	Q_SIGNALS:
		void setDirIcon();
};

class PBPropertiesWidget: public QWidget {
    Q_OBJECT

	public:
		PBPropertiesWidget( QStringList, bool *term, QWidget * );
		~PBPropertiesWidget();

	private:
		void createGUI();
		void setWidgetProperties();

		bool *terminate;

	public slots:
		void update();

	private:
		void folderProperties( QStringList paths );
		void recurseProperties( QString path );

		QString sizeTypeStr;
		QLabel *sizeTypeLabel;

		QLabel *timeLabel;

		QStringList pathsList;

		QFuture<void> thread;

		qint64 files;
		qint64 folders;
		qint64 totalSize;

		QDateTime ctimeMin, mtimeMin, atimeMin;
		QDateTime ctimeMax, mtimeMax, atimeMax;

	Q_SIGNALS:
		void updateSignal();
};

class PBPermissionsWidget: public QWidget {
    Q_OBJECT

	public:
		PBPermissionsWidget( QStringList, QWidget * );

	private:
		void createGUI();
		void setWidgetProperties();
		void readPermissions();

		PBClickLabel *iconLabel;

		QStringList pathsList;

		QCheckBox *uReadCheck, *uWritCheck, *uExecCheck;
		QCheckBox *gReadCheck, *gWritCheck, *gExecCheck;
		QCheckBox *oReadCheck, *oWritCheck, *oExecCheck;

		QCheckBox *smartExecCheck;
		QCheckBox *carryCheck;
		QCheckBox *delProtectCheck;

	private slots:
		void applyPermissions();
		void applyTo( const char*, QFile::Permissions );
		void applyProtection();

		void setReadAll();
		void setWriteOwner();
		void setExecOwner();
};

void smartExecutable( QString );

/*
	*
	* We want a independent, non modal dialog showing properties,
	*
*/
class PBPropertiesDialog: public PBDialog {
	Q_OBJECT


	public:
		Q_ENUMS( PropertiesTab )

		enum PropertiesTab {
			Properties  = 0x00,
			Permissions = 0x01,
			OpenWith    = 0x02
		};

		PBPropertiesDialog( QStringList paths, PropertiesTab tab, bool *term, QWidget *parent = 0 );

	private:
		QStackedWidget *stack;
		QTabBar *tabs;

		PBPropertiesBase *propsB;
		PBPropertiesWidget *propsW;
		PBPermissionsWidget *permsW;

		QStringList pathsList;

	private slots:
		void switchToTab( int  );
		void setDirIcon();
};
