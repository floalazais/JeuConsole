typedef enum TileType
{
	TILE_VOID,
	TILE_WALL,
	TILE_SENSOR
} TileType;

typedef struct Map
{
	int width;
	int height;
	int playerXPos;
	int playerYPos;
	int *boxesXPoss;
	int *boxesYPoss;
    TileType *tiles;
} Map;

Map *create_map(char *filePath);
void update_map(Map *map);
void draw_map(Map *map);
void free_map(Map *map);