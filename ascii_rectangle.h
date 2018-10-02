typedef struct AsciiRectangle
{
	int xPos;
	int yPos;
	int width;
	int height;
	int character;
	int color;
} AsciiRectangle;

AsciiRectangle *create_ascii_rectangle();
void draw_ascii_rectangle(AsciiRectangle *ar);