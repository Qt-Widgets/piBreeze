/*
	*
	* PBTermPlugin.cpp - NewBreeze MarkDownPreviewWidget Class
	*
*/

#include "PBTermPlugin.hpp"
#include "PBTerminal.hpp"

/* Name of the plugin */
QString PBTermPlugin::name() {

	return "TermPlugin";
};

/* The plugin version */
QString PBTermPlugin::version() {

	return "1.0.0";
};

/* The QAction hooks for menus/toolbars */
QList<QAction*> PBTermPlugin::actions( Interface, QStringList nodes ) {

	if ( ( nodes.count() == 1 ) and isDir( nodes.at( 0 ) ) ) {
		QAction *act = new QAction( QIcon( ":/icons/emblem-unmounted.png" ), "&Peek", this );

		PBTerminal *term = new PBTerminal( nodes.at( 0 ) );
		connect( act, SIGNAL( triggered() ), term, SLOT( showMaximized() ) );

		return QList<QAction*>() << act;
	}

	return QList<QAction*>();
};

/* Interface type: preview, rename etc */
PBPluginInterface::Interfaces PBTermPlugin::interfaces() {

	return  PBPluginInterface::Interfaces() << PBPluginInterface::TerminalInterface;
};

/* Interface type: preview, rename etc */
PBPluginInterface::Type PBTermPlugin::type( Interface ) {

	return PBPluginInterface::Enhancement;
};

/* Plugin load contexts */
PBPluginInterface::Contexts PBTermPlugin::contexts( Interface ) {

	return Contexts() << PBPluginInterface::Dir;
};

/* Mimetypes handled by the plugin */
QStringList PBTermPlugin::mimetypes() {

	return QStringList() << "inode/directory";
};

/* Invoke slots called called by triggering the actions */
void PBTermPlugin::actionTrigger( Interface, QString, QStringList nodes ) {

	if ( ( nodes.count() == 1 ) and isDir( nodes.at( 0 ) ) ) {
		PBTerminal *term = new PBTerminal( nodes.at( 0 ) );
		term->showMaximized();
	}
};

void PBTermPlugin::setCaller( QWidget *caller ) {

	mParent = caller;
};

#if QT_VERSION < 0x050000
	Q_EXPORT_PLUGIN2( TermPlugin, PBTermPlugin );
#endif
