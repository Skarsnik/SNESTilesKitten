#pragma once

#include "Global.h"

class Comp
{
	private:
		u32 ROMOffset;
		static u8 *ROM;
		u8 *CompBuff;

		int 	read_cmd_bit(u8 *input, int *readoff, u8 *bitscnt, u8 *cmd);
		u8		read_byte(u8 *input, int *readoff);
		u8		read_wnd_byte (u8 *window, int *wndoff);
		ushort  read_word(u8 *input, int *readoff);

		static void init_wnd(u8 **window, u8 **reserve, int *wndoff);
		static void find_matches(u8 *input, int readoff, int size, int wndoff, u8 *window, u8 *reserve, int *reps, int *from, int min_pos, int max_pos);
		static void write_byte(u8 *output, int *writeoff, u8 b);
		static void write_to_wnd(u8 *window, int *wndoff, u8 b);
		static void write_word(u8 *output, int *writeoff, ushort w);
		static void write_cmd_bit(int bit, u8 *output, int *writeoff, u8 *bitscnt, int *cmdoff);

	public:

		static void Init( u8 *ROM );
		u8 *UncompressData( u32 Offset, u32 *Size );
};

