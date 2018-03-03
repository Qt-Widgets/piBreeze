/*
	*
	* PBSKeyBindingsWidget.hpp - PBSKeyBindingsWidget.cpp header
	*
*/

#pragma once

#include "Global.hpp"

class PBKeyBindingsWidget : public QWidget {
	Q_OBJECT

	public:
		PBKeyBindingsWidget( QWidget *parent );

	private:
		void createGUI();
};

class PBKeyCaptureWidget : public QDialog {
    Q_OBJECT

	public:
		PBKeyCaptureWidget( QKeySequence oldSeq = 0, QKeySequence defaultKey = 0, QWidget *parent = 0 );

	protected:
		bool event( QEvent *e );
		void keyPressEvent( QKeyEvent *keyEvent );
		void keyReleaseEvent( QKeyEvent *keyEvent );

		void paintEvent( QPaintEvent *pEvent );

	public slots:
		void show();

	private slots:
		void apply();
		void cancel();
		void clear();
		void reset();

	private:
		QKeySequence newKey, defaultKey;
		quint32 modifierKeys;

	Q_SIGNALS:
		void keySequenceChanged( QKeySequence );
};

class PBKeySequenceButton : public QPushButton {
	Q_OBJECT

	public:
		PBKeySequenceButton( QKeySequence key = 0, QKeySequence defaultSeq = 0, QString name = QString(), QWidget *parent = 0 );

	private:
		QKeySequence defaultSequence;
		QString shortcutName;

	private slots:
		void changeKeySequence();
		void setKeySequence( QKeySequence );
};
