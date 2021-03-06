/*
	*
	* PBPluginManager.hpp - PBPluginManager.cpp Header
	*
*/

#pragma once

#include "Global.hpp"
#include "PBPluginInterface.hpp"

/* PluginList */
typedef QList<PBPluginInterface*> PluginList;

/* PluginList of Context <int> */
typedef QHash<int, PluginList> PBContextPluginHash;

class PBPluginManager : public QObject {
	Q_OBJECT

	public:
		/* Create a global instance of PBPluginManager */
		static PBPluginManager* instance();

		/* Return the plugin path for the given mimetype */
		bool hasPluginForMimeType( QString );

		/* Return the plugin path for the given mimetype */
		QString pluginForMimeType( QString );

		/* Return the plugin path for the given setup */
		PluginList plugins( PBPluginInterface::Interface, PBPluginInterface::Type, PBPluginInterface::Context, QString );

		/* Return all the unique plugins */
		PluginList allPlugins();

	public Q_SLOTS:
		/* Reload the plugins map */
		void reloadPlugins();

	private:
		PBPluginManager();

		void reloadPeekPlugins();
		void reloadOtherPlugins();

		/* Global Instance */
		static PBPluginManager* pMgr;

		/* Init flag */
		bool init;

		/* PluginList */
		PluginList mPluginList;

		/* MimeType-PluginPath map */
		QMap<QString, QStringList> mimePluginMap;
		QMap<QString, int> pluginPriorityMap;

		/* Interface-Context-PluginList Hash */
		QHash<int, PBContextPluginHash> mPluginsHash;
};
