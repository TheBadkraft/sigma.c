/*
 *	 ============================================================================
 *	 Name        : Sigma.C Compiler (main)
 *	 Author      : Badkraft
 *	 Version     :
 *	 Copyright   : 2024, A Badkraft Production
 *	 Description : Sigma.C Compiler entry
 *	 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "../sigmac.h"

#define NO_ERR 0
#define STATUS(okay) okay ? EXIT_SUCCESS : EXIT_FAILURE

int main(int argc, string *argv)
{
	bool retOk = true;
	int status = STATUS(retOk);
	sigC sigc;

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
	retOk = sigc->init(argv);
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
	if (!sigc->argc)
	{
		printf("WARNING: args (%d) :: ", sigc->argc);
	}

	/*
	 * 	load the Codex
	 */
	retOk = sigc->load();
	//	validate sigc.codex has lexer, document, etc.
	if (!retOk)
	{
		goto quit_sigmac;
	}
	retOk = Parser.load_sources();
	if (!retOk)
	{
		goto quit_sigmac;
	}

	// printf("%s\n", sigc->codex->definition->content->buffer);

/*
 * 	load the Parser
 SC.load_parser("SOURCE");
 //	validate sigc.parser has lexer, document, etc.

 */

// this is the first time in 20+ years I've used a label to go to (other than ASM)
quit_sigmac:
	status = STATUS(sigc->error == NO_ERR);
	printf("exiting Sigma.C Compiler (%d) ...\n", status);

	return status;
}
