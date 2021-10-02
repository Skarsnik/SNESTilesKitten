#ifndef __GLOBAL__
#define __GLOBAL__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <stdint.h>

#define ADR(x)	((x)&0x3fffff)

typedef uint64_t u64;
typedef int64_t s64;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef signed char s8;

u16 BS16( u16 s );

#endif
