#include <Windows.h>

#include "buffer.h"
#include "error.h"
#include "graphics.h"

static WORD color_to_word(Color color)
{
	switch (color)
	{
		case COLOR_BLACK:
		return 0;
		case COLOR_BLUE:
		return BACKGROUND_BLUE + BACKGROUND_INTENSITY;
		case COLOR_RED:
		return BACKGROUND_RED + BACKGROUND_INTENSITY;
		case COLOR_GREEN:
		return BACKGROUND_GREEN + BACKGROUND_INTENSITY;
		case COLOR_MAGENTA:
		return BACKGROUND_BLUE + BACKGROUND_RED + BACKGROUND_INTENSITY;
		case COLOR_CYAN:
		return BACKGROUND_BLUE + BACKGROUND_GREEN + BACKGROUND_INTENSITY;
		case COLOR_YELLOW:
		return BACKGROUND_RED + BACKGROUND_GREEN + BACKGROUND_INTENSITY;
		case COLOR_WHITE:
		return BACKGROUND_BLUE + BACKGROUND_RED + BACKGROUND_GREEN + BACKGROUND_INTENSITY;
		default:
		error("unsupported color %d", color);
	}
}

void draw_square(int xPos, int yPos, Color color)
{
	buffer[yPos][xPos * 2].Attributes = color_to_word(color);
	buffer[yPos][xPos * 2 + 1].Attributes = color_to_word(color);
}

void draw_text(char *string, int xPos, int yPos, Color color)
{
	if (!string)
	{
		return;
	}
	for (unsigned int i = 0; i < strlen(string); i++)
	{
		buffer[yPos][xPos * 2 + i].Char.AsciiChar = string[i];
		buffer[yPos][xPos * 2 + i].Attributes = color_to_word(color);
	}
}