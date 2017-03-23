/*
Copyright 2016 Sylvain "Skarsnik" Colinet

 This file is part of the SkarAlttp project.

    SkarAlttp is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SkarAlttp.  If not, see <http://www.gnu.org/licenses/>
    */

#ifdef __cplusplus
extern "C" {
#endif
#include "tile.h"
#include <assert.h>

tile8 unpack_bpp2_tile(const char* data, const unsigned int offset)
{
    return (unpack_bpp_tile(data, offset, 2));
}

tile8 unpack_bpp3_tile(const char* data, const unsigned int offset)
{
    return (unpack_bpp_tile(data, offset, 3));
}

tile8 unpack_bpp4_tile(const char* data, unsigned int offset)
{
    return (unpack_bpp_tile(data, offset, 4));
}


tile8 unpack_bpp_tile(const char *data, const unsigned int offset, const unsigned bpp)
{
    tile8	tile;
    assert(bpp >= 2 && bpp <= 4);
    unsigned int bpp_pos[4]; // More for conveniance and readibility
    for (int col = 0; col < 8; col++)
    {
        for (int row = 0; row < 8; row++)
        {
	    /* SNES bpp format interlace each byte of the first 2 bitplanes.
	     * | byte 1 of first bitplane | byte 1 of the second bitplane | byte 2 of first bitplane | byte 2 of second bitplane | ..
	     */
            bpp_pos[0] = offset + col * 2;
            bpp_pos[1] = offset + col * 2 + 1;
            char mask = 1 << (7 - row);
            tile.data[col * 8 + row] = (data[bpp_pos[0]] & mask) == mask;
            tile.data[col * 8 + row] |= ((data[bpp_pos[1]] & mask) == mask) << 1;
            if (bpp == 3)
            {
	        // When we have 3 bitplanes, the bytes for the third bitplane are after the 16 bytes of the 2 bitplanes.
                bpp_pos[2] = offset + 16 + col;
                tile.data[col * 8 + row] |= ((data[bpp_pos[2]] & mask) == mask) << 2;
            }
            if (bpp == 4)
            {
	        // For 4 bitplanes, the 2 added bitplanes are interlaced like the first two.
                bpp_pos[2] = offset + 16 + col * 2;
                bpp_pos[3] = offset + 16 + col * 2 + 1;
                tile.data[col * 8 + row] |= ((data[bpp_pos[2]] & mask) == mask) << 2;
                tile.data[col * 8 + row] |= ((data[bpp_pos[3]] & mask) == mask) << 3;
            }
        }
    }
    return tile;
}
#ifdef __cplusplus
}
#endif
