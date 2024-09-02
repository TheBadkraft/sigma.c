#ifndef _INDEXER_H
#define _INDEXER_H

#include <stdio.h>

#include "open/io.h"
#include "open/string.h"
#include "open/token.h"

struct io_document
{
    string name;
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
} Indexer;
#endif //  _INDEXER_H
