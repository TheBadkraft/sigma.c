/*	Created on: Aug 11, 2024
 *  Author: Badkraft
 *
 *  sigmac.c
 */

#include <stdio.h>
#include <stdlib.h>

#include "sigmac.h"

// #if DEBUG
#include "test/sigc_tests.h"
// #endif

//	version
const byte maj = 0;
const byte min = 0;
const ushort build = 2;
const RC rc = ALPHA;
string label = "tuxedo";

//	TODO: err queue
static string ERR_PARAM;
static string *const ERR_MSG_FORMATS = (string[]){
	"No Error",
	"Bad User Option: %s",
};

//	terminate collections with NULL for ease of iteration
string *const KEY_OPTIONS = (string[]){
	"i", "o", "s", NULL};
string *const TAG_OPTIONS = (string[]){
	"help", "info", "version", "default", NULL};

#define EMPTY_OPT \
	(struct sc_opt) { OPT_NOP, OPT_NO_OP, NULL, NULL }

const struct sc_opt NULL_OPT = EMPTY_OPT;

//	TODO: place into a file for configuration ...???
struct sc_opt DEFAULT_OPTIONS[] = {
	{OPT_KEY, OPTKEY_SOURCE, NULL, "Source file."},
	EMPTY_OPT,
};
struct sc_opt USER_OPTIONS[] = {
	//	TODO: add property to enable/disable
	{OPT_TAG, OPTTAG_HELP, NULL, "Display command-line options and usage."},
	{OPT_TAG, OPTTAG_INFO, NULL, "Full compiler and component version information."},
	{OPT_TAG, OPTTAG_VERSION, NULL, "Simple compiler version."},
	{OPT_TAG, OPTTAG_DEFAULT, NULL, "Display default settings."},
	{OPT_KEY, OPTKEY_OUTPUT, NULL, "To output file"},
	{OPT_KEY, OPTKEY_1D10T, NULL, "ID:10T compatibility switch."},
	EMPTY_OPT,
};

/*-- sigC prototypes --*/
/// @brief load argument options
/// @param argv
/// @return TRUE if options loaded; otherwise FALSE
static bool sigc_load(string *);
/// @brief configure the compiler
/// @param void
/// @return TRUE if configuration successful; otherwise FALSE
static bool sigc_configure(void);
/// @brief dispose compiler
/// @param  void
static void sigc_dispose(void);

/// @brief the Sigma.C compiler instance
static struct sigc SIGC = {
	.is_initialized = false,
	.name = "Sigma.C",
	.argc = 0,
	.load = &sigc_load,
	.configure = &sigc_configure,
	.dispose = &sigc_dispose,
};

static sigC_config sigC_settings;

//	PROTOTYPES	---------------------
/*
 * 	Obtain a reference to the Sigma.C compiler. If the compiler has not
 * 	been initialized, this will run initialization.
 *
 * 	sigC*:	reference to the compiler
 * 	bool :	returns TRUE if valid reference;
 * 			otherwise FALSE
 */
bool sc_get_instance(sigC *);
/*
 *	Initialize the compiler instance
 */
static bool sc_init(void);
/*
 *	Load pre-defined user options
 *
 *	Parses available options and assimilates into configurable parameters.
 */
static void sc_load_opts();
static void sc_get_error(string *);
static bool sc_load_codex(void);

static int process_args(string *);
static int find_option_index(string *, string);
static size_t count(struct sc_opt[]);

static sigC_option get_option_by_tag(string);
static sigC_option get_option_by_key(string);
static sigC_option set_source_option(string, IOType);

static bool set_source_param();

static bool display_help(sigC_param);
static bool display_info(sigC_param);
static bool display_version(sigC_param);
static bool display_default(sigC_param);
static bool id10t_compatibility_mode(sigC_param);
static bool set_output_file(sigC_param);

//	DEFINITIONS
// #if DEBUG
void __reset_sigmac()
{
	//	TODO: SC.free() / dispose() method
	String.free(SIGC.name);
	Directory.free(SIGC.cwd);
	String.free(SIGC.path);
	//	TODO: ...
	free(SIGC.ver);
	//	---------
	if (SIGC.codex != NULL)
	{
		Codex.dispose();
	}
	if (SIGC.options != NULL)
	{
		Allocator.dealloc(SIGC.options);
	}
	SIGC.error = NO_ERR;
	SIGC.is_initialized = false;
}
// #endif

//	compiler delegates
static bool sigc_load(string *argv)
{
	//	skip arg[0] CL: sigma.c
	int arg_count = process_args(++argv);
	bool retOk = SIGC.error == NO_ERR;

	int ndx = 0;
	sigC_option opt;
	sigC_config cfg;

	//	allocate configurations; +1 to terminate with \0
	sigC_settings = Allocator.alloc(sizeof(struct sc_cfg) * (arg_count + 1), INITIALIZED);

	while (retOk && ndx < arg_count)
	{
		opt = SIGC.options + ndx;

		cfg = Allocator.alloc(sizeof(struct sc_cfg), UNINITIALIZED);
		if (cfg)
		{
			cfg->configure = opt->configure;
			cfg->param = opt->param;
		}
		sigC_settings[ndx] = *cfg;

		++ndx;
	}

	SIGC.argc = arg_count;

	/*
	 *	retOk will return TRUE if no CL args are given; FALSE if a bad arg is passed
	 */
	return retOk;
}
/*
 *	configures the compiler from the loaded user options
 */
static bool sigc_configure(void)
{
	bool retOk = true;
	sigC_config ptr = sigC_settings;
	struct sc_cfg cfg = *ptr;

	while ((ptr - sigC_settings) < SIGC.argc && retOk)
	{
		retOk = cfg.configure(cfg.param);
		cfg = *++ptr;
	}

	return retOk;
}
static void sigc_dispose(void)
{
	//	temporary
	__reset_sigmac();
}

static bool sc_init()
{
	if (!SIGC.is_initialized)
	{
		SIGC.ver = Version.new(maj, min, build, rc, label);
		Directory.current(&(SIGC.cwd));
		String.alloc(Path.directory(__FILE__), &(SIGC.path));

		{ //	load built in options
			size_t optCount = count(DEFAULT_OPTIONS);
			sigC_option ndx = DEFAULT_OPTIONS;
			while (ndx - DEFAULT_OPTIONS < optCount)
			{
				switch (ndx->opt_type)
				{
				case OPT_KEY:
					switch (ndx->option.key)
					{
					case OPTKEY_SOURCE:
						ndx->configure = &set_source_param;

						break;
					default:
						break;
					}
					break;

				case OPT_TAG:

					break;
				}

				++ndx;
			}
		}

		sc_load_opts();

		SIGC.is_initialized = true;
	}

	return SIGC.is_initialized;
}

static int process_args(string *argv)
{
	string *ndx = argv;
	string arg = *ndx;
	int arg_count = 0;
	IOType io_type;
	sigC_option opt = NULL;

	//	+1 for terminator
	int optCount = count(DEFAULT_OPTIONS) + count(USER_OPTIONS) + 1;
	SIGC.options = Allocator.alloc(sizeof(struct sc_opt) * optCount, UNINITIALIZED);

	/*
	 * 	Apparently, argv terminates with a NULL ...???
	 */
	while (arg)
	{
		//	need to know whether the arg starts with '--' or '-'
		if (arg[1] == '-' && arg[0] == '-')
		{
			arg += 2;
			//	find option by tag
			opt = get_option_by_tag(arg);
			//	TODO: some options will terminate any other options
		}
		else if (arg[0] == '-')
		{
			//	find option by key
			opt = get_option_by_key(arg);
			//	TODO: some options will terminate any other options
		}
		//	does the arg look like a path?
		else if ((io_type = Path.type(arg) != IO_UNKNOWN))
		{
			opt = set_source_option(arg, io_type);
		}
		else
		{
			SIGC.error = BAD_ARG;
			printf("invalid option: %s\n", arg);
			//	set arg_count to 0 because we don't want to proceed
			ndx = argv;

			break;
		}

		//	printf("[%ld] %s\n", ndx - argv, arg);

		if (opt)
		{
			//	printf("option valid: %s\n", opt->tag);
			SIGC.options[ndx - argv] = *opt;
		}
		else
		{
			SIGC.error = BAD_ARG;
			ERR_PARAM = arg;

			free(SIGC.options);
			SIGC.options = NULL;
			ndx = argv;

			break;
		}

		arg = *++ndx;
	}
	arg_count = ndx - argv;

	if (arg_count > 0)
	{
		SIGC.options[arg_count] = NULL_OPT;
	}

	return arg_count;
}
static int find_option_index(string *const source, string target)
{
	string *ndx = source;
	string item = *ndx;
	int pos = -1;

	while (item)
	{
		if (strcmp(target, item) == 0)
		{
			pos = ndx - source;
			break;
		}

		item = *++ndx;
	}

	return pos;
}
static size_t count(struct sc_opt source[])
{
	sigC_option ndx = source;
	struct sc_opt item = *ndx;

	while (item.opt_type != NULL_OPT.opt_type)
	{
		item = *++ndx;
	}

	return ndx - source;
}
static sigC_option get_option_by_tag(string tag)
{
	int tagNdx = find_option_index(TAG_OPTIONS, tag);
	enum tag_opt tagOption = tagNdx;
	sigC_option ndx = USER_OPTIONS;
	struct sc_opt opt = *ndx;
	size_t optCount = count(USER_OPTIONS);
	while (ndx - USER_OPTIONS < optCount)
	{
		if (opt.opt_type == OPT_TAG && opt.option.tag == tagOption)
		{
			return ndx;
		}
		opt = *++ndx;
	}

	return NULL;
}
static sigC_option get_option_by_key(string key)
{
	int keyNdx = find_option_index(KEY_OPTIONS, key);
	enum key_opt keyOption = keyNdx;
	sigC_option ndx = USER_OPTIONS;
	struct sc_opt opt = *ndx;
	size_t optCount = count(USER_OPTIONS);
	while (ndx - USER_OPTIONS < optCount)
	{
		if (opt.opt_type == OPT_KEY && opt.option.key == keyOption)
		{
			return ndx;
		}
		opt = *++ndx;
	}

	return NULL;
}
static sigC_option set_source_option(string pPath, IOType io_type)
{
	sigC_option opt = &DEFAULT_OPTIONS[SOURCE];
	opt->configure = &set_source_param;
	opt->param = Allocator.alloc(sizeof(sigC_param), UNINITIALIZED);
	opt->param->params = pPath;
	opt->param->type = SOURCE;

	return opt;
}
static bool set_source_param(sigC_param param)
{
	//	TODO: read source input parameter
	printf("[TODO] Source: %s\n", param->params);

	return true;
}

static void sc_load_opts()
{
	//	printf("parse options:\n");

	sigC_option ndx = USER_OPTIONS;
	size_t optCount = count(USER_OPTIONS);
	while (ndx - USER_OPTIONS < optCount)
	{
		//		printf("option: %s\n", opt.tag);
		switch (ndx->opt_type)
		{
		case OPT_KEY:
			switch (ndx->option.key)
			{
			case OPTKEY_1D10T:
				ndx->configure = &id10t_compatibility_mode;

				break;
			case OPTKEY_OUTPUT:
				ndx->configure = &set_output_file;

				break;
			}

			break;
		case OPT_TAG:
			switch (ndx->option.tag)
			{
			case OPTTAG_HELP:
				ndx->configure = &display_help;

				break;
			case OPTTAG_INFO:
				ndx->configure = &display_info;

				break;
			case OPTTAG_VERSION:
				ndx->configure = &display_version;

				break;
			case OPTTAG_DEFAULT:
				ndx->configure = &display_default;

				break;
			}

			break;
		}

		++ndx;
	}
}
static void sc_get_error(string *errMsg)
{
	int errCode = SIGC.error;

	String.alloc(ERR_MSG_FORMATS[errCode], errMsg);
	String.format(*errMsg, errMsg, ERR_PARAM);
}
static bool sc_load_codex()
{
	// bool retOk = Codex.init(SIGC.cwd, &(SIGC.codex));
	// if (!retOk)
	// {
	// 	printf("error loading Sigma.C Codex\n");
	// }

	return false;
}

static bool id10t_compatibility_mode(sigC_param param)
{
	printf("ID10t compatibility mode [TRUE]\n");

	return false;
}
static bool display_help(sigC_param param)
{
	//	TODO: options table
	printf("[TODO] Usage & Options ====\n");

	return true;
}
static bool display_info(sigC_param param)
{
	//	TODO: verbose info
	printf("===== %s =====\n", SIGC.name);
	printf("cwd:  %s\n", SIGC.cwd->path);
	printf("path: %s\n", SIGC.path);

	return false;
}
static bool display_version(sigC_param param)
{
	//	TODO: version
	printf("[TODO] Version: \n");

	return true;
}
static bool display_default(sigC_param param)
{
	//	TODO: default
	printf("[TODO] Default configuration: \n");

	return true;
}
static bool set_output_file(sigC_param param)
{
	//	TODO: output file parameters
	printf("[TODO] Output: %s\n", param ? param->params : "[using default]");

	return true;
}
//==============================================

bool sc_get_instance(sigC *sc)
{
	bool retOk = SIGC.is_initialized;
	if (!retOk)
	{
		retOk = sc_init();
	}

	if (retOk)
	{
		(*sc) = &SIGC;
	}
	else
	{
		(*sc) = NULL;
	}

	return retOk;
}

const struct SigmaC SC = {
	.instance = &sc_get_instance,
	.get_error = &sc_get_error,
};
