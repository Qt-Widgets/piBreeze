/*
    *
    * PBCLParser.hpp - PBArgParser.cpp header
    *
*/

#pragma once
#ifndef PBARGPARSER
#define PBARGPARSER

#include <sys/utsname.h>

#include <Global.hpp>
#include <optionparser.hpp>

enum  optionIndex {
	UNKNOWN,
	HELP,
	USAGE,
	MANPAGE,
	VERSION,
	SYSTRAY,
	SETTINGS,
	ABOUTPB,
	LICENSE,
	FORCENEW,
	NORMAL
};

int PBArgParser( int argc, char** argv );

#endif
