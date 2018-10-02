#include "globals.h"
#include "ascii_rectangle.h"

AsciiRectangle *create_ascii_rectangle()
{
	AsciiRectangle *ar = malloc(sizeof (*ar));
	ar->xPos = 0;
	ar->yPos = 0;
	ar->width = 0;
	ar->height = 0;
	ar->character = 0;
	ar->color = 0;
	return ar;
}

void draw_ascii_rectangle(AsciiRectangle *ar)
{
	for (int i = 0; i < ar->height; i++)
	{
		for (int j = 0; j < ar->width; j++)
		{
			buffer[ar->yPos+i][ar->xPos+j].Char.UnicodeChar = ar->character;
			buffer[ar->yPos+i][ar->xPos+j].Attributes = ar->color;
		}
	}
}