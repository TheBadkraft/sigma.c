/*	Created on: Aug 15, 2024
 *	Author: Badkraft
 *
 *	codex.h:
 */

#ifndef _CODEX_H_
#define _CODEX_H_

#include "open/io.h"
#include "open/version.h"

#include "indexer.h"

struct sc_rule
{
	token definition;
};

struct sc_source
{
	token source;
};

struct sc_parser
{
	struct
	{
		struct sc_rule *ruleset;
	} ruleset;
	version prs_version;
};

struct sc_codex
{
	document definition;
	struct sc_parser *parser;
};

typedef struct sc_rule *rule;
typedef struct sc_source *source;
typedef struct sc_codex *codex;
typedef struct sc_parser *parser;

extern const struct SC_Codex
{
	//	initialize a singleton instance of the Codex
	bool (*init)(directory, codex *);
	void (*dispose)();
} Codex;

extern const struct SC_Parser
{
	bool (*add_source)(string);
	bool (*load_sources)(void);
} Parser;

#endif /* _CODEX_H_ */
