#include "globals.h"
#include "ascii_rectangle.h"

typedef enum TileType
{
	TILE_VOID,
	TILE_WALL,
	TILE_PLAYER,
	TILE_BLOCK,
	TILE_PLATE
} TileType;

typedef struct Map
{
    TileType tiles[SCREEN_HEIGHT][SCREEN_WIDTH];
	AsciiRectangle ars[SCREEN_HEIGHT][SCREEN_WIDTH];
	int playerXPos;
	int playerYPos;
} Map;

Map *create_map();
void fill_map_from_file(char *filePath);