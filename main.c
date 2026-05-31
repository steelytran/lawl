#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "draw.h"
#include "defs.h"
#include "colors.h"

extern volatile byte lastkey;
extern void __interrupt keyisr(void);
void (__interrupt *oldisr)(void);

word *clock=(word*)0x046C;

int main(void)
{
	int i;
	Mouse cursor = {159, 99};

	VGA = (byte *)malloc(64000);
	if (VGA==NULL)exit(1);

	oldisr = _dos_getvect(0x09);
	_dos_setvect(0x09, keyisr);
	
	init_tables();
	set_mode(0x13);

	mouseinit();

	while(lastkey != K_ESC) {
		memset(VGA, 0, 64000);

		mousemov(&cursor);

		mpointer(&cursor, pointer);

                while ((inp(0x3DA) & 0x08));
                while (!(inp(0x3DA) & 0x08));
                memcpy(VRAM,VGA,64000);
	}

	free(VGA);
	set_mode(0x03);
	_dos_setvect(0x09, oldisr);

	return 0;
}
