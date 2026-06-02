#include <dos.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <conio.h>

#include "draw.h"
#include "i_defs.h"
#include "g_defs.h"

extern void __interrupt keyisr(void);
void (__interrupt *oldisr09h)(void);
void (__interrupt *oldisr08h)(void);

uint16_t *clock=(uint16_t*)0x046C;
extern void __interrupt tick(void);

int main(void)
{
	int i;
	Stack map;
	Wall wall0 = {50, 50, -50, 50, RED, 1};
	Wall wall1 = {-50, 50, -50, -50, GREEN, 1};
	Wall wall2 = {-50, -50, 50, -50, BLUE, 1};
	Wall wall3 = {50, -50, 50, 50, WHITE, 1};
	
	Coords player = {0, 0};
	Mouse minput;
	minput.angle = 0;

	VGA = (uint8_t *)malloc(64000);
	if (VGA==NULL)exit(1);

	oldisr09h = _dos_getvect(0x09);
	oldisr08h = _dos_getvect(0x08);

	_dos_setvect(0x08, tick);
	_dos_setvect(0x09, keyisr);

	tickrate(9322);
	
	init_tables();
	set_mode(0x13);

	initstack(&map);

	while(!keystate[K_ESC]) {
		memset(VGA, 0, 64000);
		memset(VGA, LIGHTBLUE, 32000);

		mousemov(&minput);
		playerinput(&player, minput.angle);

		push_render(&map, &wall0);
		push_render(&map, &wall1);
		push_render(&map, &wall2);
		push_render(&map, &wall3);

		maprender(&map, &player, &minput);

		while ((inp(0x3DA) & 0x08));
		while (!(inp(0x3DA) & 0x08));
                memcpy(VRAM,VGA,64000);
	}

	free(VGA);
	set_mode(0x03);
	_dos_setvect(0x09, oldisr09h);
	_dos_setvect(0x08, oldisr08h);

	return 0;
}
