/*	Created on: Aug 15, 2024
 *	Author: Badkraft
 *
 *	codex.c:
 */

#include "codex.h"
#include "ebnf_indexer.h"

static codex sigc_codex;
static file def_file = NULL;

static const string DEF_PATH = ".data/sigmac.def";

//	parser version
const byte prs_maj = 0;
const byte prs_min = 0;
const ushort prs_bld = 1;
const RC prs_rc = ALPHA;
string prs_label = "stark";

//	PROTOTYPES
static bool codex_init(directory);
static bool codex_load_definition();
static void codex_dispose();

static parser parser_new();
static bool parser_load_ruleset();
//	DEFINITIONS
//	========================= Codex =========================

/*
	Initialize Codex with current working directory
*/
static bool codex_init(directory cwDir)
{
	sigc_codex = NULL;

	string def_path;
	String.new(0, &def_path);
	Path.combine(&def_path, cwDir->path, DEF_PATH, NULL);

	bool retOk = String.length(def_path) > 0 && Path.exists(def_path);
	if (retOk)
	{
		def_file = File.new(def_path);
		String.free(def_path);

		sigc_codex = Allocator.alloc(sizeof(codex), UNINITIALIZED);
	}

	if (!sigc_codex)
	{
		retOk = false;
		//	some error status message would be nice
	}
	else
	{

		sigc_codex->parser = parser_new();
		retOk = sigc_codex->parser != NULL;
	}

	return retOk;
}
/*
	Load language definition file
*/
static bool codex_load_definition()
{
	bool retOk = true;
	stream pStream;

	if ((pStream = Stream.new(DEF_PATH)) != NULL)
	{
		if (!(retOk = Document.load(pStream, &(sigc_codex->definition))))
		{
			printf("error loading doc (%s) from stream (%s)\n", sigc_codex->definition->name, def_file->path);
		}
	}
	else
	{
		printf("error creating stream: %s", DEF_PATH);
	}

	return retOk;
}
//	========================= Parser ========================
static parser parser_new()
{
	parser pParser = Allocator.alloc(sizeof(struct sc_parser), UNINITIALIZED);

	if (pParser)
	{
		pParser->prs_version = Version.new(prs_maj, prs_min, prs_bld, prs_rc, prs_label);
	}

	return pParser;
}
static bool parser_load_ruleset()
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

	if (!sigc_codex && codex_init(pCWDir))
	{
		(*cdx) = sigc_codex;
		retOk = codex_load_definition();
	}
	else
	{
		(*cdx) = NULL;
		retOk = false;
	}

	if (retOk)
	{
		retOk = parser_load_ruleset();
	}

	return retOk;
}

const struct SC_Codex Codex = {
	.init = &__codex_instance};
