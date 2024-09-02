#include <stdio.h>

#include "open/io.h"
#include "open/internal/internal_io.h"

#include "indexer.h"

bool default_tokenizer(document, token *);

static bool __doc_load(stream, document *);

static index_delegate handler = default_tokenizer;

bool default_tokenizer(document pDoc, token *pToken)
{
    printf("[default indexer]\n");

    (*pToken) = malloc(sizeof(token));
    if ((*pToken))
    {
        (*pToken)->pPos = pDoc->content;
        (*pToken)->length = String.length(pDoc->content);
    }

    return (*pToken) != NULL;
}
//  =========================================================

bool indexer_init(string name, index_delegate delegate, indexer *pIndexer)
{
    printf("Sig.C Indexer [%s]... initializing\n", name);
    (*pIndexer) = malloc(sizeof(struct io_indexer));

    if (!pIndexer)
    {
        //  throwErr()
        printf("Error initializing Indexer ... OOPS!\n");
    }
    else
    { //  configure
        (*pIndexer)->name = name;
        if (delegate)
        {
            (*pIndexer)->tokenize = delegate;
        }
        else
        {
            (*pIndexer)->tokenize = handler;
        }
    }

    return (*pIndexer) != NULL;
}
bool indexer_index(document pDoc, token *pToken)
{
    return handler(pDoc, pToken);
}

//	======================================

static bool __doc_load(stream pStream, document *pDoc)
{
    if (pStream)
    {
        //  as long as stream is created in READ mode, this is unnecessary ...
        //      we do it here just in case to prevent an error
        if (!Stream.is_open(pStream))
        {
            if (!__open_stream(pStream, READ))
            {
                return false;
            }
            // if (!Stream.open(pStream, READ))
            // {
            //     return false;
            // }
        }

        (*pDoc) = malloc(sizeof(struct io_document));
        String.new(0, &(*pDoc)->name);
        String.copy((*pDoc)->name, pStream->source);
        String.new(pStream->length + 1, &(*pDoc)->content);

        int ndx = 0;
        // while (Stream.read(pStream, (*pDoc)->content->buffer + ndx))
        // {
        //     ++ndx;
        // }
    }

    return (*pDoc) != NULL;
}

const struct Document_T Document = {
    .load = &__doc_load,
};

const struct IO_Indexer Indexer = {
    .init = &indexer_init,
    .index = &indexer_index};
