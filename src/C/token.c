#include <stdlib.h>
#include <string.h>

#include "open/token.h"

token token_new(char *, size_t);
bool token_word(token, string *);
void token_free(token);
/*
 * Create a new token from begin and length
 */
token token_new(char *pPos, size_t length)
{
	token pToken = Allocator.alloc(sizeof(token), UNINITIALIZED);
	pToken->pPos = pPos;
	pToken->length = length;
	//	pToken->prev = NULL;
	pToken->next = NULL;

	return pToken;
}
bool token_word(token pToken, string *pWord)
{
	if (!(*pWord))
	{
		(*pWord) = calloc(pToken->length, sizeof(char));
	}
	else
	{
		free((*pWord));
		(*pWord) = calloc(pToken->length, sizeof(char));
	}
	strncpy((*pWord), pToken->pPos, pToken->length);

	return (*pWord) != NULL;
}
void token_free(token pToken)
{
	token pTemp;
	while (pToken)
	{
		pTemp = pToken;
		pToken = pToken->next;
		free(pTemp);
	}
}

const struct Token_T Token = {
	.new = &token_new,
	.word = &token_word,
	.free = &token_free};
