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
    TileType tiles[SCREEN_HEIGHT][SCREEN_WIDTH];
	AsciiRectangle ars[SCREEN_HEIGHT][SCREEN_WIDTH];
	int playerXPos;
	int playerYPos;
	int *boxesXPoss;
	int *boxesYPoss;
} Map;

Map *create_map(char *filePath);