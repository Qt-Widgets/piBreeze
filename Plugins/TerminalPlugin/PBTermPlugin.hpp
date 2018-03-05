/*
	*
	* PBTermPlugin.hpp - NewBreeze MarkDownPreviewWidget Class Header
	*
*/

#pragma once

#include "Global.hpp"
#include "pibreeze.hpp"
#include "PBPluginInterface.hpp"

class PBTermPlugin : public QObject, PBPluginInterface {
	Q_OBJECT
	#if QT_VERSION >= 0x050000
		Q_PLUGIN_METADATA( IID "org.NewBreeze.PBPluginInterface" FILE "TermPlugin5.json" )
	#endif
	Q_INTERFACES( PBPluginInterface )

	public:
		/* Name of the plugin */
		 QString name();

		/* The plugin version */
		 QString version();

		/* The QAction */
		 QList<QAction*> actions( Interface, QStringList );

		/* Interface type: preview, rename etc */
		 Interfaces interfaces();

		/* Interface type: preview, rename etc */
		 Type type( Interface );

		/* Plugin load context */
		 Contexts contexts( Interface );

		/* Mimetypes handled by the plugin */
		 QStringList mimetypes();

		/* Invoke slots called called by triggering the actions */
		void actionTrigger( Interface, QString, QStringList nodes );

		/* Store the called widget pointer */
		 void setCaller( QWidget *caller );
};
