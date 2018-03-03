/*
	*
	* PBPasswordInput.hpp - PBPasswordInput class header
	*
*/

#pragma once

#ifdef STANDALONE
	#include "Global2.hpp"
#else
	#include "Global.hpp"
#endif
#include "PBPatternPad.hpp"

class PBPasswordInput : public QDialog {
	Q_OBJECT

	public:
		PBPasswordInput( QWidget *parent = NULL );

		/* Password */
		QString password();

		/* Clear the password */
		void clear();

	private:
		void createGUI();
		void setWindowProperties();

		PasswordKeypad *pKeyPad;
		QLineEdit *passwdLE;

		QString mPassword;
		bool mOkay;

	private Q_SLOTS:
		void setPassword();
};
