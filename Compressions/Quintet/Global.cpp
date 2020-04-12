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
#include "Global.h"

u16 BS16( u16 s )
{
	return ((s&0xFF)<<8) | ((s&0xFF00)>>8);
}
