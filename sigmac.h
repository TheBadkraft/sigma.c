/*	Created on: Aug 11, 2024
 *  Author: Badkraft
 *
 *  sigmac.h
 */

#ifndef _SIGMAC_H_
#define _SIGMAC_H_

#include "version.h"
#include "codex.h"

enum sc_opt_type {
	SC_OPT_SOURCE = 0,
	SC_OPT_OUTPUT = 1,
	SC_OPT_LOGOUT = 2,
};

/*
 * sc_opt_param: parameter container for command line args
 * examples:
 * 1.	file parameter for source option				/dir/file.C
 * 2.	log dir parameter for log (-l) option			-l:../logs/
 */
struct sc_opt_param {
	enum sc_opt_type type;
	char *params;

	//	sc_opt_type will tell us which member of each union to access
	//	TODO: io_type_properties (???)
};
typedef bool(*config)(struct sc_opt_param*);


/*
 * sc_opt: command line argument structure
 */
struct sc_opt {
	//	single character switch option; ex: -<h>
	char key;
	//	word option; ex: --<help>
	char *tag;
	//	delegate to configure the option
	config configure;
	//	description (for help menu)
	char *descr;
	//	parameter if required by the option
	struct sc_opt_param *param;
};

struct sigc {
	//	compiler name
	const char *name;
	//	compiler version
	version *ver;
	//	number of configuration arguments
	int cfgc;
	//	delegate handler to load options
	bool (*load)(char**);
	//	collection of sc_opt objects
	struct sc_opt *options;
	//	delegate handler to configure the compiler
	bool (*configure)(void);
	//	current working dir is what the CL path is
	char *cwd;
	//	the path from which sigmac executes
	char *path;
	/*
	 * TODO:
	 *	add exit mode - SILENT, WHISPER, SHOUT, SCREAM
	 */
};

struct sc_conf {
	config configure;
	struct sc_opt_param *param;
};

typedef struct sigc sigC;
typedef struct sc_opt sigC_option;
typedef struct sc_opt_param sigC_param;
typedef struct sc_conf sigC_config;

extern const struct sigmac {
	//	relays a reference to the Sigma.C compiler
	//	TRUE if valid instance; otherwise FALSE
	bool (*instance)(sigC*);
} SC;
#endif /* _SIGMAC_H_ */
