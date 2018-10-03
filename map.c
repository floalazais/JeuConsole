#include "map.h"
#include "error.h"
#include "stretchy_buffer.h"
#include "globals.h"
#include "ascii_rectangle.h"
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

void update_map(Map *map)
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
	if (map->tiles[map->playerXPos + map->playerYPos * map->width] == TILE_SENSOR)
	{
		ar->color = COLOR_MAGENTA;
	} else {
		ar->color = COLOR_BLUE;
	}
	ar->xPos = map->playerXPos * 2;
	ar->yPos = map->playerYPos * 1;
	draw_ascii_rectangle(ar);
	for (unsigned int i = 0; i < buf_len(map->boxesXPoss); i++)
	{
		if (map->tiles[map->boxesXPoss[i] + map->boxesYPoss[i] * map->width] == TILE_SENSOR)
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

void free_map(Map *map)
{
	buf_free(map->boxesXPoss);
	buf_free(map->boxesYPoss);
    buf_free(map->tiles);
	free(map);
}