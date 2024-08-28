/*	Created on: Aug 11, 2024
 *  Author: Badkraft
 *
 *  sigmac.c
 */

#include <stdio.h>
#include <stdlib.h>

#include "open/io.h"
#include "sigmac.h"

// char *const OPT_HELP = "help";
// const char *OPT_INFO = "info";
// const char *OPT_VERSION = "version";
// const char *OPT_DEFAULT = "default";

// char const OPT_UNKN = -1;
// char const OPT_NONE = '\0';
// char const OPT_ID10T = 'i';
// char const OPT_OUTPUT = 'o';
// char const OPT_SOURCE = 's';

//	terminate collections with NULL for ease of iteration
char **const KEY_OPTIONS = (char *[]){
	"i", "o", "s", NULL};
char **const TAG_OPTIONS = (char *[]){
	"help", "info", "version", "default", NULL};

#define NULL_OPT \
	(struct sc_opt) { OPT_NOP, OPT_NO_OP, NULL, NULL }

//	TODO: place into a file for configuration ...???
const size_t USR_OPT_COUNT = 6;
const size_t BLTIN_OPT_CNT = 1;
struct sc_opt bltin_optns[] = {
	{OPT_KEY, OPTKEY_SOURCE, NULL, "Source file."},
	NULL_OPT,
};
struct sc_opt usr_options[] = {
	//	TODO: add property to enable/disable
	{OPT_TAG, OPTTAG_HELP, NULL, "Display command-line options and usage."},
	{OPT_TAG, OPTTAG_INFO, NULL, "Full compiler and component version information."},
	{OPT_TAG, OPTTAG_VERSION, NULL, "Simple compiler version."},
	{OPT_TAG, OPTTAG_DEFAULT, NULL, "Display default settings."},
	{OPT_KEY, OPTKEY_OUTPUT, NULL, "To output file"},
	{OPT_KEY, OPTKEY_1D10T, NULL, "ID:10T compatibility switch."},
	NULL_OPT};

static sigC *sigc;
static sigC_config *sigC_settings;

//	version
const byte maj = 0;
const byte min = 0;
const ushort bld = 2;
const relc rc = ALPHA;
char *label = "tuxedo";

//	PROTOTYPES
bool __sc_instance(sigC **);
bool __sc_load_codex();

static bool sc_init();
static bool sc_load_configuration(char **);
static bool sc_configure();

static int process_args(char **);
static void parse_built_in_options();
static void parse_options();
static int find_source_index(char **const, char *);

static sigC_option *get_option_by_tag(char *);
static sigC_option *get_option_by_key(char *);
static sigC_option *set_source_option(char *, IOType);

static bool set_source_param();

static bool display_help(sigC_param *);
static bool display_info(sigC_param *);
static bool display_version(sigC_param *);
static bool display_default(sigC_param *);
static bool id10t_compatibility_mode(sigC_param *);
static bool set_output_file(sigC_param *);

//	DEFINITIONS
static bool sc_init()
{
	bool retOk = (sigc = malloc(sizeof(sigC))) != NULL;

	if (retOk)
	{
		sigc->name = "Sigma.C";
		sigc->ver = Version.new(maj, min, bld, rc, label);
		sigc->load = &sc_load_configuration;
		sigc->configure = &sc_configure;
		Directory.current(&(sigc->cwd));
		sigc->path = String.alloc(Path.directory(__FILE__));
		// sigc->cdx_definition = "sigmac.def";

		printf("===== %s =====\n", sigc->name);
		printf("cwd:  %s\n", sigc->cwd->name->buffer);
		printf("path: %s\n", sigc->path->buffer);

		parse_built_in_options();
		parse_options();
	}
	else
	{
		//	pError("initialization error");
	}

	return retOk;
}
static bool sc_load_configuration(char **argv)
{
	bool retOk = true;
	//	skip arg[0] CL: sigma.c
	int arg_count = process_args(++argv);
	int ndx = 0;
	sigC_option *opt;
	sigC_config *cfg;

	//	allocate config;
	sigC_settings = malloc(sizeof(sigC_config) * arg_count);

	while (ndx < arg_count)
	{
		opt = sigc->options + ndx;

		cfg = malloc(sizeof(sigC_config));
		if (cfg)
		{
			cfg->configure = opt->configure;
			cfg->param = opt->param;
		}
		sigC_settings[ndx] = *cfg;

		++ndx;
	}

	(*sigc).cfgc = arg_count;

	return retOk;
}
static bool sc_configure()
{
	bool retOk = true;
	sigC_config *ptr = sigC_settings;
	sigC_config cfg = *ptr;

	while ((ptr - sigC_settings) < sigc->cfgc && retOk)
	{
		retOk = cfg.configure(cfg.param);
		cfg = *++ptr;
	}

	return retOk;
}
static int process_args(char **argv)
{
	char **ndx = argv;
	char *arg = *ndx;
	int arg_count = 0;
	IOType io_type;
	sigC_option *opt = NULL;
	sigc->options = malloc(sizeof(sigC_option) * (BLTIN_OPT_CNT + USR_OPT_COUNT));

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
		else if (*(ndx + 1) == NULL && (io_type = Path.type(arg) != IO_UNKNOWN))
		{
			//	we are at the last arg - expect FILE or PATH
			opt = set_source_option(arg, io_type);
		}
		else
		{
			printf("invalid option: %s\n", arg);
			//	set arg_count to 0 because we don't want to proceed
			ndx = argv;

			break;
		}

		//	printf("[%ld] %s\n", ndx - argv, arg);

		if (opt)
		{
			//	printf("option valid: %s\n", opt->tag);
			sigc->options[ndx - argv] = *opt;
		}
		else
		{
			printf("invalid option: %s\n", arg);
			//	set arg_count to 0 because we don't want to proceed
			free(sigc->options);
			sigc->options = NULL;
			ndx = argv;

			break;
		}

		arg = *++ndx;
	}
	arg_count = ndx - argv;

	if (arg_count > 0)
	{
		//		printf("sigc->actions[%d] = NULL\n", arg_count);
		sigc->options[arg_count] = (sigC_option){OPT_NOP, OPT_NO_OP, NULL, NULL};
	}

	return arg_count;
}
static int find_source_index(char **const source, char *item)
{
	char **ptr = source;
	char *pItem = *ptr;
	int ndx = -1;

	while (pItem)
	{
		if (strcmp(item, pItem) == 0)
		{
			ndx = ptr - source;
			break;
		}

		pItem = *++ptr;
	}

	return ndx;
}
static sigC_option *get_option_by_tag(char *tag)
{
	int tagNdx = find_source_index(TAG_OPTIONS, tag);
	enum tag_opt tagOption = tagNdx;
	sigC_option *ndx = usr_options;
	sigC_option opt = *ndx;
	while (ndx - usr_options < USR_OPT_COUNT)
	{
		if (opt.opt_type == OPT_TAG && opt.option.tag == tagOption)
		{
			return ndx;
		}
		opt = *++ndx;
	}

	return NULL;
}
static sigC_option *get_option_by_key(char *key)
{
	int keyNdx = find_source_index(KEY_OPTIONS, key);
	enum key_opt keyOption = keyNdx;
	sigC_option *ndx = usr_options;
	sigC_option opt = *ndx;
	while (ndx - usr_options < USR_OPT_COUNT)
	{
		if (opt.opt_type == OPT_KEY && opt.option.key == keyOption)
		{
			return ndx;
		}
		opt = *++ndx;
	}

	return NULL;
}
static sigC_option *set_source_option(char *pPath, IOType io_type)
{
	sigC_option *opt = &bltin_optns[SOURCE];
	opt->configure = &set_source_param;
	opt->param = malloc(sizeof(sigC_param));
	opt->param->params = pPath;
	opt->param->type = SOURCE;

	return opt;
}
static bool set_source_param(sigC_param *param)
{
	//	TODO: read source input parameter
	printf("[TODO] Source: %s\n", param->params);

	return true;
}
static void parse_built_in_options()
{
	sigC_option *ndx = bltin_optns;
	while (ndx - bltin_optns < BLTIN_OPT_CNT)
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
static void parse_options()
{
	//	printf("parse options:\n");

	sigC_option *ndx = usr_options;
	while (ndx - usr_options < USR_OPT_COUNT)
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

static bool id10t_compatibility_mode(sigC_param *param)
{
	printf("ID10t compatibility mode [TRUE]\n");

	return false;
}
static bool display_help(sigC_param *param)
{
	//	TODO: options table
	printf("[TODO] Usage & Options ====\n");

	return true;
}
static bool display_info(sigC_param *param)
{
	//	TODO: verbose info
	printf("[TODO] Info ===============\n");

	return false;
}
static bool display_version(sigC_param *param)
{
	//	TODO: version
	printf("[TODO] Version: \n");

	return true;
}
static bool display_default(sigC_param *param)
{
	//	TODO: default
	printf("[TODO] Default configuration: \n");

	return true;
}
static bool set_output_file(sigC_param *param)
{
	//	TODO: output file parameters
	printf("[TODO] Output: %s\n", param ? param->params : "[using default]");

	return true;
}
//==============================================

/*
 * 	Relays reference to the Sigma.C compiler. If the compiler has not
 * 	been initialized, this will run initialization.
 *
 * 	sigC**:	reference to the compiler
 * 	bool:	returns TRUE if valid reference;
 * 			otherwise FALSE
 */
bool __sc_instance(sigC **sc)
{
	if (!sigc && sc_init())
	{
		(*sc) = sigc;
	}
	else
	{
		(*sc) = NULL;
	}

	return (*sc) != NULL;
}
bool __sc_load_codex()
{
	bool retOk = Codex.init(sigc->cwd, &(sigc->codex));
	if (!retOk)
	{
		printf("error loading Sigma.C Codex\n");
	}

	return retOk;
}

const struct SigmaC SC = {
	.instance = &__sc_instance,
	.load_codex = &__sc_load_codex};
