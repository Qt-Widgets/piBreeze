/*
	*
	* PBSKeyBindingsWidget.cpp - The GUI classes for PiBreeze
	*
*/

#include "PBSKeyBindingsWidget.hpp"

PBKeyBindingsWidget::PBKeyBindingsWidget( QWidget *parent ) : QWidget( parent ) {

	QScrollArea *scroller = new QScrollArea();
	scroller->setStyleSheet( scroller->styleSheet() + "\nQScrollArea { border: none; }" );
	scroller->setWidgetResizable( true );

	QGridLayout *keysLyt = new QGridLayout();
	keysLyt->setSpacing( 0 );

	PBKeySequenceButton *aboutPBKS = new PBKeySequenceButton( Settings->Shortcuts.AboutPB.at( 0 ), PBSettings::defaultInstance()->Shortcuts.AboutPB.at( 0 ), "AboutPB", this );
	PBKeySequenceButton *toggleCrumbLEKS = new PBKeySequenceButton( Settings->Shortcuts.ToggleCrumbLE.at( 0 ), PBSettings::defaultInstance()->Shortcuts.ToggleCrumbLE.at( 0 ), "ToggleCrumbLE", this );
	PBKeySequenceButton *addCustomActionKS = new PBKeySequenceButton( Settings->Shortcuts.AddCustomAction.at( 0 ), PBSettings::defaultInstance()->Shortcuts.AddCustomAction.at( 0 ), "AddCustomAction", this );
	PBKeySequenceButton *goHomeKS = new PBKeySequenceButton( Settings->Shortcuts.GoHome.at( 0 ), PBSettings::defaultInstance()->Shortcuts.GoHome.at( 0 ), "GoHome", this );
	PBKeySequenceButton *goUpKS = new PBKeySequenceButton( Settings->Shortcuts.GoUp.at( 0 ), PBSettings::defaultInstance()->Shortcuts.GoUp.at( 0 ), "GoUp", this );
	PBKeySequenceButton *goLeftKS = new PBKeySequenceButton( Settings->Shortcuts.GoLeft.at( 0 ), PBSettings::defaultInstance()->Shortcuts.GoLeft.at( 0 ), "GoLeft", this );
	PBKeySequenceButton *goRightKS = new PBKeySequenceButton( Settings->Shortcuts.GoRight.at( 0 ), PBSettings::defaultInstance()->Shortcuts.GoRight.at( 0 ), "GoRight", this );
	PBKeySequenceButton *newFolderKS = new PBKeySequenceButton( Settings->Shortcuts.NewFolder.at( 0 ), PBSettings::defaultInstance()->Shortcuts.NewFolder.at( 0 ), "NewFolder", this );
	PBKeySequenceButton *newFileKS = new PBKeySequenceButton( Settings->Shortcuts.NewFile.at( 0 ), PBSettings::defaultInstance()->Shortcuts.NewFile.at( 0 ), "NewFile", this );
	PBKeySequenceButton *peekKS = new PBKeySequenceButton( Settings->Shortcuts.Peek.at( 0 ), PBSettings::defaultInstance()->Shortcuts.Peek.at( 0 ), "Peek", this );
	PBKeySequenceButton *reloadKS = new PBKeySequenceButton( Settings->Shortcuts.Reload.at( 0 ), PBSettings::defaultInstance()->Shortcuts.Reload.at( 0 ), "Reload", this );
	PBKeySequenceButton *toggleHiddenKS = new PBKeySequenceButton( Settings->Shortcuts.ToggleHidden.at( 0 ), PBSettings::defaultInstance()->Shortcuts.ToggleHidden.at( 0 ), "ToggleHidden", this );
	PBKeySequenceButton *selectAllKS = new PBKeySequenceButton( Settings->Shortcuts.SelectAll.at( 0 ), PBSettings::defaultInstance()->Shortcuts.SelectAll.at( 0 ), "SelectAll", this );
	PBKeySequenceButton *cutKS = new PBKeySequenceButton( Settings->Shortcuts.Cut.at( 0 ), PBSettings::defaultInstance()->Shortcuts.Cut.at( 0 ), "Cut", this );
	PBKeySequenceButton *copyKS = new PBKeySequenceButton( Settings->Shortcuts.Copy.at( 0 ), PBSettings::defaultInstance()->Shortcuts.Copy.at( 0 ), "Copy", this );
	PBKeySequenceButton *pasteKS = new PBKeySequenceButton( Settings->Shortcuts.Paste.at( 0 ), PBSettings::defaultInstance()->Shortcuts.Paste.at( 0 ), "Paste", this );
	PBKeySequenceButton *renameKS = new PBKeySequenceButton( Settings->Shortcuts.Rename.at( 0 ), PBSettings::defaultInstance()->Shortcuts.Rename.at( 0 ), "Rename", this );
	PBKeySequenceButton *deleteKS = new PBKeySequenceButton( Settings->Shortcuts.Delete.at( 0 ), PBSettings::defaultInstance()->Shortcuts.Delete.at( 0 ), "Delete", this );
	PBKeySequenceButton *trashKS = new PBKeySequenceButton( Settings->Shortcuts.Trash.at( 0 ), PBSettings::defaultInstance()->Shortcuts.Trash.at( 0 ), "Trash", this );
	PBKeySequenceButton *propertiesKS = new PBKeySequenceButton( Settings->Shortcuts.Properties.at( 0 ), PBSettings::defaultInstance()->Shortcuts.Properties.at( 0 ), "Properties", this );
	PBKeySequenceButton *terminalKS = new PBKeySequenceButton( Settings->Shortcuts.Terminal.at( 0 ), PBSettings::defaultInstance()->Shortcuts.Terminal.at( 0 ), "Terminal", this );
	PBKeySequenceButton *nbInfoKS = new PBKeySequenceButton( Settings->Shortcuts.PBInfo.at( 0 ), PBSettings::defaultInstance()->Shortcuts.PBInfo.at( 0 ), "PBInfo", this );
	PBKeySequenceButton *settingsKS = new PBKeySequenceButton( Settings->Shortcuts.Settings.at( 0 ), PBSettings::defaultInstance()->Shortcuts.Settings.at( 0 ), "Settings", this );
	PBKeySequenceButton *customActionsKS = new PBKeySequenceButton( Settings->Shortcuts.CustomActions.at( 0 ), PBSettings::defaultInstance()->Shortcuts.CustomActions.at( 0 ), "CustomActions", this );
	PBKeySequenceButton *focusAddressBarKS = new PBKeySequenceButton( Settings->Shortcuts.FocusAddressBar.at( 0 ), PBSettings::defaultInstance()->Shortcuts.FocusAddressBar.at( 0 ), "FocusAddressBar", this );
	PBKeySequenceButton *newWindowKS = new PBKeySequenceButton( Settings->Shortcuts.NewWindow.at( 0 ), PBSettings::defaultInstance()->Shortcuts.NewWindow.at( 0 ), "NewWindow", this );
	PBKeySequenceButton *addBookmarkKS = new PBKeySequenceButton( Settings->Shortcuts.AddBookmark.at( 0 ), PBSettings::defaultInstance()->Shortcuts.AddBookmark.at( 0 ), "AddBookmark", this );
	PBKeySequenceButton *focusSearchBarKS = new PBKeySequenceButton( Settings->Shortcuts.FocusSearchBar.at( 0 ), PBSettings::defaultInstance()->Shortcuts.FocusSearchBar.at( 0 ), "FocusSearchBar", this );
	PBKeySequenceButton *clearSearchBarKS = new PBKeySequenceButton( Settings->Shortcuts.ClearSearchBar.at( 0 ), PBSettings::defaultInstance()->Shortcuts.ClearSearchBar.at( 0 ), "ClearSearchBar", this );

	keysLyt->addWidget( new QLabel( "AboutPB" ), 0, 0 );
	keysLyt->addWidget( new QLabel( "Toggle CrumbsBar" ), 1, 0 );
	keysLyt->addWidget( new QLabel( "Add Custom Action" ), 3, 0 );
	keysLyt->addWidget( new QLabel( "Go Home" ), 4, 0 );
	keysLyt->addWidget( new QLabel( "Go Up" ), 5, 0 );
	keysLyt->addWidget( new QLabel( "Go Prev Dir" ), 6, 0 );
	keysLyt->addWidget( new QLabel( "Go Next Dir" ), 7, 0 );
	keysLyt->addWidget( new QLabel( "New Folder" ), 8, 0 );
	keysLyt->addWidget( new QLabel( "New File" ), 9, 0 );
	keysLyt->addWidget( new QLabel( "Peek" ), 10, 0 );
	keysLyt->addWidget( new QLabel( "Reload" ), 11, 0 );
	keysLyt->addWidget( new QLabel( "Toggle Hidden Files" ), 12, 0 );
	keysLyt->addWidget( new QLabel( "Select All" ), 13, 0 );
	keysLyt->addWidget( new QLabel( "Cut" ), 14, 0 );
	keysLyt->addWidget( new QLabel( "Copy" ), 15, 0 );
	keysLyt->addWidget( new QLabel( "Paste" ), 16, 0 );
	keysLyt->addWidget( new QLabel( "Rename" ), 17, 0 );
	keysLyt->addWidget( new QLabel( "Delete" ), 18, 0 );
	keysLyt->addWidget( new QLabel( "Trash" ), 19, 0 );
	keysLyt->addWidget( new QLabel( "Properties" ), 20, 0 );
	keysLyt->addWidget( new QLabel( "Terminal" ), 21, 0 );
	keysLyt->addWidget( new QLabel( "Show PBInfo" ), 22, 0 );
	keysLyt->addWidget( new QLabel( "Settings" ), 23, 0 );
	keysLyt->addWidget( new QLabel( "Custom Actions" ), 24, 0 );
	keysLyt->addWidget( new QLabel( "Focus AddressBar" ), 25, 0 );
	keysLyt->addWidget( new QLabel( "New Window" ), 26, 0 );
	keysLyt->addWidget( new QLabel( "Add Bookmark" ), 27, 0 );
	keysLyt->addWidget( new QLabel( "Focus SearchBar" ), 28, 0 );
	keysLyt->addWidget( new QLabel( "Clear SearchBar" ), 29, 0 );

	keysLyt->addWidget( aboutPBKS, 0, 1 );
	keysLyt->addWidget( toggleCrumbLEKS, 1, 1 );
	keysLyt->addWidget( addCustomActionKS, 3, 1 );
	keysLyt->addWidget( goHomeKS, 4, 1 );
	keysLyt->addWidget( goUpKS, 5, 1 );
	keysLyt->addWidget( goLeftKS, 6, 1 );
	keysLyt->addWidget( goRightKS, 7, 1 );
	keysLyt->addWidget( newFolderKS, 8, 1 );
	keysLyt->addWidget( newFileKS, 9, 1 );
	keysLyt->addWidget( peekKS, 10, 1 );
	keysLyt->addWidget( reloadKS, 11, 1 );
	keysLyt->addWidget( toggleHiddenKS, 12, 1 );
	keysLyt->addWidget( selectAllKS, 13, 1 );
	keysLyt->addWidget( cutKS, 14, 1 );
	keysLyt->addWidget( copyKS, 15, 1 );
	keysLyt->addWidget( pasteKS, 16, 1 );
	keysLyt->addWidget( renameKS, 17, 1 );
	keysLyt->addWidget( deleteKS, 18, 1 );
	keysLyt->addWidget( trashKS, 19, 1 );
	keysLyt->addWidget( propertiesKS, 20, 1 );
	keysLyt->addWidget( terminalKS, 21, 1 );
	keysLyt->addWidget( nbInfoKS, 22, 1 );
	keysLyt->addWidget( settingsKS, 23, 1 );
	keysLyt->addWidget( customActionsKS, 24, 1 );
	keysLyt->addWidget( focusAddressBarKS, 25, 1 );
	keysLyt->addWidget( newWindowKS, 26, 1 );
	keysLyt->addWidget( addBookmarkKS, 27, 1 );
	keysLyt->addWidget( focusSearchBarKS, 28, 1 );
	keysLyt->addWidget( clearSearchBarKS, 29, 1 );

	QWidget *scrollWidget = new QWidget( this );
	scrollWidget->setLayout( keysLyt );

	scroller->setWidget( scrollWidget );

	QHBoxLayout *baseLyt = new QHBoxLayout();
	baseLyt->setContentsMargins( QMargins() );
	baseLyt->addWidget( scroller );

	setLayout( baseLyt );
};

PBKeyCaptureWidget::PBKeyCaptureWidget( QKeySequence old, QKeySequence defKey, QWidget *parent ) : QDialog( parent ) {

	newKey = old;
	defaultKey = defKey;

	setWindowModality( Qt::WindowModal );

	setFixedSize( QSize( 500, 150 ) );
	setFocusPolicy( Qt::StrongFocus );

	QLabel *lbl = new QLabel( "Press in the key combination of your choice, BackSpace clears the key combo." );
	lbl->setWordWrap( true );

	QPushButton *okBtn = new QPushButton( QIcon::fromTheme( "dialog-ok-apply", QIcon( ":/icons/ok.png" ) ), "Apply" );
	okBtn->setFocusPolicy( Qt::NoFocus );
	connect( okBtn, SIGNAL( clicked() ), this, SLOT( apply() ) );

	QPushButton *cancelBtn = new QPushButton( QIcon::fromTheme( "edit-delete" ), "Cancel" );
	cancelBtn->setDefault( false );
	cancelBtn->setFocusPolicy( Qt::NoFocus );
	connect( cancelBtn, SIGNAL( clicked() ), this, SLOT( cancel() ) );

	QPushButton *clearBtn = new QPushButton( QIcon::fromTheme( "edit-clear" ), "Clear" );
	clearBtn->setDefault( false );
	clearBtn->setFocusPolicy( Qt::NoFocus );
	connect( clearBtn, SIGNAL( clicked() ), this, SLOT( clear() ) );

	QPushButton *resetBtn = new QPushButton( QIcon::fromTheme( "view-refresh", QIcon( ":/icons/reload.png" ) ), "Reset" );
	resetBtn->setDefault( false );
	resetBtn->setFocusPolicy( Qt::NoFocus );
	connect( resetBtn, SIGNAL( clicked() ), this, SLOT( reset() ) );

	QVBoxLayout *lyt = new QVBoxLayout();
	QHBoxLayout *bLyt = new QHBoxLayout();

	bLyt->addWidget( clearBtn );
	bLyt->addWidget( resetBtn );
	bLyt->addStretch();
	bLyt->addWidget( cancelBtn );
	bLyt->addWidget( okBtn );

	lyt->addWidget( lbl );
	lyt->addStretch();
	lyt->addLayout( bLyt );

	setLayout( lyt );
};

bool PBKeyCaptureWidget::event( QEvent *e ) {

	if ( e->type() == QEvent::KeyPress ) {

		keyPressEvent( static_cast<QKeyEvent*>( e ) );
		return true;
	}

	if ( e->type() == QEvent::ShortcutOverride ) {

		e->accept();
		return true;
	}

	if ( e->type() == QEvent::FocusIn ) {

		grabKeyboard();
		return true;
	}

	if ( e->type() == QEvent::FocusOut ) {

		releaseKeyboard();
		return true;
	}

	return QWidget::event( e );
};

void PBKeyCaptureWidget::keyPressEvent( QKeyEvent *keyEvent ) {

	newKey = QKeySequence();
	repaint();

	int keyQt =  keyEvent->key();

	/*
		*
		* Qt sometimes returns garbage keycodes if it doesn't know a key.
		* We cannot do anything useful with those ( several keys have -1, indistinguishable )
		* and QKeySequence.toString() will also yield a garbage string.
		*
	*/
	if ( keyQt == -1 ) {

		// keu moy supported in Qt
		keyEvent->ignore();
		return;
	}

	//get modifiers key
	modifierKeys = keyEvent->modifiers() & ( Qt::SHIFT | Qt::CTRL | Qt::ALT | Qt::META );

	// switching key type
	switch( keyQt ) {

		case Qt::Key_AltGr: //or else we get unicode salad
		case Qt::Key_Shift:
		case Qt::Key_Control:
		case Qt::Key_Alt:
		case Qt::Key_Meta:
		case Qt::Key_Menu: {
			break;
		}

		case Qt::Key_Backspace: {

			// Modifiers + BackSpace
			if ( modifierKeys ) {
				newKey = QKeySequence( keyQt |= modifierKeys );
				break;
			}

			// This is our reset
			else {
				newKey = QKeySequence( "");
				repaint();
				break;
			}
		}

		default: {
			// We now have a valid key press.
			if ( keyQt ) {
				if ( ( keyQt == Qt::Key_Backtab ) && ( modifierKeys & Qt::SHIFT ) ) {

					keyQt = Qt::Key_Tab | modifierKeys;
				}

				else {

					keyQt |= modifierKeys;
				}

				newKey = QKeySequence( keyQt );
			}
		}
	}

	repaint();
	keyEvent->accept();
};

void PBKeyCaptureWidget::keyReleaseEvent( QKeyEvent *keyEvent ) {

	if ( keyEvent->key() == -1 ) {

		keyEvent->ignore();
		return;
	}

	keyEvent->accept();
};

void PBKeyCaptureWidget::paintEvent( QPaintEvent *pEvent ) {

	QWidget::paintEvent( pEvent );

	QPainter *painter = new QPainter( this );
	painter->setRenderHints( QPainter::Antialiasing );

	QString str = newKey.toString();

	if ( not str.isEmpty() )
		painter->drawText( QRect( 0, 65, width(), 20 ), Qt::AlignCenter, str );

	else {
		if ( modifierKeys ) {
			if ( ( modifierKeys & Qt::META ) )
				str += "Meta+";

			if ( ( modifierKeys & Qt::CTRL ) )
				str += "Ctrl+";

			if ( ( modifierKeys & Qt::ALT ) )
				str += "Alt+";

			if ( ( modifierKeys & Qt::SHIFT ) )
				str += "Shift+";
		}

		painter->drawText( QRect( 0, 65, width(), 20 ), Qt::AlignCenter, str );
	}

	painter->end();
};

void PBKeyCaptureWidget::show() {

	QWidget::show();
	grabKeyboard();
};

void PBKeyCaptureWidget::apply() {

	emit keySequenceChanged( newKey );
	close();
};

void PBKeyCaptureWidget::cancel() {

	close();
};

void PBKeyCaptureWidget::clear() {

	newKey = QKeySequence();
	repaint();
};

void PBKeyCaptureWidget::reset() {

	newKey = QKeySequence( defaultKey );
	repaint();
};

PBKeySequenceButton::PBKeySequenceButton( QKeySequence key, QKeySequence defaultSeq, QString name, QWidget *parent ) : QPushButton( parent ) {

	defaultSequence = QKeySequence( defaultSeq );
	shortcutName = QString( name );

	setIcon( QIcon( ":/icons/shortcuts.png" ) );
	setText( key.toString() );

	connect( this, SIGNAL( clicked() ), this, SLOT( changeKeySequence() ) );
};

void PBKeySequenceButton::changeKeySequence() {

	PBKeyCaptureWidget *capturer = new PBKeyCaptureWidget( text(), defaultSequence, qobject_cast<QWidget*>( parent() ) );
	connect( capturer, SIGNAL( keySequenceChanged( QKeySequence ) ), this, SLOT( setKeySequence( QKeySequence ) ) );

	capturer->show();
};

void PBKeySequenceButton::setKeySequence( QKeySequence newKey ) {

	setText( newKey.toString() );

	QSettings shortcuts( "PiBreeze", "Shortcuts" );
	shortcuts.setValue( shortcutName, newKey.toString() );
	shortcuts.sync();

	Settings->reload();
};
