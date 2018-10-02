#include "map.h"
#include "error.h"
#include "xalloc.h"
#include <stdio.h>

char *file_to_string(char *filePath)
{
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

    char *fileString = xmalloc(sizeof (*fileString) * fileSize);
    size_t result = fread(fileString, sizeof (char), fileSize - 1, file);
    if (result != (unsigned long)fileSize - 1)
    {
        error("could not read %s.", filePath);
    }
    fileString[fileSize - 1] = '\0';

	fclose(file);

	return fileString;
}

Map *create_map()
{
	Map *map = malloc(sizeof (*map));
	map->playerXPos = 0;
	map->playerYPos = 0;
	return map;
}

void fill_map_from_file(char *filePath)
{
	;
}