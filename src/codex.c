/*	Created on: Aug 15, 2024
 *	Author: Badkraft
 *
 *	codex.c:
 */

#include "codex.h"
#include "ebnf_indexer.h"

static codex sigc_codex;

static const string DEF_PATH = ".data/sigmac.def";
static file DEF_SOURCE = NULL;

//	parser version
const byte prs_maj = 0;
const byte prs_min = 0;
const ushort prs_bld = 1;
const RC prs_rc = ALPHA;
string prs_label = "stark";

//	PROTOTYPES
static bool __cdx_init(directory);
static bool __cdx_load_definition();

static parser __prs_new();
static bool __prs_load_ruleset();
//	DEFINITIONS
//	========================= Codex =========================

/*
	Initialize Codex with current working directory
*/
static bool __cdx_init(directory pCWDir)
{
	sigc_codex = NULL;

	string def_path;
	String.new(0, &def_path);
	Path.combine(def_path, pCWDir->path, DEF_PATH, NULL);

	bool retOk = String.length(def_path) > 0 && Path.exists(def_path);
	if (retOk)
	{
		DEF_SOURCE = File.new(def_path);
		free(def_path);

		sigc_codex = malloc(sizeof(codex));
	}

	if (!sigc_codex)
	{
		retOk = false;
		//	some error status message would be nice
	}
	else
	{
		sigc_codex->parser = __prs_new();
		retOk = sigc_codex->parser != NULL;
	}

	return retOk;
}
/*
	Load language definition file
*/
static bool __cdx_load_definition()
{
	bool retOk = true;
	stream pStream;

	if ((pStream = Stream.new(DEF_PATH)) != NULL)
	{
		if (!(retOk = Document.load(pStream, &(sigc_codex->definition))))
		{
			printf("error loading doc (%s) from stream (%s)\n", sigc_codex->definition->name, DEF_SOURCE->path);
		}
	}
	else
	{
		printf("error creating stream: %s", DEF_PATH);
	}

	return retOk;
}
//	========================= Parser ========================
static parser __prs_new()
{
	parser pParser = malloc(sizeof(struct sc_parser));

	if (pParser)
	{
		pParser->prs_version = Version.new(prs_maj, prs_min, prs_bld, prs_rc, prs_label);
	}

	return pParser;
}
static bool __prs_load_ruleset()
{
	bool retOk = true;
	indexer pIndexer;
	Indexer.init(sigc_codex->definition->name, ebnf_tokenize, &pIndexer);

	token pToken;
	pIndexer->tokenize(sigc_codex->definition, &pToken);

	char *word = NULL;
	token ptr = pToken;
	while (ptr)
	{
		Token.word(ptr, &word);
		printf("%s\n", word);

		ptr = ptr->next;
	}

	return retOk;
}

//	=========================================================

/*
 * 	Relays reference to the Sigma.C Codex. The codex will only initialize once per
 *	execution run of the compiler. The compiler will be given reference to the codex.
 *
 * 	sigC**:	reference to the compiler
 * 	bool:	returns TRUE if valid reference;
 * 			otherwise FALSE
 */
bool __codex_instance(directory pCWDir, codex *cdx)
{
	bool retOk = true;

	if (!sigc_codex && __cdx_init(pCWDir))
	{
		(*cdx) = sigc_codex;
		retOk = __cdx_load_definition();
	}
	else
	{
		(*cdx) = NULL;
		retOk = false;
	}

	if (retOk)
	{
		retOk = __prs_load_ruleset();
	}

	return retOk;
}

const struct SC_Codex Codex = {
	.init = &__codex_instance};
