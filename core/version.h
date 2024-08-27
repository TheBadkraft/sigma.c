#ifndef _OPEN_VERSION_H
#define _OPEN_VERSION_H

#include "open/core.h"

enum RC
{
    ALPHA,
    BETA,
    RELEASE
};
#define relc enum RC

typedef struct version_t
{
    byte major;
    byte minor;
    ushort build;
    relc release;
    char *descr;
} version;

extern const struct Version_T
{
    char *(*release)(relc);
    char *(*version)(version *);
    char *(*to_string)(version *);
    version *(*new)(byte, byte, ushort, relc, char *);
} Version;

#endif //  _OPEN_VERSION_H
