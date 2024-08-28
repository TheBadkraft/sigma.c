#include <stdio.h>

#include "open/io_stream.h"

#include "../core/indexer.h"

bool default_tokenizer(document *, token **);

static bool __doc_load(stream *, document **);

static index_delegate handler = default_tokenizer;

bool default_tokenizer(document *pDoc, token **pToken)
{
    printf("[default indexer]\n");

    (*pToken) = malloc(sizeof(token));
    if ((*pToken))
    {
        (*pToken)->pPos = pDoc->content->buffer;
        (*pToken)->length = String.length(pDoc->content);
    }

    return (*pToken) != NULL;
}
//  =========================================================

indexer *indexer_init(char *name, index_delegate delegate)
{
    printf("Sig.C Indexer [%s]... initializing\n", name);
    indexer *pIndexer = malloc(sizeof(indexer));

    if (!pIndexer)
    {
        //  throwErr()
        printf("Error initializing Indexer ... OOPS!\n");
        return NULL;
    }

    //  configure
    pIndexer->name = name;
    if (delegate)
    {
        pIndexer->tokenize = delegate;
    }
    else
    {
        pIndexer->tokenize = handler;
    }

    return pIndexer;
}
bool indexer_index(document *pDoc, token **pToken)
{
    return handler(pDoc, pToken);
}

//	======================================

static bool __doc_load(stream *pStream, document **pDoc)
{
    if (pStream)
    {
        //  as long as stream is created in READ mode, this is unnecessary ...
        //      we do it here just in case to prevent an error
        if (!pStream->fstream)
        {
            if (!Stream.open(pStream, READ))
            {
                return false;
            }
        }

        (*pDoc) = malloc(sizeof(document));
        (*pDoc)->name = String.new();
        (*pDoc)->content = String.new();
        String.copy((*pDoc)->name, pStream->source->name->buffer);
        String.capacity((*pDoc)->content, pStream->length + 1);

        int ndx = 0;
        while (Stream.read(pStream, (*pDoc)->content->buffer + ndx))
        {
            ++ndx;
        }
    }

    return (*pDoc) != NULL;
}

const struct Document_T Document = {
    .load = &__doc_load,
};

const struct IO_Indexer Indexer = {
    .init = &indexer_init,
    .index = &indexer_index};
