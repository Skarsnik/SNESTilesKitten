#include "Comp.h"

#define wndsize 0x2000
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
	*wndoff = 0x0;
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
	int size = 0, bit = 0, low = 0, high = 0, readoff = 0, writeoff = 0, wndoff = 0, reps = 0, from = 0;
	u8 bitscnt = 0, cmd = 0;
	ushort b = 0;
	u8 *window, *reserve;

	init_wnd(&window, &reserve, &wndoff);
	ROMOffset = ADR(Offset);
	readoff = 0;
	writeoff = 0;
	size = read_byte(ROM, &readoff) | ((read_byte(ROM, &readoff) << 8) & 0xFF00);
	printf("[SIZE]: %04x\n", size);
	CompBuff = new u8[0x10000];
	bitscnt = 1;

	while (readoff - 2 < size)
	{
		bit = read_cmd_bit(ROM, &readoff, &bitscnt, &cmd);

		if (bit)
		{
			b = read_byte(ROM, &readoff);
			write_byte(CompBuff, &writeoff, (u8)b);
			write_to_wnd(window, &wndoff, (u8)b);
			//printf("[RAW]\n");
		}
		else
		{
			low = read_byte(ROM, &readoff);
			high = read_byte(ROM, &readoff);
			reps = (low & 0xF) + 3;
			//printf("[LZ]: %x\n", reps);
			from = (((high << 8) | low) >> 4)&wndmask;
			int offset = (wndoff-from);
			while (reps > 0)
			{
				b = read_wnd_byte(window, &offset);

				write_byte(CompBuff, &writeoff, (u8)b);
				write_to_wnd(window, &wndoff, (u8)b);

				reps--;
			}
		}
	}

	// Transform from 2bpp composite to 2bpp planar
	TempBuffOdd = new u8[0x10000];
	TempBuffEven = new u8[0x10000];
	OutBuff = new u8[0x10000];

	int odd = 0;
	int even = 0;
	int out = 0;

	for (int x = 0; x < (writeoff / 8); x++)
	{
		if (x & 0x1)
		{
			for (int i = 0; i < 8; i++)
			{
				TempBuffOdd[odd++] = CompBuff[x * 8 + i];
			}
		}
		else
		{
			for (int i = 0; i < 8; i++)
			{
				TempBuffEven[even++] = CompBuff[x * 8 + i];
			}
		}
	}

	//free(CompBuff);

	for (int x = 0; x < writeoff / 2; x++)
	{
		OutBuff[out++] = TempBuffOdd[x];
		OutBuff[out++] = TempBuffEven[x];
	}

	free(TempBuffOdd);
	free(TempBuffEven);
	free(window);
	free(reserve);
	*Size = writeoff;
	return OutBuff;
}
