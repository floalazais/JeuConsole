#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#include "map.h"
#include "error.h"
#include "stretchy_buffer.h"
#include "keyboard.h"
#include "graphics.h"

Map *create_map(char *filePath)
{
	Map *map = malloc(sizeof (*map));
	FILE *file = fopen(filePath, "rb");
    if (!file)
    {
        error("could not open %s.", filePath);
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    if (fileSize == -1L)
    {
        error("could not get size of %s.", filePath);
    }
    fileSize++;  // For '\0'
    rewind(file);

    char *fileString = malloc(sizeof (*fileString) * fileSize);
    size_t result = fread(fileString, sizeof (char), fileSize - 1, file);
    if (result != (unsigned long)fileSize - 1)
    {
        error("could not read %s.", filePath);
    }
    fileString[fileSize - 1] = '\0';

	fclose(file);
	
	int currentChar = 0;
	
	if (isdigit(fileString[currentChar]))
	{
        char *begin = fileString + currentChar;
        char *end = begin;
        do {
            currentChar++;
            end++;
        } while (isdigit(fileString[currentChar]));
        map->width = (int)strtol(begin, &end, 10);
	}
	currentChar++;
	int cursorXPos = 0;
	int cursorYPos = 0;
	map->playerXPos = -1;
	map->playerYPos = -1;
	map->tiles = NULL;
	map->boxesXPoss = NULL;
	map->boxesYPoss = NULL;
	while (fileString[currentChar] != '\0')
	{
		if (cursorXPos == map->width + 1)
		{
			error("map is wider than specified width at line %d of %s.", cursorYPos + 1, filePath);
		}
		if (fileString[currentChar] == ' ')
		{
			buf_add(map->tiles, TILE_VOID);
		} else if (fileString[currentChar] == 'W') {
			buf_add(map->tiles, TILE_WALL);
		} else if (fileString[currentChar] == 'S') {
			buf_add(map->tiles, TILE_SENSOR);
		} else if (fileString[currentChar] == 'P') {
			if (map->playerXPos != -1)
			{
				error("cannot have 2 players on the same map in %s.", filePath);
			}
			buf_add(map->tiles, TILE_VOID);
			map->playerXPos = cursorXPos;
			map->playerYPos = cursorYPos;
		} else if (fileString[currentChar] == 'B') {
			buf_add(map->tiles, TILE_VOID);
			buf_add(map->boxesXPoss, cursorXPos);
			buf_add(map->boxesYPoss, cursorYPos);
		} else if (fileString[currentChar] == 'X') {
			if (map->playerXPos != -1)
			{
				error("cannot have 2 players on the same map in %s.", filePath);
			}
			buf_add(map->tiles, TILE_SENSOR);
			map->playerXPos = cursorXPos;
			map->playerYPos = cursorYPos;
		} else if (fileString[currentChar] == 'V') {
			buf_add(map->tiles, TILE_SENSOR);
			buf_add(map->boxesXPoss, cursorXPos);
			buf_add(map->boxesYPoss, cursorYPos);
		} else if (fileString[currentChar] == '\n') {
			if (cursorXPos < map->width)
			{
				error("map must be filled at line %d of %s", cursorYPos + 1, filePath);
			}
			cursorXPos = -1;
			cursorYPos++;
		}
		cursorXPos++;
		currentChar++;
	}
	map->height = cursorYPos;
	if (map->playerXPos == -1)
	{
		error("no player specified.");
	}
	free(fileString);
	return map;
}

bool update_map(Map *map)
{
	if (is_input_key_pressed(INPUT_KEY_Z))
	{
		if (map->playerYPos != 0)
		{
			TileType tileUp = map->tiles[map->playerXPos + (map->playerYPos - 1) * map->width];
			if (tileUp == TILE_VOID || tileUp == TILE_SENSOR)
			{
				int boxUp = -1;
				for (unsigned int i = 0; i < buf_len(map->boxesXPoss); i++)
				{
					if (map->boxesXPoss[i] == map->playerXPos && map->boxesYPoss[i] == map->playerYPos - 1)
					{
						boxUp = i;
						break;
					}
				}
				if (boxUp != -1)
				{
					if (map->playerYPos != 1)
					{
						TileType tileUp2 = map->tiles[map->playerXPos + (map->playerYPos - 2) * map->width];
						if (tileUp2 == TILE_VOID || tileUp2 == TILE_SENSOR)
						{
							int boxUp2 = -1;
							for (unsigned int i = 0; i < buf_len(map->boxesXPoss); i++)
							{
								if (map->boxesXPoss[i] == map->playerXPos && map->boxesYPoss[i] == map->playerYPos - 2)
								{
									boxUp2 = i;
									break;
								}
							}
							if (boxUp2 == -1)
							{
								map->playerYPos--;
								map->boxesYPoss[boxUp]--;
							}
						}
					}
				} else {
					map->playerYPos--;
				}
			}
		}
	} else if (is_input_key_pressed(INPUT_KEY_S)) {
		if (map->playerYPos != map->height)
		{
			TileType tileDown = map->tiles[map->playerXPos + (map->playerYPos + 1) * map->width];
			if (tileDown == TILE_VOID || tileDown == TILE_SENSOR)
			{
				int boxDown = -1;
				for (unsigned int i = 0; i < buf_len(map->boxesXPoss); i++)
				{
					if (map->boxesXPoss[i] == map->playerXPos && map->boxesYPoss[i] == map->playerYPos + 1)
					{
						boxDown = i;
						break;
					}
				}
				if (boxDown != -1)
				{
					if (map->playerYPos != map->height - 1)
					{
						TileType tileDown2 = map->tiles[map->playerXPos + (map->playerYPos + 2) * map->width];
						if (tileDown2 == TILE_VOID || tileDown2 == TILE_SENSOR)
						{
							int boxDown2 = -1;
							for (unsigned int i = 0; i < buf_len(map->boxesXPoss); i++)
							{
								if (map->boxesXPoss[i] == map->playerXPos && map->boxesYPoss[i] == map->playerYPos + 2)
								{
									boxDown2 = i;
									break;
								}
							}
							if (boxDown2 == -1)
							{
								map->playerYPos++;
								map->boxesYPoss[boxDown]++;
							}
						}
					}
				} else {
					map->playerYPos++;
				}
			}
		}
	} else if (is_input_key_pressed(INPUT_KEY_Q)) {
		if (map->playerXPos != 0)
		{
			TileType tileLeft = map->tiles[(map->playerXPos - 1) + map->playerYPos * map->width];
			if (tileLeft == TILE_VOID || tileLeft == TILE_SENSOR)
			{
				int boxLeft = -1;
				for (unsigned int i = 0; i < buf_len(map->boxesXPoss); i++)
				{
					if (map->boxesXPoss[i] == map->playerXPos - 1 && map->boxesYPoss[i] == map->playerYPos)
					{
						boxLeft = i;
						break;
					}
				}
				if (boxLeft != -1)
				{
					if (map->playerXPos != 1)
					{
						TileType tileLeft2 = map->tiles[(map->playerXPos - 2) + map->playerYPos * map->width];
						if (tileLeft2 == TILE_VOID || tileLeft2 == TILE_SENSOR)
						{
							int boxLeft2 = -1;
							for (unsigned int i = 0; i < buf_len(map->boxesXPoss); i++)
							{
								if (map->boxesXPoss[i] == map->playerXPos - 2 && map->boxesYPoss[i] == map->playerYPos)
								{
									boxLeft2 = i;
									break;
								}
							}
							if (boxLeft2 == -1)
							{
								map->playerXPos--;
								map->boxesXPoss[boxLeft]--;
							}
						}
					}
				} else {
					map->playerXPos--;
				}
			}
		}
	} else if (is_input_key_pressed(INPUT_KEY_D)) {
		if (map->playerXPos != map->width)
		{
			TileType tileRight = map->tiles[(map->playerXPos + 1) + map->playerYPos * map->width];
			if (tileRight == TILE_VOID || tileRight == TILE_SENSOR)
			{
				int boxRight = -1;
				for (unsigned int i = 0; i < buf_len(map->boxesXPoss); i++)
				{
					if (map->boxesXPoss[i] == map->playerXPos + 1 && map->boxesYPoss[i] == map->playerYPos)
					{
						boxRight = i;
						break;
					}
				}
				if (boxRight != -1)
				{
					if (map->playerXPos != map->width - 1)
					{
						TileType tileRight2 = map->tiles[(map->playerXPos + 2) + map->playerYPos * map->width];
						if (tileRight2 == TILE_VOID || tileRight2 == TILE_SENSOR)
						{
							int boxRight2 = -1;
							for (unsigned int i = 0; i < buf_len(map->boxesXPoss); i++)
							{
								if (map->boxesXPoss[i] == map->playerXPos + 2 && map->boxesYPoss[i] == map->playerYPos)
								{
									boxRight2 = i;
									break;
								}
							}
							if (boxRight2 == -1)
							{
								map->playerXPos++;
								map->boxesXPoss[boxRight]++;
							}
						}
					}
				} else {
					map->playerXPos++;
				}
			}
		}
	}
	for (unsigned int i = 0; i < buf_len(map->boxesXPoss); i++)
	{
		if (map->tiles[map->boxesXPoss[i] + map->boxesYPoss[i] * map->width] != TILE_SENSOR)
		{
			return false;
		}
	}
	return true;
}

void draw_map(Map *map)
{
	TileType tile;
	int xPos;
	int yPos;
	Color color;
	for (int i = 0; i < map->height; i++)
	{
		for (int j = 0; j < map->width; j++)
		{
			tile = map->tiles[i * map->width + j];
			xPos = j;
			yPos = i;
			if (tile == TILE_VOID)
			{
				color = COLOR_WHITE;
			} else if (tile == TILE_WALL) {
				color = COLOR_BLACK;
			} else if (tile == TILE_SENSOR) {
				color = COLOR_YELLOW;
			}
			draw_square(xPos, yPos, color);
		}
	}
	if (map->tiles[map->playerXPos + map->playerYPos * map->width] == TILE_SENSOR)
	{
		color = COLOR_CYAN;
	} else {
		color = COLOR_BLUE;
	}
	xPos = map->playerXPos;
	yPos = map->playerYPos;
	draw_square(xPos, yPos, color);
	for (unsigned int i = 0; i < buf_len(map->boxesXPoss); i++)
	{
		if (map->tiles[map->boxesXPoss[i] + map->boxesYPoss[i] * map->width] == TILE_SENSOR)
		{
			color = COLOR_GREEN;
		} else {
			color = COLOR_RED;
		}
		xPos = map->boxesXPoss[i];
		yPos = map->boxesYPoss[i];
		draw_square(xPos, yPos, color);
	}
}

void free_map(Map *map)
{
	buf_free(map->boxesXPoss);
	buf_free(map->boxesYPoss);
    buf_free(map->tiles);
	free(map);
}