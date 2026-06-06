#include <dos.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "draw.h"
#include "i_defs.h"
#include "colors.h"

int editmap(char* name, int size)
{
	Wall tree[4];
	Mouse minput = {0, 0, 0};
	
	Coords sel;

	FILE *map = fopen(name, "r+b");
		if(map == NULL) return 1;
	if(map == NULL) map = fopen(name, "wb");
	if(map == NULL) return 1;

	set_mode(0x13);
	while(!keystate[K_ESC]) {
		memset(VGA, 0, 64000);

		mouseinput(&minput, 0x03);
		mouserange(319, 199);
		cursor(&minput, cursor1);

		if(minput.bx & 1) {
			rect(sel.x, sel.y, minput.x, minput.y, G5);
		} else {
			sel.x = minput.x;
			sel.y = minput.y;
		}
/*
		if(keystate[K_1]) selectmode();
		if(keystate[K_2]) polygonmode();
		if(keystate[K_3]) rectmode();
		if(keystate[K_4]) linemode();
*/
		while ((inp(0x3DA) & 0x08));
		while (!(inp(0x3DA) & 0x08));
	        memcpy(VRAM,VGA,64000);
	}
	set_mode(0x03);
	return 0;

/*
	if(fwrite(tree, sizeof(Wall), size, map) < size)
		return 2;
*/

	fread(tree, sizeof(Wall), size, map);
	fclose(map);
	return 0;
}

