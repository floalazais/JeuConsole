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

typedef struct AsciiRectangle
{
	int xPos;
	int yPos;
	int width;
	int height;
	Color color;
} AsciiRectangle;

AsciiRectangle *create_ascii_rectangle();
void draw_ascii_rectangle(AsciiRectangle *ar);