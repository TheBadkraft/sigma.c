/*	Created on: Aug 15, 2024
 *	Author: Badkraft
 *
 *	codex.c:
 */

#include "codex.h"
#include "ebnf_indexer.h"
#include "source_indexer.h"

static codex CODEX;
static file def_file = NULL;

static const string DEF_PATH = ".data/sigmac.def";

//	TODO: collection (list) type
static const int DEF_LIST_SIZE = 1;
static document SOURCES = NULL;

//	parser version
const byte prs_maj = 0;
const byte prs_min = 0;
const ushort prs_bld = 1;
const RC prs_rc = ALPHA;
string prs_label = "stark";

//	PROTOTYPES
static bool codex_init(directory);
static bool codex_load_definition(void);
static void codex_dispose(void);

static parser parser_new(void);
static bool parser_load_ruleset(void);
static bool parser_add_source(string);
//	DEFINITIONS
//	========================= Codex =========================

/*
	Initialize Codex with current working directory
*/
static bool codex_init(directory cwDir)
{
	CODEX = NULL;

	string def_path;
	String.new(0, &def_path);
	Path.combine(&def_path, cwDir->path, DEF_PATH, NULL);

	bool retOk = String.length(def_path) > 0 && Path.exists(def_path);
	if (retOk)
	{
		def_file = File.new(def_path);
		String.free(def_path);

		CODEX = Allocator.alloc(sizeof(codex), UNINITIALIZED);
	}

	if (!CODEX)
	{
		retOk = false;
		//	some error status message would be nice
	}
	else
	{

		CODEX->parser = parser_new();
		retOk = CODEX->parser != NULL;
	}

	return retOk;
}
/*
	Load language definition file
*/
static bool codex_load_definition()
{
	bool retOk = true;
	stream fStream;

	if ((fStream = Stream.new(DEF_PATH)) != NULL)
	{
		if (!(retOk = Document.load(fStream, &(CODEX->definition))))
		{
			printf("\nERROR: could not load doc (%s) from stream (%s)\n", CODEX->definition->source, def_file->path);
		}
	}
	else
	{
		printf("\nERROR: stream initialization failed: %s", DEF_PATH);
	}
	// Stream.free(fStream);

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
	Indexer.init(CODEX->definition->source, index_ebnf, &pIndexer);

	token pToken;
	pIndexer->tokenize(CODEX->definition, &pToken);

	// #if DEBUG
	char *word = NULL;
	token ptr = pToken;

	printf("\n");
	while (ptr)
	{
		Token.word(ptr, &word);
		printf("%s\n", word);

		ptr = ptr->next;
	}
	// #endif

	return retOk;
}
static bool parser_add_source(string srcPath)
{
	bool retOk = true;
	//	at the moment we are only adding one source file so not much error checking, etc. here
	if (!SOURCES)
	{
		SOURCES = Allocator.alloc(sizeof(document) * DEF_LIST_SIZE + 1, INITIALIZED);
	}

	//	load the document
	document srcDoc;
	stream fStream;

	if ((fStream = Stream.new(srcPath)) != NULL)
	{
		if (!(retOk = Document.load(fStream, &srcDoc)))
		{
			printf("\nERROR: could not load doc (%s) from stream (%s)\n", CODEX->definition->source, def_file->path);
		}
	}
	else
	{
		printf("\nERROR: stream initialization failed: %s", DEF_PATH);
	}
	//	TODO: investigate
	// Stream.free(fStream);

	SOURCES = srcDoc;

	return retOk && SOURCES != NULL;
}
static bool parser_load_sources(void)
{
	bool retOk = true;
	indexer pIndexer;
	//	again, for now this is working under the assumption that there is a single source file
	Indexer.init("source", index_source, &pIndexer);
	printf("\n");

	token pToken;
	pIndexer->tokenize(SOURCES, &pToken);

	// #if DEBUG
	string word = NULL;
	token ptr = pToken;
	while (ptr)
	{
		Token.word(ptr, &word);
		if (word == NULL)
		{
			break;
		}

		printf("%s%s", *word != '\0' ? word : "", *word == '\n' ? "" : "\n");

		ptr = ptr->next;
	}
	String.free(word);
	// #endif

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
bool codex_instance(directory pCWDir, codex *cdx)
{
	bool retOk = true;

	if (!CODEX && codex_init(pCWDir))
	{
		(*cdx) = CODEX;
		retOk = codex_load_definition();
	}
	else
	{
		(*cdx) = NULL;
		retOk = false;
	}

	if (retOk && (retOk = parser_load_ruleset()))
	{
		printf("Codex loaded!\n");
	}

	return retOk;
}
void codex_dispose()
{
	if (CODEX)
	{
		if (CODEX->parser)
		{
			Allocator.dealloc(CODEX->parser);
		}

		Allocator.dealloc(CODEX);
	}

	CODEX = NULL;
}

const struct SC_Codex Codex = {
	.init = &codex_instance,
	.dispose = &codex_dispose,
};
const struct SC_Parser Parser = {
	.add_source = &parser_add_source,
	.load_sources = &parser_load_sources,
};
