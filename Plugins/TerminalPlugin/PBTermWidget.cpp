/*
    *
    * * PBTermWidget.cpp - QTermWidget reimplementation for PBTerminal
    *
*/

#include "PBTermWidget.hpp"

PBTermWidget::PBTermWidget( QWidget *parent ) : QTermWidget( 0, parent ) {

	if ( settings.value( "EnableTransparency" ).toBool() )
		setTerminalOpacity( settings.value( "Opacity" ).toReal() );

	/* Set the enivronment variable TERM as xterm */
	QProcessEnvironment procEnv = QProcessEnvironment::systemEnvironment();
	procEnv.insert( "TERM", "xterm-256color" );
	setEnvironment( procEnv.toStringList() );

	if ( not watcher->files().contains( settings.fileName() ) )
		watcher->addPath( settings.fileName() );

	connect( watcher, SIGNAL( fileChanged( QString ) ), this, SLOT( reloadSettings() ) );

	setColorScheme( settings.value( "ColorScheme" ).toString() );
	setScrollBarPosition( QTermWidget::ScrollBarRight );
	setStyleSheet( getStyleSheet( "QTermWidget" ) );

	setTerminalFont( settings.value( "Font" ).value<QFont>() );
	setHistorySize( 10000 );										// Infinte scroll back

	setShellProgram( "/bin/bash" );

	setMotionAfterPasting( 2 );
	setFlowControlEnabled( true );
	setFlowControlWarningEnabled( true );

	startShellProgram();
	watcher->addPath( QString( "/proc/%1/" ).arg( getShellPID() ) );
	oldCWD = QFileInfo( QString( "/proc/%1/cwd" ).arg( getShellPID() ) ).symLinkTarget();
	connect( watcher, SIGNAL( directoryChanged( const QString& ) ), this, SLOT( handleFSWSignals( QString ) ) );

	setFocus();
};

PBTermWidget::PBTermWidget( QString wDir, QWidget *parent ) : QTermWidget( 0, parent ) {

	if ( settings.value( "EnableTransparency" ).toBool() )
		setTerminalOpacity( settings.value( "Opacity" ).toReal() );

	/* Set the enivronment variable TERM as xterm */
	QProcessEnvironment procEnv = QProcessEnvironment::systemEnvironment();
	procEnv.insert( "TERM", "xterm-256color" );
	setEnvironment( procEnv.toStringList() );

	if ( not watcher->files().contains( settings.fileName() ) )
		watcher->addPath( settings.fileName() );

	connect( watcher, SIGNAL( fileChanged( QString ) ), this, SLOT( reloadSettings() ) );

	setColorScheme( settings.value( "ColorScheme" ).toString() );
	setScrollBarPosition( QTermWidget::ScrollBarRight );
	setStyleSheet( getStyleSheet( "QTermWidget" ) );

	setTerminalFont( settings.value( "Font" ).value<QFont>() );
	setHistorySize( 10000 );										// Infinte scroll back

	setWorkingDirectory( wDir );
	setShellProgram( "/bin/bash" );

	setMotionAfterPasting( 2 );
	setFlowControlEnabled( true );
	setFlowControlWarningEnabled( true );

	startShellProgram();

	setFocus();
};

PBTermWidget::PBTermWidget( QString wDir, QString cmd, QWidget *parent ) : QTermWidget( 0, parent ) {

	if ( settings.value( "EnableTransparency" ).toBool() )
		setTerminalOpacity( settings.value( "Opacity" ).toReal() );

	/* Set the enivronment variable TERM as xterm */
	QProcessEnvironment procEnv = QProcessEnvironment::systemEnvironment();
	procEnv.insert( "TERM", "xterm-256color" );
	setEnvironment( procEnv.toStringList() );

	if ( not watcher->files().contains( settings.fileName() ) )
		watcher->addPath( settings.fileName() );

	connect( watcher, SIGNAL( fileChanged( QString ) ), this, SLOT( reloadSettings() ) );

	setColorScheme( settings.value( "ColorScheme" ).toString() );
	setScrollBarPosition( QTermWidget::ScrollBarRight );
	setStyleSheet( getStyleSheet( "QTermWidget" ) );

	setTerminalFont( settings.value( "Font" ).value<QFont>() );
	setHistorySize( 10000 );

	setWorkingDirectory( wDir );
	QStringList shArgs = QStringList() << "-il" << "-c" << cmd;			//.split( QRegExp( "\\s+" ), QString::SkipEmptyParts );

	setShellProgram( "/bin/bash" );
	if ( not cmd.isEmpty() )
		setArgs( shArgs );

	setMotionAfterPasting( 2 );
	setFlowControlEnabled( true );
	setFlowControlWarningEnabled( true );

	startShellProgram();

	setFocus();
};

QString PBTermWidget::currentWorkingDirectory() {

	QString cwd = QString( "/proc/%1/cwd" ).arg( getShellPID() );
	return QFileInfo( cwd ).symLinkTarget();
};

void PBTermWidget::reloadSettings() {

	if ( settings.value( "EnableTransparency" ).toBool() )
		setTerminalOpacity( settings.value( "Opacity" ).toReal() );

	else
		setTerminalOpacity( 1 );

	setColorScheme( settings.value( "ColorScheme" ).toString() );
	setTerminalFont( settings.value( "Font" ).value<QFont>() );
};

void PBTermWidget::handleFSWSignals( QString dir ) {

	if ( QFileInfo( QString( "/proc/%1/cwd" ).arg( getShellPID() ) ).symLinkTarget() == oldCWD )
		return;

	oldCWD = QFileInfo( QString( "/proc/%1/cwd" ).arg( getShellPID() ) ).symLinkTarget();
	emit chDir( oldCWD );
};
