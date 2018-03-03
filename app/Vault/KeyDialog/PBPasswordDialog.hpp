/*
	*
	* PBPasswordDialog.hpp - New EncFS Volume Class header
	*
*/

#pragma once

#ifdef STANDALONE
	#include "Global2.hpp"
#else
	#include "Global.hpp"
#endif
#include "PBPasswordEdit.hpp"
#include "PBPasswordInput.hpp"
#include "PBTools.hpp"

class PBPasswordDialog : public QDialog {
	Q_OBJECT

	public:
		PBPasswordDialog( QWidget *parent = NULL );

		/* Password */
		QByteArray password();

		/* Clear the passwords */
		void clear();

	private:
		void createGUI();
		void setWindowProperties();

		PBPasswordEdit *passwd1LE;
		PBPasswordEdit *passwd2LE;

		QPushButton *okBtn;

	private Q_SLOTS:
		/* Check if the new passwords match */
		void validatePassword();
};
