#ifndef _CTYPES_H
#ifndef __ctypes_build
#define __ctypes_build "0005"
#endif
#define _CTYPES_H

#include <string.h>
#include <stdbool.h>

typedef unsigned char byte;
typedef unsigned short ushort;
typedef char sbyte;

#define TF(val) val ? "true" : "false"
#define CHAR_NE(val) val == '\0'

#endif //  _CTYPES_H
