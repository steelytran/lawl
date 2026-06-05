#include <dos.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "draw.h"
#include "i_defs.h"
#include "colors.h"

int openmap(char* name, Wall *tree, int size)
{
	FILE *map = fopen(name, "rb");
		if(map == NULL) return 1;
	fread(tree, sizeof(Wall), size, map);
	fclose(map);
	return 0;
} 


int editmap(char* name, Wall *tree, int size)
{
	FILE *map = fopen(name, "r+b");
		if(map == NULL) return 1;
	if(map == NULL) map = fopen(name, "wb");
	if(map == NULL) return 1;

/*
	if(fwrite(tree, sizeof(Wall), size, map) < size)
		return 2;
*/

	fread(tree, sizeof(Wall), size, map);
	fclose(map);
	return 0;
}
