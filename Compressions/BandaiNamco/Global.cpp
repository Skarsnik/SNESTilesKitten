#include "Global.h"

u16 BS16( u16 s )
{
	return ((s&0xFF)<<8) | ((s&0xFF00)>>8);
}
