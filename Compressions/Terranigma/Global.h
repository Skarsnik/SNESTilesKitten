/*

	TD - A tool to decompress data from the SNES game Terranigma

	Copyright (C) 2009-2012  crediar


    TD is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    TD is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with TD.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef __GLOBAL__
#define __GLOBAL__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <tchar.h>

#define ADR(x)	((x)&0x3fffff)

typedef unsigned __int64 u64;
typedef signed __int64 s64;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef signed char s8;

u16 BS16( u16 s );

#endif
