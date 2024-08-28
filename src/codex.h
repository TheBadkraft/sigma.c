/*	Created on: Aug 15, 2024
 *	Author: Badkraft
 *
 *	codex.h:
 */

#ifndef _CODEX_H_
#define _CODEX_H_

// #include "open/io.h"
#include "open/io_stream.h"
#include "../core/indexer.h"
#include "../core/version.h"

typedef token *rule;

struct sc_rule
{
	rule rule_def;
};

struct sc_parser
{
	struct
	{
		struct sc_rule *ruleset;
	} ruleset;
	version *prs_version;
};

struct sc_codex
{
	document *definition;
	struct sc_parser *parser;
};

typedef struct sc_codex codex;
typedef struct sc_parser parser;

extern const struct SC_Codex
{
	//	initialize a singleton instance of the Codex
	bool (*init)(directory *, codex **);
} Codex;

#endif /* _CODEX_H_ */
