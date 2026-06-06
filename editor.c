#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <conio.h>

#include "draw.h"
#include "i_defs.h"
#include "colors.h"

void mousesnap(int* x, int* y);
void selmode();
void linemode();
void editmode(int mode);
int editmap(char* name);

Mouse minput = {0, 0, 0};
Node newseg;

uint8_t oldbx;
Node *tree = NULL;
int node_n;

void mousesnap(int* x, int* y)
{
	int i, j;
	int dx, dy;

	if(!keystate[K_LCTRL]) {
		
		for(i=0; i<node_n; i++) {
		for(j=0; j<2; j++) {

				dx = abs(minput.x-tree[i].x[j]);
				dy = abs(minput.y-tree[i].y[j]);

				if(dx < 5 && dy < 5) {
					*x = tree[i].x[j];
					*y = tree[i].y[j];

					circle(*x, *y, 3, RED);
				}
			}
		}
	}
}

void selmode()
{
	int i, j;
	int dx, dy;
	int lsel_x, lsel_y;
	int lnode_x, lnode_y;

	if(minput.bx & 1) {

		newseg.x[1] = minput.x;
		newseg.y[1] = minput.y;

		rect(newseg.x[0], newseg.y[0], newseg.x[1], newseg.y[1], G5);
	} /*else if((!minput.bx & 1) && (oldbx & 1)) {

		dx = newseg.x[0] - newseg.x[1];
		dy = newseg.y[0] - newseg.y[1];

		lsel_x = (dx<0?0:1);
		lsel_y = (dy<0?0:1);

		for(i=0; i<node_n; i++) {

			lnode_x = (tree[i].x[1]<tree[i].x[0]?1:0);
			lnode_y = (tree[i].y[1]<tree[i].y[0]?1:0);

			if(newseg.x[lsel_x] < tree[i].x[lnode_x]
			&& newseg.y[lsel_y] < tree[i].y[lnode_y]
			&& newseg.x[!lsel_x] > tree[i].x[!lnode_x]
			&& newseg.y[!lsel_y] > tree[i].y[!lnode_y]) {
			}

			line(tree[i].x[0], tree[i].y[0], tree[i].x[1], tree[i].y[1], RED);
		}
	}*/ else {

		newseg.x[0] = minput.x;
		newseg.y[0] = minput.y;
	}


}

void linemode()
{
	int i, j;
	int dx, dy;

	if(minput.bx & 1) {

		newseg.x[1] = minput.x;
		newseg.y[1] = minput.y;
		mousesnap(&newseg.x[1], &newseg.y[1]);
		line(newseg.x[0], newseg.y[0], newseg.x[1], newseg.y[1], WHITE);

	} else if((!minput.bx & 1) && (oldbx & 1)) {

		dx = abs(newseg.x[1]-newseg.x[0]);
		dy = abs(newseg.y[1]-newseg.y[0]);

		if((dx+dy) > 2) {
			for(i=0; i<2; i++) {
				tree[node_n].x[i] = newseg.x[i];
				tree[node_n].y[i] = newseg.y[i];
			}
			node_n++;
		}
	} else {
		newseg.x[0] = minput.x;
		newseg.y[0] = minput.y;
		mousesnap(&newseg.x[0], &newseg.y[0]);
	}
}

void editmode(int mode)
{
		if(mode == 1) {
			cursor(&minput, cursor_1);
			selmode();
		}

		if(mode == 2) {
			cursor(&minput, crosshair);
			linemode();
		}
}

int editmap(char* name)
{
	int i, j;
	int mode = 1;
	int treesize;

	Node *tmptree;
	
	FILE *mapfile = fopen(name, "r+b");
	if(mapfile == NULL) mapfile = fopen(name, "wb");
	if(mapfile == NULL) return 1;

	treesize = 5;
	tree = calloc(treesize, sizeof(Node));
	if(tree == NULL){return 1;}

	oldbx = 0;
	node_n = 0;

	set_mode(0x13);
	while(!keystate[K_ESC]) {

		memset(VGA, 0, 64000);

		mouseinput(&minput, 0x03);
		mouserange(319, 199);

		if(node_n == treesize) {
			tmptree = realloc(tree, treesize<<=1 * sizeof(Node));
			if(tmptree == NULL) {
				free(tree);
				return 1;
			} else { tree = tmptree; }
		}

		if(keystate[K_1]) mode = 1;
		if(keystate[K_2]) mode = 2;

		editmode(mode);

		oldbx = minput.bx;

		for(i=0; i<node_n; i++) {
			line(tree[i].x[0], tree[i].y[0], tree[i].x[1], tree[i].y[1], GREEN);
		}

		while ((inp(0x3DA) & 0x08));
		while (!(inp(0x3DA) & 0x08));
	        memcpy(VRAM,VGA,64000);
	}
	set_mode(0x03);

	for(i=0; i<node_n; i++) {
		for(j=0; j<2; j++) {
			tree[i].x[j] = (tree[i].x[j]-159)<<2;
			tree[i].y[j] = (tree[i].y[j]-99)<<2;
		}
	}

	fwrite(&node_n, sizeof(int), 1, mapfile);

	if(fwrite(tree, sizeof(Node), node_n, mapfile) < node_n)
		return 2;

	fclose(mapfile);
	free(tree);
	return 0;
}

