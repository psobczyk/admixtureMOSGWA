/********************************************************************************
 *	This file is part of the MOSGWA program code.				*
 *	Copyright ©2011–2015, Erich Dolejsi, Bernhard Bodenstorfer.		*
 *										*
 *	This program is free software; you can redistribute it and/or modify	*
 *	it under the terms of the GNU General Public License as published by	*
 *	the Free Software Foundation; either version 3 of the License, or	*
 *	(at your option) any later version.					*
 *										*
 *	This program is distributed in the hope that it will be useful,		*
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of		*
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.			*
 *	See the GNU General Public License for more details.			*
 ********************************************************************************/

#include "buildinfo.hpp"
#include "Parameter.hpp"
#include "Exception.hpp"
#include "logging/FileLogger.hpp"
#include "logging/MultiLogger.hpp"
#include "search/egreedy/ElaboratedGreedy.hpp"
#include "search/memetica/MA.hpp"
#include <string>
#include "MData.hpp"

using namespace std;
using namespace logging;

/** Application entry point */
int main ( const int argn, const char *argv[] ) {

	// initialise primary logging
	StreamLogger stderrLogger( cerr );
	logger = &stderrLogger;

	if ( 1 >= argn ) {
		logger->error( "You need to provide a configuration file to use MOSGWA!" );
		return 1;
	}

	// set parameters
	Parameter mainParameter;
	parameter = &mainParameter;
	parameter->setParameters( argn, argv );

	// add file logging
	const string logFileName( parameter->out_file_name + ".log" );
	FileLogger fileLogger( logFileName.c_str() );
	MultiLogger multiLogger;
	multiLogger.add( stderrLogger );
	multiLogger.add( fileLogger );
	logger = &multiLogger;

	switch( parameter->log_level ) {
		case 0: logger->setThreshold( Logger::DEBUG );
			break;
		case 1: logger->setThreshold( Logger::INFO );
			break;
		case 2: logger->setThreshold( Logger::WARNING );
			break;
		case 3: logger->setThreshold( Logger::ERROR );
			break;
	}

	// log start
	logger->info( " _____ _____ _____ _____ _ _ _ _____ " );
	logger->info( "|     |     |   __|   __| | | |  _  |" );
	logger->info( "| | | |  |  |__   |  |  | | | |     |" );
	logger->info( "|_|_|_|_____|_____|_____|_____|__|__|" );
	logger->info( "Model Selection for Genom-Wide Associations" );
	logger->info(
		"Release %s, %s, called with command & arguments:",
		buildinfo::version,
		buildinfo::timestamp
	);
	for ( int i = 0; i < argn; ++i ) {
		logger->info( "\t%s", argv[i] );
	}

	try {
		if ( Parameter::searchStrategy_memetic_algorithm == parameter->searchStrategy ) {
			memetica::MA artur;
			artur.run();

/** <AG> It is only for testing */
   MData data;
   Model model( *(artur.result()) );
   string out_file_name = parameter->out_file_name;
   parameter->out_file_name = parameter->out_file_name + "_MA";
   model.printModel("Memetic Algoritm");
   parameter->out_file_name = out_file_name;
/** The end of testing */

		} else {
			egreedy::ElaboratedGreedy erich;
			erich.run();
		}
	} catch ( const Exception e ) {
		logger->error( "%s", e.what() );
		return 2;
	}
	logger->info( "Finished MOSGWA." );
	return 0;
}
