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
uint8_t t1;

int main(int argc, char* argv[])
{
	int i;

	Stack map;

	Wall wall0 = {200, 200, 100, 200, 0, RED, 1};
	Wall wall1 = {100, 200, 100, 100, 0, GREEN, 1};
	Wall wall2 = {100, 100, 200, 100, 0, BLUE, 1};
	Wall wall3 = {200, 100, 200, 200, 0, WHITE, 1};
	
	Coords player = {0, 0};
	Mouse minput;
	minput.angle = 0;

	VGA = (uint8_t *)malloc(64000);
	if (VGA==NULL)exit(1);

	oldisr09h = _dos_getvect(0x09);
	oldisr08h = _dos_getvect(0x08);

	_dos_setvect(0x08, tick);
	_dos_setvect(0x09, keyisr);

	tickrate(18644); /*64hz*/
	
	init_tables();
	set_mode(0x13);

	initstack(&map);

	push_render(&map, &wall0);
	push_render(&map, &wall1);
	push_render(&map, &wall2);
	push_render(&map, &wall3);

	while(!keystate[K_ESC]) {
		tickcount = 0;
		memset(VGA, 0, 64000);

		mousemov(&minput);
		playerinput(&player, minput.angle);
		maprender(&map, &player, &minput);

		while ((inp(0x3DA) & 0x08));
		while (!(inp(0x3DA) & 0x08));
                memcpy(VRAM,VGA,64000);
		t1 = tickcount;
	}

	free(VGA);
	set_mode(0x03);
	_dos_setvect(0x09, oldisr09h);
	_dos_setvect(0x08, oldisr08h);

	if(argc) {printf("%s\n", argv[1]);}

	return 0;
}
