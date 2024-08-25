/*	Created on: Aug 15, 2024
 *	Author: Badkraft
 *	
 *	codex.h: 
 */

#ifndef _CODEX_H_
#define _CODEX_H_

#include "sigmac.h"

extern const struct sc_codex {
	bool (*init)(char*);
	//	codex definition file: .[/.data/]sigmac.def
	char* definition;
	bool has_definition;
} Codex;

#endif /* _CODEX_H_ */
