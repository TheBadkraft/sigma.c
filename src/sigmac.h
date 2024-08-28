/*	Created on: Aug 11, 2024
 *  Author: Badkraft
 *
 *  sigmac.h
 */

#ifndef _SIGMAC_H_
#define _SIGMAC_H_

#include "open/io.h"

#include "../core/version.h"

#include "codex.h"

//	built-in option type
enum bi_opt_type
{
	SOURCE = 0,
	OUTPUT = 1,
	LOGDEST = 2,
};
//	key option type - e.g., '-i', '-o', etc
enum key_opt
{
	OPTKEY_1D10T = 0,
	OPTKEY_OUTPUT = 1,
	OPTKEY_SOURCE = 2,
};
//	tag option type - e.g., '--help', '--info', etc
enum tag_opt
{
	OPTTAG_HELP = 0,
	OPTTAG_INFO = 1,
	OPTTAG_VERSION = 2,
	OPTTAG_DEFAULT = 3,
};
//	no-op option - annotates a *null* option
enum nop_opt
{
	OPT_NO_OP
};
/*
 * sc_opt_param: parameter container for command line args
 * examples:
 * 1.	file parameter for source option				/dir/file.C
 * 2.	log dir parameter for log (-l) option			-l:../logs/
 */
struct sc_opt_param
{
	enum bi_opt_type type;
	char *params;

	//	sc_opt_type will tell us which member of each union to access
	//	TODO: io_type_properties (???)
};

//	configuration delegate
typedef bool (*config)(struct sc_opt_param *);

/*
 * sc_opt: command line argument structure
 */
struct sc_opt
{
	//	what kind of option - no-op, key, tag
	enum
	{
		OPT_NOP = -1,
		OPT_TAG = 0,
		OPT_KEY = 1
	} opt_type;
	//	the option
	union
	{
		enum nop_opt nop;
		enum key_opt key;
		enum tag_opt tag;
	} option;
	//	delegate to configure the option
	config configure;
	//	description (for help menu)
	char *descr;
	//	parameter if required by the option
	struct sc_opt_param *param;
};

struct sigc
{
	//	compiler name
	const char *name;
	//	compiler version
	version *ver;
	//	number of configuration arguments
	int cfgc;
	//	delegate handler to load options
	bool (*load)(char **);
	//	collection of sc_opt objects
	struct sc_opt *options;
	//	delegate handler to configure the compiler
	bool (*configure)(void);
	//	current working dir is what the CL path is
	directory *cwd;
	//	the path from which sigmac executes
	string *path;
	//	codex: language definition
	struct sc_codex *codex;
	/*
	 * TODO:
	 *	add exit mode - SILENT, WHISPER, SHOUT, SCREAM
	 */
};

struct sc_conf
{
	config configure;
	struct sc_opt_param *param;
};

typedef struct sigc sigC;
typedef struct sc_opt sigC_option;
typedef struct sc_opt_param sigC_param;
typedef struct sc_conf sigC_config;

extern const struct SigmaC
{
	//	relays a reference to the Sigma.C compiler
	//	TRUE if valid instance; otherwise FALSE
	bool (*instance)(sigC **);
	bool (*load_codex)();
} SC;
#endif /* _SIGMAC_H_ */
