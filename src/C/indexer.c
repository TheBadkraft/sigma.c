#include <stdio.h>
#include <string.h>

#include "open/io.h"
#include "open/internal/internal_io.h"

#include "indexer.h"

bool default_tokenizer(document, token *);

static bool doc_load(stream, document *);
static bool doc_dispose(document);

static index_delegate handler = default_tokenizer;

bool default_tokenizer(document pDoc, token *pToken)
{
    printf("[default indexer]\n");

    (*pToken) = Allocator.alloc(sizeof(token), INITIALIZED);
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
    printf("Sig.C Indexer [%s] ... initializing ", name);
    (*pIndexer) = Allocator.alloc(sizeof(struct io_indexer), UNINITIALIZED);

    if (!pIndexer)
    {
        //  throwErr()
        printf("\nERROR: Indexer could not be initialized ... OOPS!\n");
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

        printf("... ");
    }

    return (*pIndexer) != NULL;
}
bool indexer_index(document pDoc, token *pToken)
{
    return handler(pDoc, pToken);
}

//	======================================

static bool doc_load(stream pStream, document *doc)
{
    if (pStream)
    {
        //  as long as stream is created in READ mode, this is unnecessary ...
        //      we do it here just in case to prevent an error
        if (!Stream.is_open(pStream))
        {
            if (!open_stream(pStream, READ))
            {
                return false;
            }
            // if (!Stream.open(pStream, READ))
            // {
            //     return false;
            // }
        }

        (*doc) = Allocator.alloc(sizeof(struct io_document), INITIALIZED);
        String.new(0, &(*doc)->source);
        String.copy((*doc)->source, pStream->source);
        String.new(pStream->length, &(*doc)->content);

        char *ndx = (*doc)->content;
        int pos = ndx - (*doc)->content;
        while ((*ndx = (char)fgetc(pStream->fstream)) != EOF)
        {
            ++ndx;
        }

        //  tack NULL at the end
        *ndx = '\0';
    }

    return (*doc) != NULL;
}
static bool doc_dispose(document doc)
{
    String.free(doc->source);
    String.free(doc->content);
    return Allocator.dealloc(doc);
}

const struct Document_T Document = {
    .load = &doc_load,
    .dispose = &doc_dispose,
};

const struct IO_Indexer Indexer = {
    .init = &indexer_init,
    .index = &indexer_index};
