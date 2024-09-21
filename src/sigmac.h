/*	Created on: Aug 11, 2024
 *  Author: Badkraft
 *
 *  sigmac.h
 */

#ifndef _SIGMAC_H_
#define _SIGMAC_H_

#include "open/io.h"
#include "open/version.h"

#include "codex.h"

//	built-in option type
enum bltin_opt_type
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
	enum bltin_opt_type type;
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
	/// @brief compiler name
	string name;
	/// @brief compiler version
	version ver;
	/// @brief number of configuration arguments
	int argc;
	/// @brief current working dir is what the CL path is
	directory cwd;
	/// @brief the path from which Sigma.C executes
	string path;
	/// @brief determines if the compiler has been initialized
	bool is_initialized;
	/// @brief codex: language definition
	struct sc_codex *codex;
	/// @brief collection of sc_opt objects
	struct sc_opt *options;
	/// @brief get the compiler's error status
	enum sc_error
	{
		NO_ERR = 0,
		BAD_ARG = 1,
	} error;

	/// @brief load argument options
	/// @param string*
	/// @return TRUE if options loaded; otherwise FALSE
	bool (*const init)(string *);
	/// @brief configure the compiler
	/// @param void
	/// @return TRUE if configuration successful; otherwise FALSE
	bool (*const configure)(void);
	/// @brief load Sigma.C codex
	/// @param void
	/// @return TRUE if loaded; otherwise FALSE
	bool (*const load)(void);
	/// @brief dispose the compiler object
	/// @param void
	/// @return void
	void (*const dispose)(void);

	/*
	 * TODO:
	 *	add exit mode - SILENT, WHISPER, SHOUT, SCREAM
	 */
};

struct sc_cfg
{
	config configure;
	struct sc_opt_param *param;
};

typedef struct sigc *sigC;
typedef struct sc_opt *sigC_option;
typedef struct sc_opt_param *sigC_param;
typedef struct sc_cfg *sigC_config;

extern const struct SigmaC
{
	/// @brief obtain a reference to the Sigma.C compiler instance
	/// @param sigC* (out)
	/// @return TRUE if valid instance; otherwise FALSE
	bool (*instance)(sigC *);
	/// @brief get error message
	/// @param string* (out)
	void (*get_error)(string *);
	/// @brief exit the Sigma.C compiler
	/// @param void
	/// @return 0 if not errors; otherwise 1
	int (*exit)(void);
} SC;
#endif /* _SIGMAC_H_ */
