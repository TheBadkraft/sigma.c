#ifndef _INDEXER_H
#define _INDEXER_H

#include <stdio.h>

#include "open/io_stream.h"
#include "open/string.h"

#include "token.h"

typedef struct document_t
{
    string *name;
    string *content;
} document;

typedef bool (*index_delegate)(document *, token **);

typedef struct io_indexer
{
    const char *name;
    index_delegate tokenize;
} indexer;

extern const struct Document_T
{
    bool (*load)(stream *, document **);
} Document;

extern const struct IO_Indexer
{
    indexer *(*init)(char *, index_delegate);
    index_delegate index;
} Indexer;
#endif //  _INDEXER_H
