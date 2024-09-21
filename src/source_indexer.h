#ifndef _SOURCE_INDEXER_H
#define _SOURCE_INDEXER_H

#include "open/types.h"
#include "indexer.h"

const char OPN_BRACE = '{'; // 2:'{'!'}'
const char CLS_BRACE = '}'; // ...
const char OPN_BRACK = '['; // 2:'['!']'
const char CLS_BRACK = ']'; //
const char OPN_PAREN = '('; // 2:'('!')'
const char CLS_PAREN = ')'; // ...

static bool is_whitespace(char);
static bool is_newline(char);
static bool is_symbol(char);
static bool is_paired(char);

/*
    This indexer will tokenize the ENDLINE char. Since `token` isn't aware of its position or line
    number, we will keep the '\n' for the Parser to evaluate.
*/
bool index_source(document pDoc, token *pTknHead)
{
    //     printf("%s\n", pDoc->source);
    string pSrc;
    char *ndx = pSrc = pDoc->content;
    size_t pos = ndx - pSrc;
    size_t length = 0;
    size_t srcLen = String.length(pSrc);
    char *wrdEnd;
    (*pTknHead) = NULL;
    token pCurrent = NULL;

    while (pos < srcLen)
    {
        if (is_whitespace(*ndx))
        {
            ++ndx;
            // ++pos;
            pos = ndx - pSrc;

            continue;
        }
        // else if (is(*ndx, NEWLINE))
        // {
        //     ++ndx;
        //     length = 1;
        //     // ++pos;

        //     //  we want to tokenize '\n' to preserve EOL for Parser evaluation
        //     // continue;
        // }
        else if (is(*ndx, NEWLINE) || is_symbol(*ndx))
        {
            wrdEnd = ndx;
            length = 1;
        }
        else
        {
            //  pass series of possible (valid) word delimeters
            wrdEnd = get_word(ndx, " (){};\0", srcLen - pos);
            length = wrdEnd - ndx + 1;
        }
        //    	printf("[%d] start: %ld len: %ld    [w:%c-%c]\n", line, start, length, ptr[0], pWrd[0]);

        if (!(*pTknHead))
        {
            pCurrent = (*pTknHead) = Token.new(ndx, length);
        }
        else if (!pCurrent->next)
        {
            pCurrent = pCurrent->next = Token.new(ndx, length);
        }

        // pCurrent->type = UNKNOWN;
        // pCurrent->line = line;

        // pos += length;
        ndx = ++wrdEnd;
        pos = ndx - pSrc;
    }

    return (*pTknHead) != NULL;
}

static bool is_whitespace(char c)
{
    return is(c, SPACE);
}
static bool is_newline(char c)
{
    return is(c, NEWLINE);
}
static bool is_symbol(char c)
{
    return is_paired(c) || is(c, ENDLINE);
}
static bool is_paired(char c)
{
    return is(c, OPN_PAREN) || is(c, CLS_PAREN) || is(c, OPN_BRACE) || is(c, CLS_BRACE) || is(c, OPN_BRACK) || is(c, CLS_BRACK);
}

#endif //  _SOURCE_INDEXER_H
