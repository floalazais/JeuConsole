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
	map->width = 0;
	while (getc(file) != '\n')
	{
		map->width++;
	}
	map->boxesXPoss = NULL;
	map->boxesYPoss = NULL;
	int cursorXPos = 0;
	int cursorYPos = 0;
	rewind(file);
	map->tiles = NULL;
	char c = getc(file);
	while (c != EOF)
	{
		if (c == ' ')
		{
			buf_add(map->tiles, TILE_VOID);
		} else if (c == 'W') {
			buf_add(map->tiles, TILE_WALL);
		} else if (c == 'S') {
			buf_add(map->tiles, TILE_SENSOR);
		} else if (c == 'P') {
			if (map->playerXPos != -1)
			{
				error("cannot have 2 players on the same map.");
			}
			buf_add(map->tiles, TILE_VOID);
			map->playerXPos = cursorXPos;
			map->playerYPos = cursorYPos;
		} else if (c == 'B') {
			buf_add(map->tiles, TILE_VOID);
			buf_add(map->boxesXPoss, cursorXPos);
			buf_add(map->boxesYPoss, cursorYPos);
		} else if (c == 'X') {
			if (map->playerXPos != -1)
			{
				error("cannot have 2 players on the same map.");
			}
			buf_add(map->tiles, TILE_SENSOR);
			map->playerXPos = cursorXPos;
			map->playerYPos = cursorYPos;
		} else if (c == 'V') {
			buf_add(map->tiles, TILE_SENSOR);
			buf_add(map->boxesXPoss, cursorXPos);
			buf_add(map->boxesYPoss, cursorYPos);
		} else if (c == '\n') {
			cursorXPos = -1;
			cursorYPos++;
		}
		cursorXPos++;
		c = getc(file);
	}
	map->height = cursorYPos;
	if (map->playerXPos == -1)
	{
		error("no player specified.");
	}
	fclose(file);
	return map;
}

void draw_map(Map *map)
{
	AsciiRectangle *ar = create_ascii_rectangle();
	ar->width = 2;
	ar->height = 1;
	for (int i = 0; i < map->height; i++)
	{
		for (int j = 0; j < map->width; j++)
		{
			TileType tileType = map->tiles[i * map->width + j];
			ar->xPos = j * 2;
			ar->yPos = i * 1;
			if (tileType == TILE_VOID)
			{
				ar->color = COLOR_WHITE;
			} else if (tileType == TILE_WALL) {
				ar->color = COLOR_BLACK;
			} else if (tileType == TILE_SENSOR) {
				ar->color = COLOR_YELLOW;
			}
			draw_ascii_rectangle(ar);
		}
	}
	ar->color = COLOR_BLUE;
	ar->xPos = map->playerXPos * 2;
	ar->yPos = map->playerYPos * 1;
	draw_ascii_rectangle(ar);
	for (unsigned int i = 0; i < buf_len(map->boxesXPoss); i++)
	{
		if (map->tiles[map->boxesXPoss[i] * map->width + map->boxesYPoss[i]] == TILE_SENSOR)
		{
			ar->color = COLOR_GREEN;
		} else {
			ar->color = COLOR_RED;
		}
		ar->xPos = map->boxesXPoss[i] * 2;
		ar->yPos = map->boxesYPoss[i] * 1;
		draw_ascii_rectangle(ar);
	}
	free(ar);
}