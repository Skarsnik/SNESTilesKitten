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
#pragma once

#include "Global.h"

class Comp
{
	private:
		u32 ROMOffset;
		static u8 *ROM;
		
		u32 CompOff;
		u8 *CompBuff;
		u8 InfoByte;
		s8 Counter;
		
		u8		GetBit		( void );
		
		u8		GetByte		( u32 Increase );
		u16		GetWord		( u32 Increase, u32 Swap );
		u32		GetU24		( u32 Increase );

	public:

		static void Init( u8 *ROM );
		u8 *UncompressData( u32 Offset, u32 *Size );
};

