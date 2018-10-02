#include "globals.h"
#include "ascii_rectangle.h"

typedef enum TileType
{
	TILE_VOID,
	TILE_WALL,
	TILE_SENSOR
} TileType;

typedef struct Map
{
	int xPos;
	int yPos;
    TileType tiles[SCREEN_WIDTH][SCREEN_HEIGHT];
	AsciiRectangle ars[SCREEN_WIDTH][SCREEN_HEIGHT];
	int playerXPos;
	int playerYPos;
	int *boxesXPoss;
	int *boxesYPoss;
} Map;

Map *create_map(char *filePath);
void draw_map(Map *map);