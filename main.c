#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <conio.h>

#include "draw.h"
#include "i_defs.h"
#include "colors.h"

void __interrupt keyisr(void);
void (__interrupt *oldisr09h)(void);
void (__interrupt *oldisr08h)(void);

void __interrupt tick(void);
uint16_t *clock=(uint16_t*)0x046C;
uint8_t t1;

int main(int argc, char* argv[])
{
	int i;
	int exit = 0;

	oldisr09h = _dos_getvect(0x09);
	oldisr08h = _dos_getvect(0x08);
	_dos_setvect(0x08, tick);
	_dos_setvect(0x09, keyisr);

	tickrate(18644); /*64hz*/
	
	VGA = (uint8_t *)malloc(64000);
	if(VGA==NULL) {
		printf("Could not allocate video memory.\n");
		exit = 1;
	}

	init_tables();

	if(argc == 2) {
		if(openmap(argv[1], 4)) {
			printf("Could not find map \'%s\'\n", argv[1]);
			exit = 1;
		}
	} else if(argc == 3 && !strcmp(argv[1],"-e")) {
		if(editmap(argv[2], 4)) {
			printf("Could not edit map \'%s\'\n", argv[2]);
			exit = 1;
		}
	} else {printf("Invalid option."); exit = 1;}

	free(VGA);
	_dos_setvect(0x09, oldisr09h);
	_dos_setvect(0x08, oldisr08h);

	return exit;
}
