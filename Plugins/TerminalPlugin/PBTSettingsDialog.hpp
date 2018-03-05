/*
    *
    * * PBTSettingsDialog.cpp - QTermWidget reimplementation for PBTerminal
    *
*/

#ifndef PBTSETTINGSDIALOG_HPP
#define PBTSETTINGSDIALOG_HPP

#include "Global.hpp"

class PBTSettingsDialog : public QDialog {
	Q_OBJECT

	public:
		PBTSettingsDialog();

		QComboBox *colorSchemesCombo;
		QDoubleSpinBox *transparencySpin;
		QFontComboBox *fontCombo;
		QCheckBox *enableTransparencyCheck;

	private:
		void createGUI();

	private slots:
		void setColorScheme();
		void setFont( QFont );
		void setEnableTransparency();
		void setTransparency();
};

#endif
