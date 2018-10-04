typedef enum Color
{
	COLOR_BLACK,
	COLOR_BLUE,
	COLOR_RED,
	COLOR_GREEN,
	COLOR_MAGENTA,
	COLOR_CYAN,
	COLOR_YELLOW,
	COLOR_WHITE
} Color;

void draw_square(int xPos, int yPos, Color color);
void draw_text(char *string, int xPos, int yPos, Color color);