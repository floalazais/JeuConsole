typedef struct AsciiRectangle
{
	int cPos;
	int lPos;
	int width;
	int height;
	int character;
	int color;
} AsciiRectangle;

AsciiRectangle *create_ascii_rectangle();
void draw_ascii_rectangle(AsciiRectangle *asciiRectangle);