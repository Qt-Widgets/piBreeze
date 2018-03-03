/*
	*
	* PBMessageDialog.hpp - PBMessageDialog.cpp header
	*
*/

#pragma once

#include "Global.hpp"

class PBMessageDialog : public QMessageBox {
	Q_OBJECT

	public:
		static int information( QWidget*, QString, QString );
		static int question( QWidget*, QString, QString );
		static int warning( QWidget*, QString, QString );
		static int error( QWidget*, QString, QString );
		static int critical( QWidget*, QString, QString );
};
