#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <conio.h>

#include "draw.h"
#include "i_defs.h"
#include "colors.h"

int openmap(char* name)
{
	int i;
	int node_n;
	Node *tree = NULL;

	Coords player = {0, 0};
	Mouse minput = {0, 0, 0};

	FILE *mapfile = fopen(name, "rb");
		if(mapfile == NULL) return 1;

	fread(&node_n, sizeof(int), 1, mapfile);

	tree = calloc(node_n, sizeof(Node));

	fread(tree, sizeof(Node), node_n, mapfile);
	fclose(mapfile);
	set_mode(0x13);

	while(!keystate[K_ESC]) {
		tickcount = 0;
		memset(VGA, 0, 64000);

		mouseinput(&minput, 0x0b);
		keyinput(&player, minput.x);

		for(i=0; i<node_n; i++)
			mapshift(&tree[i], &player, &minput);

		while ((inp(0x3DA) & 0x08));
		while (!(inp(0x3DA) & 0x08));
	        memcpy(VRAM,VGA,64000);
		t1 = tickcount;
	}
	set_mode(0x03);
	free(tree);
	return 0;
} 
