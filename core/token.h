#ifndef _TOKEN_H
#define _TOKEN_H

#include "open/types.h"

struct token_t
{
    char *pPos;
    size_t length;
    struct token_t *next;
};

//  non-specialized token
typedef struct token_t token;

extern const struct Token_T
{
    // Create a new token from begin and length
    token *(*new)(char *, size_t);
    bool (*word)(token *, char **);
    void (*free)(token *);
} Token;
#endif //  _TOKEN_H
