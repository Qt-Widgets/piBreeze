/*
    *
    * PBCLParser.cpp - Command Line Parser for PiBreeze. This is based on OptionParser by Matthias S. Benkmann
    *
*/

#include <PBCLParser.hpp>

/* Our own CheckArg function to detect unknown arguments passed to PiBreeze */
struct Arg: public option::Arg {

	static void printError( const char* msg1, const option::Option& opt, const char* msg2 ) {

		fprintf( stderr, "%s", msg1 );
		fwrite( opt.name, opt.namelen, 1, stderr );
		fprintf( stderr, "%s", msg2 );
	}

	static option::ArgStatus Unknown( const option::Option& opt, bool msg ) {

		if (msg)
			printError( "Unknown option '", opt, "'\n");

		return option::ARG_IGNORE;
	}
};

const option::Descriptor usage[] = {
    /* Help prefix */
    { UNKNOWN,    0,   "",     "",            Arg::Unknown,     "PiBreeze version 3.0.0 Alpha\n"
		                                                        "Copyright (C) 2016 Marcus Britanicus\n\n"
		                                                        "This is free software and is released under the GPLv3 license.\n"
		                                                        "There is ABSOLUTELY NO WARRANTY; not even for MERCHANTABILITY or "
		                                                        "FITNESS FOR A PARTICULAR PURPOSE.\n\n"

                                                                "USAGE:\n"
                                                                "    pibreeze [options]\n"
                                                                "    pibreeze [-f|--force-new] [URL]\n\n"
                                                                "Options:" },

    /* Print the help option */
    { HELP,       0,   "h",     "help",       Arg::None,        "    -h, --help       Print this help message and exit." },

    /* Print the help option */
    { USAGE,      0,   "u",     "usage",      Arg::None,        "    -u, --usage      Print the usage and exit." },

    /* Print the help option */
    { MANPAGE,    0,   "m",     "manpage",    Arg::None,        "    -m, --manpage    Open the man page if it is installed" },

    /* Print the version option */
    { VERSION,    0,   "v",    "version",     Arg::None,        "    -v  --version    Print version information and exit." },

	/* Print the settings option */
    { SYSTRAY,    0,   "t",    "tray",        Arg::None,        "    -t, --tray       Open an instance in system tray." },

	/* Print the settings option */
    { SETTINGS,   0,   "s",    "settings",    Arg::None,        "    -s, --settings   Open the settings dialog." },

	/* Print the about option */
    { ABOUTPB,      0,   "a",    "about",     Arg::None,        "    -a, --about      Show the about PiBreeze dialog." },

	/* Print the about option */
    { LICENSE,      0,   "l",    "license",   Arg::None,        "    -l, --license    Show the PiBreeze License dialog." },

	/* Print the force-new option */
    { FORCENEW,   0,   "f",    "force-new",   Arg::None,        "    -f, --force-new  Kill the old server and start a new one." },

    /* Print the meaning of [URL] */
    { UNKNOWN,    0,   "",     "",            Arg::None,        "\nArguments:\n"
                                                                "    URL     \t Url of the path you want to open." },

    /* List the examples */
    { UNKNOWN,    0,   "", "",                Arg::None,        "\nExamples:\n"
                                                                "    pibreeze -h                           # Prints this help message\n"
                                                                "    pibreeze                              # Opens the last opened directory\n"
                                                                "    pibreeze -t                           # Opens an instance in tray\n"
                                                                "    pibreeze -s                           # Opens the settings dialog\n"
                                                                "    pibreeze --settings                   # Opens the settings dialog\n"
                                                                "    pibreeze -a                           # Opens the about PB dialog\n"
                                                                "    pibreeze --license                    # Opens the PB license dialog\n"
                                                                "    pibreeze --force-new                  # Kills the existsing server and "
                                                                                                            "opens window at the last opened location\n"
                                                                "    pibreeze --force-new /home/cosmos     # Kills the existsing server and "
                                                                                                             "opens a window at /home/cosmos\n"
    },

    /* End of options */
    { 0, 0, 0, 0, 0, 0 }
};

int PBArgParser( int argc, char** argv ) {

	// Decrease the number of arguments by 1
	argc -= ( argc > 0 );

	// Increase the pointer of the arguments by 1
	argv += ( argc > 0 );

	/* Init the Option parser */
	option::Stats stats( usage, argc, argv );
	option::Option options[ stats.options_max ], buffer[ stats.buffer_max ];

	/* Parse the command line arguments */
	option::Parser parse(usage, argc, argv, options, buffer);

	if ( parse.error() )
		return 1;

	if ( options[ HELP ] ) {

		option::printUsage( std::cout, usage, 144 );
		return 0;
	}

	if ( options[ USAGE ] ) {

		fprintf( stdout,
			"PiBreeze version 3.0.0\n"
			"Copyright (C) 2013 Marcus Britanicus\n\n"

			"USAGE:\n"
			"    pibreeze [options]\n"
			"    pibreeze [-f|--force-new] [URL]\n\n"
		);

		return 0;
	}

	if ( options[ MANPAGE ] ) {

		return system( "man pibreeze" );
	}

	if ( options[ VERSION ] ) {

		fprintf( stdout,
			"PiBreeze version 3.0.0\n"
			"Copyright (C) 2013 Marcus Britanicus\n\n"

			"This is free software and is released under the GPLv3 license.\n"
			"There is ABSOLUTELY NO WARRANTY; not even for MERCHANTABILITY or "
			"FITNESS FOR A PARTICULAR PURPOSE.\n\n"

			"This program was compiled using GCC " __VERSION__ " for "
			#if defined( __i386__ )
				"x86 machines\n"
			#elif defined( __amd64__ )
				"x86_64 machines\n"
			#endif
		);

		struct utsname uts;
		if ( uname( &uts ) == 0 )
			fprintf( stdout, "You are currently running %s %s\n\n", uts.sysname, uts.release );

		return 0;
	}

	/* If there some other argument was given to @a --tray, ignore it, but print it to let the user know */
	if ( options[ SYSTRAY ] ) {
		if( argc > 1 )
			fprintf( stdout, "Ignoring the %d extra arguments specified...\n", parse.optionsCount() + parse.nonOptionsCount() - 1 );

		return SYSTRAY;
	}

	/* If there some other argument was given to @a --settings, ignore it, but print it to let the user know */
	if ( options[ SETTINGS ] ) {
		if( argc > 1 )
			fprintf( stdout, "Ignoring the %d extra arguments specified...\n", parse.optionsCount() + parse.nonOptionsCount() - 1 );

		return SETTINGS;
	}

	if ( options[ ABOUTPB ] ) {
		if( argc > 1 )
			fprintf( stdout, "Ignoring the %d extra arguments specified...\n", parse.optionsCount() + parse.nonOptionsCount() - 1 );

		return ABOUTPB;
	}

	if ( options[ LICENSE ] ) {
		if( argc > 1 )
			fprintf( stdout, "Ignoring the %d extra arguments specified...\n", parse.optionsCount() + parse.nonOptionsCount() - 1 );

		return LICENSE;
	}

	/* If there some other argument was given to @a --settings, ignore it, but print it to let the user know */
	else if ( options[ FORCENEW ] ) {
		/* We were unable to open a new instance in the existing session. */
		/* Forcing to start a new session */
		return FORCENEW;
	}

	else {
		/* All is well. */
		return NORMAL;
	}
}
