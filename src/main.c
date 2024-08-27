/*
 *	 ============================================================================
 *	 Name        : Sigma.C Compiler (main)
 *	 Author      : The Badkraft
 *	 Version     :
 *	 Copyright   : 2024 Badkraft Productions
 *	 Description : Sigma.C Compiler entry
 *	 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "sigmac.h"

#define STATUS(okay) okay ? EXIT_SUCCESS : EXIT_FAILURE

int main(int argc, char **argv)
{
	bool retOk = true;
	int status = STATUS(retOk);
	sigC *sigc;

	if (!retOk)
	{
		goto quit_sigmac;
	}

	/*
	 *	initialize Sigma.C compiler
	 */
	retOk = SC.instance(&sigc);
	if (!retOk)
	{
		goto quit_sigmac;
	}
	//	printf("completed %s initialization ...\n", sigc->name);

	/*
	 * 	have compiler load the command line arguments
	 */
	retOk = sigc->load(argv);
	if (!retOk)
	{
		goto quit_sigmac;
	}
	//	printf("processed (%d) arg%s\n", arg_count, arg_count > 1 ? "s" : "");

	/*
	 * 	configure the compiler
	 *
	 * 	NOTE: Even when a source is supplied, we are getting 0 `cfgc`. It is not causing the compiler to bail, but
	 * 	we do get the [WARNING] message below with 0 args.
	 */
	retOk = sigc->configure();
	if (!sigc->cfgc)
	{
		printf("WARNING: args (%d) :: ", sigc->cfgc);
	}

/*
 * 	load the Codex

 SC.load_codex("./.data/sigmac.def");
 //	validate sigc.codex has lexer, document, etc.

 */

/*
 * 	load the Parser
 SC.load_parser("SOURCE");
 //	validate sigc.parser has lexer, document, etc.

 */

// this is the first time in 20+ years I've used a label to go to (other than ASM)
quit_sigmac:
	status = STATUS(retOk);
	printf("exiting Sigma.C Compiler (%d) ...\n", status);

	return status;
}
