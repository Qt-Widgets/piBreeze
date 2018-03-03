/*
	*
	* PBLogger.hpp - PBLogger.cpp header
	*
*/

#pragma once

#include "Global.hpp"

namespace DbgMsgPart {
	enum MsgPart {
		HEAD,
		BODY,
		TAIL,
		ONESHOT
	};
};

#include <QtWidgets>

void PBMessageOutput5( QtMsgType, const QMessageLogContext&, const QString& );
