#include "Comp.h"

#define wndsize (1 << 10)
#define wndmask (wndsize - 1)
#define maxreps0 0x03
#define maxreps1 0x22
#define maxreps2 0x45

u8 *Comp::ROM = (u8 *)NULL;

void Comp::Init(u8 *_ROM)
{
	ROM = _ROM;
}

u8 Comp::read_byte(u8 *ROM, int *readoff)
{
	return (ROM[(*readoff)++]);
}

void Comp::write_byte(u8 *CompBuff, int *writeoff, u8 b)
{
	CompBuff[(*writeoff)++] = b;
}

u8 Comp::read_wnd_byte(u8 *window, int *wndoff)
{
	u8 b = window[*wndoff];
	*wndoff = (*wndoff + 1) & wndmask;
	return b;
}

void Comp::write_to_wnd(u8 *window, int *wndoff, u8 b)
{
	window[*wndoff] = b;
	*wndoff = (*wndoff + 1) & wndmask;
}

ushort Comp::read_word(u8 *ROM, int *readoff)
{
	ushort retn = read_byte(ROM, readoff) << 8;
	retn |= read_byte(ROM, readoff);
	return retn;
}

void Comp::write_word(u8 *CompBuff, int *writeoff, ushort w)
{
	write_byte(CompBuff, writeoff, w >> 8);
	write_byte(CompBuff, writeoff, w & 0xFF);
}

int Comp::read_cmd_bit(u8 *ROM, int *readoff, u8 *bitscnt, u8 *cmd)
{
	(*bitscnt)--;

	if (!*bitscnt)
	{
		*cmd = read_byte(ROM, readoff);
		*bitscnt = 8;
	}

	int retn = *cmd & 1;
	*cmd >>= 1;
	return retn;
}

void Comp::write_cmd_bit(int bit, u8 *CompBuff, int *writeoff, u8 *bitscnt, int *cmdoff)
{
	if (*bitscnt == 8)
	{
		*bitscnt = 0;
		*cmdoff = (*writeoff)++;
		CompBuff[*cmdoff] = 0;
	}

	CompBuff[*cmdoff] = ((bit & 1) << *bitscnt) | CompBuff[*cmdoff];
	bit >>= 1;
	(*bitscnt)++;
}

void Comp::init_wnd(u8 **window, u8 **reserve, int *wndoff)
{
	*window = (u8 *)malloc(wndsize);
	*reserve = (u8 *)malloc(wndsize);
	memset(*window, 0x00, wndsize);
	*wndoff = 0x3DF;
}

void Comp::find_matches(u8 *ROM, int readoff, int size, int wndoff, u8 *window, u8 *reserve, int *reps, int *from, int min_pos, int max_pos)
{
	int wpos = 0, tlen = 0;

	*reps = 1;
	wpos = min_pos;
	memcpy(reserve, window, wndsize);

	while (wpos < max_pos)
	{
		tlen = 0;
		while ((readoff + tlen < size && tlen < maxreps1) &&
			   window[(wpos + tlen) & wndmask] == ROM[readoff + tlen])
		{
			window[(wndoff + tlen) & wndmask] = ROM[readoff + tlen];
			tlen++;
		}

		if (tlen > *reps)
		{
			*reps = tlen;
			*from = wpos & wndmask;
		}

		memcpy(window, reserve, wndsize);
		wpos++;
	}
}

u8 *Comp::UncompressData(u32 Offset, u32 *Size)
{
	int size = 0, bit = 0, lzpair = 0, readoff = 0, writeoff = 0, wndoff = 0, reps = 0, from = 0;
	u8 bitscnt = 0, cmd = 0;
	ushort b = 0;
	u8 *window, *reserve;

	init_wnd(&window, &reserve, &wndoff);
	ROMOffset = ADR( Offset );
	readoff = 0;
	writeoff = 0;
	size = read_byte(ROM, &readoff) | ((read_byte(ROM, &readoff) << 8) & 0xFF00);
	CompBuff = new u8[ 0x10000 ];

	while (readoff - 2 < size)
	{
		b = read_byte(ROM, &readoff);

		if (b < 0x80)
		{
			lzpair = ((b << 8) | read_byte(ROM, &readoff));
			reps = ((lzpair >> 10) & 0xFF) + 2;
			//printf("[LZ]: %x\n", reps);
			from = (lzpair & wndmask);
		}
		else if (b >= 0x80 && b < 0xC0)
		{
			reps = (b & 0x1F);
			//printf("[RAW]: %x\n", reps);
			while (reps > 0)
			{
				b = read_byte(ROM, &readoff);
				write_byte(CompBuff, &writeoff, (u8)b);
				write_to_wnd(window, &wndoff, (u8)b);

				reps--;
			}
		}
		else if (b >= 0xC0 && b < 0xE0)
		{
			reps = (b & 0x1F) + 2;
			//printf("[RLE]: %x\n", reps);
			b = read_byte(ROM, &readoff);
			while (reps > 0)
			{
				write_byte(CompBuff, &writeoff, (u8)b);
				write_to_wnd(window, &wndoff, (u8)b);

				reps--;
			}
		}
		else
		{
			reps = (b & 0x1F) + 2;
			b = 0x00;
			//printf("[ZEROS]: %x\n", reps);
			while (reps > 0)
			{
				write_byte(CompBuff, &writeoff, (u8)b);
				write_to_wnd(window, &wndoff, (u8)b);

				reps--;
			}
		}

		while (reps > 0)
		{
			b = read_wnd_byte(window, &from);

			write_byte(CompBuff, &writeoff, (u8)b);
			write_to_wnd(window, &wndoff, (u8)b);

			reps--;
		}
	}

	free(window);
	free(reserve);
	*Size = writeoff;
	return CompBuff;
}
