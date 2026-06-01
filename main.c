#include <dos.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <conio.h>

#include "draw.h"
#include "i_defs.h"
#include "g_defs.h"

extern void __interrupt keyisr(void);
void (__interrupt *oldisr)(void);

uint16_t *clock=(uint16_t*)0x046C;

int main(void)
{
	int i;
	Stack map;
	Wall walls = {50, 50, 25, 75, RED, 1};

	Coords player = {0, 0};
	Mouse minput;

	VGA = (uint8_t *)malloc(64000);
	if (VGA==NULL)exit(1);

	oldisr = _dos_getvect(0x09);
	_dos_setvect(0x09, keyisr);
	
	init_tables();
	set_mode(0x13);

	mouseinit();
	mousemov(&minput);

	initstack(&map);

	while(!keystate[K_ESC]) {
		memset(VGA, 0, 64000);

		pixel(159, 99, LIGHTGREEN);

		playerinput(&player, &minput);
		push_render(&map, &walls);
		maprender(&map, &player, &minput);

                while ((inp(0x3DA) & 0x08));
                while (!(inp(0x3DA) & 0x08));
                memcpy(VRAM,VGA,64000);
	}

	free(VGA);
	set_mode(0x03);
	_dos_setvect(0x09, oldisr);

	return 0;
}
