#include "globals.h"
#include "ascii_rectangle.h"

AsciiRectangle *create_ascii_rectangle()
{
	AsciiRectangle *asciiRectangle = malloc(sizeof (*asciiRectangle));
	asciiRectangle->cPos = 0;
	asciiRectangle->lPos = 0;
	asciiRectangle->width = 0;
	asciiRectangle->height = 0;
	asciiRectangle->character = 0;
	asciiRectangle->color = 0;
	return asciiRectangle;
}

void draw_ascii_rectangle(AsciiRectangle *asciiRectangle)
{
	for (int x = asciiRectangle->width; x > 0; x--)
	{
		for (int y = asciiRectangle->height; y > 0; y--)
		{
			buffer[asciiRectangle->lPos+x][asciiRectangle->cPos+y].Char.UnicodeChar = asciiRectangle->character;
			buffer[asciiRectangle->lPos+x][asciiRectangle->cPos+y].Attributes = asciiRectangle->color;
		}	
	}
}