#include "globals.h"
#include "ascii_rectangle.h"
#include "error.h"

AsciiRectangle *create_ascii_rectangle()
{
	AsciiRectangle *ar = malloc(sizeof (*ar));
	ar->xPos = 0;
	ar->yPos = 0;
	ar->width = 0;
	ar->height = 0;
	ar->color = COLOR_BLACK;
	return ar;
}

void draw_ascii_rectangle(AsciiRectangle *ar)
{
	for (int i = 0; i < ar->height; i++)
	{
		for (int j = 0; j < ar->width; j++)
		{
			switch (ar->color)
			{
				case COLOR_BLACK:
                buffer[ar->yPos+i][ar->xPos+j].Attributes = 0;
                break;
				case COLOR_BLUE:
                buffer[ar->yPos+i][ar->xPos+j].Attributes = BACKGROUND_BLUE + BACKGROUND_INTENSITY;
                break;
				case COLOR_RED:
                buffer[ar->yPos+i][ar->xPos+j].Attributes = BACKGROUND_RED + BACKGROUND_INTENSITY;
                break;
				case COLOR_GREEN:
                buffer[ar->yPos+i][ar->xPos+j].Attributes = BACKGROUND_GREEN + BACKGROUND_INTENSITY;
                break;
				case COLOR_MAGENTA:
                buffer[ar->yPos+i][ar->xPos+j].Attributes = BACKGROUND_BLUE + BACKGROUND_RED + BACKGROUND_INTENSITY;
                break;
				case COLOR_CYAN:
                buffer[ar->yPos+i][ar->xPos+j].Attributes = BACKGROUND_BLUE + BACKGROUND_GREEN + BACKGROUND_INTENSITY;
                break;
				case COLOR_YELLOW:
                buffer[ar->yPos+i][ar->xPos+j].Attributes = BACKGROUND_RED + BACKGROUND_GREEN + BACKGROUND_INTENSITY;
                break;
				case COLOR_WHITE:
                buffer[ar->yPos+i][ar->xPos+j].Attributes = BACKGROUND_BLUE + BACKGROUND_RED + BACKGROUND_GREEN + BACKGROUND_INTENSITY;
                break;
				default:
                error("unsupported color %d", ar->color);
			}
		}
	}
}