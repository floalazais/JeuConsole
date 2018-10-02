#include "map.h"
#include "error.h"
#include "stretchy_buffer.h"
#include <stdio.h>

Map *create_map(char *filePath)
{
	Map *map = malloc(sizeof (*map));
	map->playerXPos = -1;
	map->playerYPos = -1;
	FILE *file = fopen(filePath, "rb");
    if (!file)
    {
        error("could not open %s.", filePath);
    }
	int lineLength = 0;
	while (getc(file) != '\n')
	{
		lineLength++;
	}
	map->xPos = (120 - lineLength) / 2;
	map->yPos = 0;
	map->boxesXPoss = NULL;
	map->boxesYPoss = NULL;
	int cursorXPos = map->xPos;
	int cursorYPos = 0;
	rewind(file);
	char c;
	while ((c = getc(file)) != '\0')
	{
		if (c == ' ')
		{
			map->tiles[cursorXPos][cursorYPos] = TILE_VOID;
		} else if (c == 'W') {
			map->tiles[cursorXPos][cursorYPos] = TILE_WALL;
		} else if (c == 'S') {
			map->tiles[cursorXPos][cursorYPos] = TILE_SENSOR;
		} else if (c == 'P') {
			if (map->playerXPos != -1)
			{
				error("cannot have 2 players on the same map.");
			}
			map->playerXPos = cursorXPos;
			map->playerYPos = cursorYPos;
		} else if (c == 'B') {
			buf_add(map->boxesXPoss, cursorXPos);
			buf_add(map->boxesYPoss, cursorYPos);
		} else if (c == '\n') {
			cursorXPos = map->xPos - 1;
			cursorYPos++;
		}
		cursorXPos++;
	}
	return map;
}