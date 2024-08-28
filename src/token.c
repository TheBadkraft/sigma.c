#include <stdlib.h>
#include <string.h>

#include "../core/token.h"

token *__token_new(char *, size_t);
bool __token_word(token *, char **);
void __token_free(token *);
/*
 * Create a new token from begin and length
 */
token *__token_new(char *pPos, size_t length)
{
	token *pToken = malloc(sizeof(token));
	pToken->pPos = pPos;
	pToken->length = length;
	//	pToken->prev = NULL;
	pToken->next = NULL;

	return pToken;
}
bool __token_word(token *pToken, char **pWord)
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
void __token_free(token *pToken)
{
	token *pTemp;
	while (pToken)
	{
		pTemp = pToken;
		pToken = pToken->next;
		free(pTemp);
	}
}

const struct Token_T Token = {
	.new = &__token_new,
	.word = &__token_word,
	.free = &__token_free};
