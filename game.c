#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <conio.h>

#include "draw.h"
#include "i_defs.h"
#include "colors.h"

int openmap(char* name, int size)
{
	Wall tree[4];

	Coords player = {0, 0};
	Mouse minput = {0, 0, 0};

	FILE *map = fopen(name, "rb");
		if(map == NULL) return 1;

	fread(tree, sizeof(Wall), size, map);
	fclose(map);
	set_mode(0x13);

	while(!keystate[K_ESC]) {
		tickcount = 0;
		memset(VGA, 0, 64000);

		mouseinput(&minput, 0x0b);
		keyinput(&player, minput.x);

		mapshift(&tree[0], &player, &minput);
		mapshift(&tree[1], &player, &minput);	/*temporary solution*/
		mapshift(&tree[2], &player, &minput);
		mapshift(&tree[3], &player, &minput);

		while ((inp(0x3DA) & 0x08));
		while (!(inp(0x3DA) & 0x08));
	        memcpy(VRAM,VGA,64000);
		t1 = tickcount;
	}
	set_mode(0x03);
	return 0;
} 
