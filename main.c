#include <dos.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <conio.h>

#include "draw.h"
#include "i_defs.h"
#include "colors.h"

#define GAMEMODE 1
#define EDITMODE 0

void __interrupt keyisr(void);
void (__interrupt *oldisr09h)(void);
void (__interrupt *oldisr08h)(void);

void __interrupt tick(void);
uint16_t *clock=(uint16_t*)0x046C;
uint8_t t1;

int main(int argc, char* argv[])
{
	int i;
	Wall bsptree[4];

	Coords player = {0, 0};
	Mouse minput = {0, 0, 0, 0};

	if(argc == 2) {
		if(openmap(argv[1], &bsptree, 4)) {
			printf("Could not find map \'%s\'\n", argv[1]);
			return 1;
		}
	} else if(argc == 3 && !strcmp(argv[1],"-e")) {
		if(editmap(argv[2], &bsptree, 4)) {
			printf("Could not edit map \'%s\'\n", argv[2]);
			return 1;
		}
	} else {printf("Invalid option."); return 1;}

	oldisr09h = _dos_getvect(0x09);
	oldisr08h = _dos_getvect(0x08);
	_dos_setvect(0x08, tick);
	_dos_setvect(0x09, keyisr);

	tickrate(18644); /*64hz*/
	
	VGA = (uint8_t *)malloc(64000);
	if(VGA==NULL) {
		printf("Could not allocate video memory.\n");
		exit(1);
	}

	set_mode(0x13);
	init_tables();

	while(!keystate[K_ESC]) {
		tickcount = 0;
		memset(VGA, 0, 64000);

		mouseinput(&minput);
		keyinput(&player, minput.angle);

		mapshift(&bsptree[0], &player, &minput);
		mapshift(&bsptree[1], &player, &minput);
		mapshift(&bsptree[2], &player, &minput);
		mapshift(&bsptree[3], &player, &minput);

		while ((inp(0x3DA) & 0x08));
		while (!(inp(0x3DA) & 0x08));
	        memcpy(VRAM,VGA,64000);
		t1 = tickcount;
	}

	free(VGA);
	set_mode(0x03);
	_dos_setvect(0x09, oldisr09h);
	_dos_setvect(0x08, oldisr08h);

	return 0;
}
