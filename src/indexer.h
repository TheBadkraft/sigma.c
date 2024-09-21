#ifndef _INDEXER_H
#define _INDEXER_H

#include <stdio.h>

#include "open/io.h"
#include "open/string.h"
#include "open/token.h"

struct io_document
{
    string source;
    string content;
};

typedef struct io_document *document;
typedef bool (*index_delegate)(document, token *);

struct io_indexer
{
    string name;
    index_delegate tokenize;
};

typedef struct io_indexer *indexer;

extern const struct Document_T
{
    bool (*load)(stream, document *);
} Document;

extern const struct IO_Indexer
{
    bool (*init)(string, index_delegate, indexer *);
    index_delegate index;
    //  TODO: !!!
    void (*dispose)(void);
} Indexer;

//  generic indexer functions
static const char SPACE = ' ';
static const char NEWLINE = '\n';
static const char ENDLINE = ';';

static bool is(char c, char cmp)
{
    return c == cmp;
}
static bool any(char c, string cmps)
{
    //  to use this properly, terminate series with '\0'
    bool retAny = false;
    char *ndx = cmps;
    while (*ndx != '\0')
    {
        retAny = is(c, *ndx);

        if (retAny)
        {
            break;
        }

        ++ndx;
    }

    return retAny;
}
static bool is_noise(char c)
{
    return is(c, SPACE) || is(c, NEWLINE);
}
static char *get_word(char *pChar, string delims, size_t strLen)
{
    if (delims == NULL)
    {
        delims = " \0";
    }
    char *ndx = pChar;
    char *end;

    while (ndx - pChar < strLen)
    {
        if (any(*ndx, delims))
        {
            end = ndx - 1;

            break;
        }
        ++ndx;
    }

    return end;
}
#endif //  _INDEXER_H
