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
#include "Comp.h"

u8 *Comp::ROM = (u8*)NULL;

void Comp::Init( u8 *_ROM )
{
	ROM = _ROM;
}
u8 Comp::GetByte( u32 Increase = 1 )
{
	u8 Byte = ROM[ROMOffset];

	if( Increase )
		++ROMOffset;

	return Byte;
}
u16 Comp::GetWord( u32 Increase = 1, u32 Swap = 1 )
{
	u16 Word = GetByte();
	if( Swap )
		Word = Word | GetByte()<<8;
	else
		Word = Word<<8 | GetByte();

	if(!Increase)
		ROMOffset-=2;

	return Word;
}
u32 Comp::GetU24( u32 Increase = 1 )
{
	u32 Value = GetByte();
		Value = Value | GetByte()<<8;
		Value = Value | GetByte()<<16;
		
	if(!Increase)
		ROMOffset-=3;

	return Value;
}
u8 Comp::GetBit( void )
{
	u8 Bit;

	if( Counter < 0 )	// We need to load a new byte!
	{
		Counter = 7;
		InfoByte = GetByte();
		//printf("CMD:%02X ", InfoByte );
		//for( u32 i=0; i < 8; i++)
		//	printf("%d ", (InfoByte>>(7-i)) & 1 );
		//printf("\n");
	}

	Bit = !!(InfoByte & ( 1 << Counter ));
	Counter--;
	
	//printf("[%d]", Bit );
	return Bit;
}
u8 *Comp::UncompressData( u32 Offset, u32 *Size )
{
	u8 Length;
	u16 Word;

	ROMOffset = ADR( Offset + 1 );		//First byte is always 0x00
	CompOff = 0;
	Counter = -1;
	*Size = GetWord();

	//Optional sanity check
	//switch( *Size )
	//{
	//	case 0x6000:
	//	case 0x4000:
	//	case 0x2800:
	//	case 0x3000:
	//	case 0x2000:
	//	case 0x1C00:
	//	case 0x1800:
	//	case 0x1000:
	//	case 0xC00:
	//	case 0x800:
	//	case 0x600:
	//	case 0x400:
	//	case 0x200:
	//	case 0x100:
	//	case 0x101:
	//	case 0x7d00:
	//	break;
	//	default:
	//		//printf("UncompressData: Size:%04X\n", *Size );
	//		//return NULL;
	//		break;
	//}

	CompBuff = new u8[ *Size ];
    //printf("Comp:UncompressData: Size:%04X\n", *Size );
	
	//First byte always uncompressed!
	//printf("Write %02X\n", GetByte(0) );
	CompBuff[CompOff++] = GetByte();

	while(1)
	{
		//printf("[CO:%04X]", CompOff );
		if( GetBit() )		// 1
		{
			//printf("Write %02X\n", GetByte(0) );
			CompBuff[CompOff++] = GetByte();

		} else {		// 0

			if( GetBit() )	// 0 1
			{
				Word = BS16( GetWord() );

				if( Word & 0x0007 )
				{
					Length = (Word&0007) + 1;
				} else {
					Length = GetByte();
					if( Length == 0 )
					{
						*Size = CompOff;
						break;
					}
				}

					
				Word = Word>>3;
				Word = Word | 0xE000;
				Word = Word+CompOff;
				
				//printf("[1]Copy %02X from %04X to %04X\n", Length, Word, CompOff );
				for( u32 i=0; i <= Length; ++i )
				{
					if( Word >= *Size)
					{
						delete CompBuff;
						return NULL;
					}
					CompBuff[ CompOff++ ] = CompBuff[ Word++ ];
				}

			} else {						// 0 0

				Length = GetBit()<<1;
				Length = (Length | GetBit()) + 2;

				Word = GetByte() | 0xFF00;
				Word = Word+CompOff;
	
				//printf("[2]Copy %02X from %04X to %04X\n", Length, Word, CompOff );
				for( u32 i=0; i < Length; ++i )
				{
					//printf("[%04X]=[%04X]\n", CompOff ,Word );
					if( Word >= *Size)
					{
						delete CompBuff;
						return NULL;
					}
					CompBuff[ CompOff++ ] = CompBuff[ Word++ ];
				}
			}
		}
	}
	
	return CompBuff;
}
