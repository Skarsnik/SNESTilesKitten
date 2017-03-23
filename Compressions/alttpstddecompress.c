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


#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define INITIAL_ALLOC_SIZE 512

/*
 * The compression format follow a simple pattern:
 * first byte represente a header. The header represent a command and a lenght
 * The bytes after the header have meaning depending on the command
 * Then you have a new header byte and so on, until you hit a header with the value FF
 */

char*	decompress(const char *c_data, const unsigned int start, unsigned int* uncompressed_data_size)
{
    char*		u_data;
    unsigned char	header;
    unsigned int	c_data_pos;
    unsigned int	u_data_pos;
    unsigned int	allocated_memory;

    header = c_data[start];
    u_data = (char *) malloc(INITIAL_ALLOC_SIZE); // No way to know the final size, we will probably realloc if needed
    allocated_memory = INITIAL_ALLOC_SIZE;
    u_data_pos = 0;
    c_data_pos = start;
    while (header != 0xFF)
    {
        unsigned int lenght;
        char command;

        command = header >> 5; // 3 hightest bits are the command
        lenght = (header & 0x1F); // The rest is the lenght

        // Extended header, to allow for bigger lenght value than 127
        if (command == 7)
        {
            // 2 bits in the original header are the hight bit for the new lenght
            // the next byte is added to this lenght
            command = (header >> 2 ) & 7;
            lenght = ((int)((header & 3) << 8)) + (unsigned char) c_data[c_data_pos + 1];
            c_data_pos++;
        }

        //lenght value starts at 0, 0 is 1
        lenght++;

        printf("header %2X - Command : %d , lenght : %d\n", header, command, lenght);

        if (u_data_pos + lenght > allocated_memory) // Adjust allocated memory
        {
            u_data = realloc(u_data, allocated_memory + INITIAL_ALLOC_SIZE);
            allocated_memory += INITIAL_ALLOC_SIZE;
        }
        switch (command)
        {
        case 0: { // No compression, data are copied as
            memcpy(u_data + u_data_pos, c_data + c_data_pos + 1, lenght);
            c_data_pos += lenght + 1;
            break;
        }
        case 1: { // Copy the same byte lenght time
            memset(u_data + u_data_pos, c_data[c_data_pos + 1], lenght);
            c_data_pos += 2;
            break;
        }
        case 2: { // Next byte is A, the one after is B, copy the sequence AB lenght times
            char a = c_data[c_data_pos + 1];
            char b = c_data[c_data_pos + 2];
            for (int i = 0; i < lenght; i = i + 2)
            {
                u_data[u_data_pos + i] = a;
                if ((i + 1) < lenght)
                    u_data[u_data_pos + i + 1] = b;
            }
            c_data_pos += 3;
            break;
        }
        case 3: { // Next byte is copied and incremented lenght time
            for (int i = 0; i < lenght; i++) {
                u_data[u_data_pos + i] = c_data[c_data_pos + 1] + i;
            }
            c_data_pos += 2;
            break;
        }
        case 4: { // Next 2 bytes form an offset to pick data from the output (dafuq?)
            int	offset = c_data[c_data_pos + 1] | (c_data[c_data_pos + 2] << 8);
            memcpy(u_data + u_data_pos, u_data + offset, lenght);
            c_data_pos += 3;
            break;
        }
        default: {
            fprintf(stderr, "Invalid command in the header for decompression\n");
            return NULL;
        }

        }
        u_data_pos += lenght;
        header = c_data[c_data_pos];
    }
    *uncompressed_data_size = u_data_pos;
    return u_data;
}
