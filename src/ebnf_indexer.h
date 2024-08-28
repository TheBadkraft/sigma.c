#ifndef _EBNF_INDEXER_H
#define _EBNF_INDEXER_H

#include "open/types.h"
#include "../core/indexer.h"

const char SPACE = ' ';
const char NEWLINE = '\n';
const char PCT_ENDLINE = ';';

const char *ASSIGNMENT = ":=";

static bool is(char c, char cmp)
{
    return c == cmp;
}
static bool is_noise(char *c)
{
    return is(*c, SPACE) || is(*c, NEWLINE);
}
static char *get_word(char *ptr, size_t *wrdLen, size_t strLen)
{
    char *pPos = ptr;
    char *end;

    while (pPos - ptr < strLen)
    {
        if (is(*pPos, SPACE))
        {
            //  (*j)++;
            (*wrdLen) = (pPos - ptr);
            end = pPos - 1;

            break;
        }
        ++pPos;
    }

    return end;
}
bool ebnf_tokenize(document *pDoc, token **pTknHead)
{
    char *pSrc;
    char *ptr = pSrc = pDoc->content->buffer;
    size_t ndx = 0;
    size_t length = 0;
    size_t srcLen = strlen(pSrc);
    int line = 1;
    char *pWrd;
    (*pTknHead) = NULL;
    token *pCurrent = NULL;

    while (ndx < srcLen)
    {
        if (is_noise(ptr))
        {
            ++ptr;
            ++ndx;
            continue;
        }
        else if (is(*ptr, PCT_ENDLINE))
        {
            pWrd = ptr;
            length = 1;
        }
        else
        {
            pWrd = get_word(ptr, &length, srcLen - ndx);
        }
        //    	printf("[%d] start: %ld len: %ld    [w:%c-%c]\n", line, start, length, ptr[0], pWrd[0]);

        if (!(*pTknHead))
        {
            pCurrent = (*pTknHead) = Token.new(ptr, length);
        }
        else if (!pCurrent->next)
        {
            pCurrent = pCurrent->next = Token.new(ptr, length);
        }

        ndx += length;
        ptr = pWrd + 1;
    }
    return (*pTknHead) != NULL;
}

#endif //  _EBNF_INDEXER_H