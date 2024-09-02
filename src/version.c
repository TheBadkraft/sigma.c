#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "open/types.h"
#include "open/version.h"

char **CANDIDATES = (char *[]){
    "alpha", "beta", "release"};

static version ver_new(byte maj, byte min, ushort bld, RC rel, string descr)
{
    version pVer = malloc(sizeof(struct Open_Version));
    pVer->major = maj;
    pVer->minor = min;
    pVer->build = bld;
    pVer->release = rel;
    pVer->descr = descr;

    return pVer;
}
static string rc_to_string(RC candidate)
{
    return CANDIDATES[candidate];
}
static string ver_to_string(version pVer)
{
    const char *ver_format = "%s%02d%s%02d%s%03d";
    char pre[] = "v.";
    char dot[] = ".";

    int length = snprintf(NULL, 0, ver_format, pre, pVer->major, dot, pVer->minor, dot, pVer->build);
    string str = malloc(length);

    sprintf(str, ver_format, pre, pVer->major, dot, pVer->minor, dot, pVer->build);

    return str;
}
static string ver_info(version pVer)
{
    const string ver_info_format = "%s(%s-%s)";
    string ver_string = ver_to_string(pVer);
    string ver_rc_string = rc_to_string(pVer->release);

    int length = snprintf(NULL, 0, ver_info_format, ver_string, ver_rc_string, pVer->descr);

    string str = malloc(length);
    sprintf(str, ver_info_format, ver_string, ver_rc_string, pVer->descr);

    return str;
}

const struct Version_T Version = {
    .release = &rc_to_string,
    .version = &ver_to_string,
    .to_string = &ver_info,
    .new = &ver_new};
