/*
	*
	* PBPluginManager.cpp - Plugin Manager for PiBreeze
	*
*/

#include "PBPluginManager.hpp"

QList<PBPluginInterface::Interface> developedInterfaces;

PBPluginManager *PBPluginManager::pMgr = NULL;

PBPluginManager* PBPluginManager::instance() {

	/* If the Plugin Manager has already been init, then return the instance */
	if ( pMgr and pMgr->init )
		return pMgr;

	/* Init our plugin manager */
	pMgr = new PBPluginManager();

	/* If init succeeds, we set the init flag and return */
	pMgr->init = true;

	return pMgr;
};

bool PBPluginManager::hasPluginForMimeType( QString mimeName ) {

	if ( getuid() )
		return mimePluginMap.value( mimeName ).count() ? true : false;

	else
		return false;
};

QString PBPluginManager::pluginForMimeType( QString mimeName ) {

	if ( not getuid() )
		return QString();

	QStringList plugins = mimePluginMap.value( mimeName );

	if ( plugins.count() == 1 )
		return plugins.value( 0 );

	QString bestPlugin = plugins.value( 0 );
	Q_FOREACH( QString plugin, plugins ) {
		if ( pluginPriorityMap.value( baseName( plugin ) ) > pluginPriorityMap.value( baseName( bestPlugin ) ) )
			bestPlugin = plugin;
	}

	return bestPlugin;
};

PluginList PBPluginManager::plugins( PBPluginInterface::Interface iface, PBPluginInterface::Type type, PBPluginInterface::Context ctxt, QString mime ) {

	PluginList plugList;
	if ( not getuid() )
		return plugList;

	Q_UNUSED( type );

	PBContextPluginHash cph = mPluginsHash.value( iface );
	Q_FOREACH( PBPluginInterface *plugin, cph.value( ctxt ) ) {
		if ( plugin->mimetypes().contains( mime ) or plugin->mimetypes().contains( "*" ) )
			plugList << plugin;
	}

	return plugList;
};

PluginList PBPluginManager::allPlugins() {

	return mPluginList;
};

PBPluginManager::PBPluginManager() {

	reloadPlugins();
};

void PBPluginManager::reloadPlugins() {

	if ( not getuid() )
		return;

	reloadPeekPlugins();
	reloadOtherPlugins();
};

void PBPluginManager::reloadPeekPlugins() {
};

void PBPluginManager::reloadOtherPlugins() {

	QSettings nbsett( "PiBreeze", "Plugins" );

	QStringList pluginPaths;
	pluginPaths << nbsett.value( "PluginPaths", QStringList() << "./plugins" ).toStringList();

	int parsed = 0;
	int added = 0;

	mPluginsHash.clear();
	Q_FOREACH( QString path, pluginPaths ) {
		QDir pPathDir( path );
		Q_FOREACH( QString pluginSo, pPathDir.entryList( QStringList() << "*.so", QDir::Files, QDir::Name ) ) {
			parsed++;
			QPluginLoader loader( pPathDir.absoluteFilePath( pluginSo ) );
			QObject *pObject = loader.instance();
			if ( pObject ) {
				PBPluginInterface *plugin = qobject_cast<PBPluginInterface*>( pObject );
				if ( not plugin )
					continue;

				mPluginList << plugin;
				added++;
				Q_FOREACH( PBPluginInterface::Interface iface, plugin->interfaces() ) {
					PBContextPluginHash cph = mPluginsHash.value( iface );
					Q_FOREACH( PBPluginInterface::Context ctxt, plugin->contexts( iface ) ) {
						PluginList pl = cph.value( ctxt );
						if ( not pl.contains( plugin ) )
							pl << plugin;

						cph[ ctxt ] = pl;
					}

					mPluginsHash[ iface ] = cph;
				}
			}
		}
	}
};
