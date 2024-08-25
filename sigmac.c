/*	Created on: Aug 11, 2024
 *  Author: Badkraft
 *
 *  sigmac.c
 */

#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "sigmac.h"

const char *const CSTRING = "cstring";
const char *const CFILE = "file";
const char *const LEXER = "lexer";
const char *const CTYPES = "ctypes";
const char *const VERSION = "version";

const char **const COMPONENTS = (const char*[] ) {
	"cstring", "file", "lexer", "ctypes", "version", NULL
};

char *const OPT_NULL = NULL;
char *const OPT_HELP = "help";
char *const OPT_INFO = "info";
char *const OPT_VERSION = "version";
char *const OPT_DEFAULTS = "defaults";

char const OPT_UNKN = -1;
char const OPT_NONE = '\0';
char const OPT_ID10T = 'i';
char const OPT_OUTPUT = 'o';
char const OPT_SOURCE = 'S';

//	TODO: place into a file for configuration ...???
const size_t USR_OPT_COUNT = 6;
const size_t BLTIN_OPT_CNT = 1;
struct sc_opt bltin_optns[] = {
		{ OPT_SOURCE, OPT_NULL, NULL, "Source file." }
};
struct sc_opt usr_options[] = {
	//	TODO: add property to enable/disable
	{ OPT_NONE, OPT_HELP, NULL, "Display command-line options and usage." },
	{ OPT_NONE,	OPT_INFO, NULL, "Full compiler and component version information." },
	{ OPT_NONE, OPT_VERSION, NULL, "Simple compiler version." },
	{ OPT_NONE, OPT_DEFAULTS, NULL, "Display default settings." },
	{ OPT_OUTPUT, OPT_NULL, NULL, "To output file" },
	{ OPT_ID10T, OPT_NULL, NULL, "ID:10T compatibility switch." }
};

static sigC *sigc;
static sigC_config *sigC_settings;

const byte maj = 0;
const byte min = 0;
const ushort bld = 1;
const relc rc = ALPHA;
char *label = "tuxedo";

static void parse_built_in_options();
static void parse_options();

static bool sc_load_configuration(char**);
static int process_args(char**);
static bool sc_configure();

static sigC_option *get_option_by_tag(char*);
static sigC_option *get_option_by_key(char);
static sigC_option *set_source_option(char*, IOType);

static bool set_source_param();

static bool display_help(sigC_param*);
static bool display_info(sigC_param*);
static bool display_version(sigC_param*);
static bool id10t_compatibility_mode(sigC_param*);
static bool set_output_file(sigC_param*);

static bool sc_init() {
	bool retOk = (sigc = malloc(sizeof(sigC))) != NULL;

	if(retOk) {
	sigc->name = "Sigma.C";
	sigc->ver = Version.new(maj, min, bld, rc, label);
	sigc->load = &sc_load_configuration;
	sigc->configure = &sc_configure;
	sigc->cwd = Directory.current_directory();
	sigc->path = File.get_directory(__FILE__);
	sigc->cdx_definition = "sigmac.def";

	printf("===== %s =====\n", sigc->name);
	printf("cwd:  %s\n", sigc->cwd);
	printf("path: %s\n", sigc->path);

	parse_built_in_options();
	parse_options();
	}
	else {
		//	pError("initialization error");
	}

	return retOk;
}
static bool sc_load_configuration(char **argv) {
	bool retOk = true;
	//	skip arg[0] CL: sigma.c
	int arg_count = process_args(++argv);
	int ndx = 0;
	sigC_option *opt;
	sigC_config *cfg;

	//	allocate config;
	sigC_settings = malloc(sizeof(sigC_config) * arg_count);

	while (ndx < arg_count) {
		opt = sigc->options + ndx;

		cfg = malloc(sizeof(sigC_config));
		if (cfg) {
			cfg->configure = opt->configure;
			cfg->param = opt->param;
		}
		sigC_settings[ndx] = *cfg;

		++ndx;
	}

	sigc->cfgc = arg_count;

	return retOk;
}
static int process_args(char **argv) {
	char **ndx = argv;
	char *arg = *ndx;
	int arg_count = 0;
	IOType io_type;
	sigC_option *opt = NULL;
	sigc->options = malloc(sizeof(sigC_option) * (BLTIN_OPT_CNT + USR_OPT_COUNT));

	/*
	 * 	Apparently, argv terminates with a NULL ...???
	 */
	while (arg) {
		//	need to know whether the arg starts with '--' or '-'
		if (arg[1] == '-' && arg[0] == '-') {
			arg += 2;
			//	find option by tag
			opt = get_option_by_tag(arg);
			//	TODO: some options will terminate any other options
		} else if (arg[0] == '-') {
			//	find option by key
			opt = get_option_by_key(arg[1]);
			//	TODO: some options will terminate any other options
		}
		else if (*(ndx + 1) == NULL && File.path_or_file(arg, &io_type)) {
			//	we are at the last arg - expect FILE or PATH
			opt = set_source_option(arg, io_type);
		}
		else {
			printf("invalid option: %s\n", arg);
			//	set arg_count to 0 because we don't want to proceed
			ndx = argv;

			break;
		}

//		printf("[%ld] %s\n", ndx - argv, arg);

		if(opt) {
//			printf("option valid: %s\n", opt->tag);
			sigc->options[ndx - argv] = *opt;
		}
		else {
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

	if(arg_count > 0) {
//		printf("sigc->actions[%d] = NULL\n", arg_count);
		sigc->options[arg_count] = (sigC_option){ -1, NULL, NULL, NULL };
	}

	return arg_count;
}
static bool sc_configure() {
	bool retOk = true;
	sigC_config *ptr = sigC_settings;
	sigC_config cfg = *ptr;

	while((ptr - sigC_settings) < sigc->cfgc && retOk) {
		retOk = cfg.configure(cfg.param);
		cfg = *++ptr;
	}

	return retOk;
}
static sigC_option *get_option_by_tag(char *tag) {
	sigC_option *ndx = usr_options;
	sigC_option opt = *ndx;
	while (ndx - usr_options < USR_OPT_COUNT) {
		if(strcmp(opt.tag, tag) == 0) {
			return ndx;
		}
		opt = *++ndx;
	}

	return NULL;
}
static sigC_option *get_option_by_key(char key) {
	sigC_option *ndx = usr_options;
	sigC_option opt = *ndx;
	while (ndx - usr_options < USR_OPT_COUNT) {
		if(opt.key == key) {
			return ndx;
		}
		opt = *++ndx;
	}

	return NULL;
}
static sigC_option *set_source_option(char *pPath, IOType io_type) {
	sigC_option *opt = &bltin_optns[SC_OPT_SOURCE];
	opt->configure = &set_source_param;
	opt->param = malloc(sizeof(sigC_param));
	opt->param->params = pPath;
	opt->param->type = SC_OPT_SOURCE;

	return opt;
}
static bool set_source_param(sigC_param* param) {
	//	TODO: read source input parameter
	printf("[TODO] Source: %s\n", param->params);


	return true;
}
static void parse_built_in_options() {
	sigC_option *ndx = bltin_optns;
	while(ndx - bltin_optns < BLTIN_OPT_CNT) {
		if(OPT_SOURCE == ndx->key) {
			ndx->configure = &set_source_param;
		}

		++ndx;
	}
}
static void parse_options() {
//	printf("parse options:\n");

	sigC_option *ndx = usr_options;
	while (ndx - usr_options < USR_OPT_COUNT) {
//		printf("option: %s\n", opt.tag);
		if (ndx->tag) {
			if (strcmp(OPT_HELP, ndx->tag) == 0) {
				ndx->configure = &display_help;
			} else if (strcmp(OPT_INFO, ndx->tag) == 0) {
				ndx->configure = &display_info;
			} else if (strcmp(OPT_VERSION, ndx->tag) == 0) {
				ndx->configure = &display_version;
			}
		} else {
			if (OPT_ID10T == ndx->key) {
				//	how can we bump this to the first option???
				ndx->configure = &id10t_compatibility_mode;
			} else if (OPT_OUTPUT == ndx->key) {
				ndx->configure = &set_output_file;
			}
		}

		++ndx;
	}
}
static bool id10t_compatibility_mode(sigC_param* param) {
	printf("ID10.t compatibility mode [TRUE]\n");

	return false;
}
static bool display_help(sigC_param* param) {
	//	TODO: options table
	printf("[TODO] Usage & Options ====\n");

	return true;
}
static bool display_info(sigC_param* param) {
	//	TODO: verbose info
	printf("[TODO] Info ===============\n");

	return false;
}
static bool display_version(sigC_param* param) {
	//	TODO: version
	printf("[TODO] Version: \n");

	return true;
}
static bool set_output_file(sigC_param* param) {
	//	TODO: output file parameters
	printf("[TODO] Output: %s\n", param ? param->params : "[using default]");

	return true;
}
//==============================================

/*
 * 	Relays reference to the Sigma.C compiler. If the compiler has not
 * 	been initialized, this will run initialization.
 *
 * 	sigC*:	reference to compiler object
 * 	bool:	returns TRUE if valid reference;
 * 			otherwise FALSE
 */
bool sc_instance(sigC *sc) {
	if (!sigc && sc_init()) {
		*sc = *sigc;
	}
	else {
		sc = NULL;
	}

	return sc != NULL;
}

const struct sigmac SC = {
		.instance = &sc_instance
};
