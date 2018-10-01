#include <stdio.h>
#include <conio.h>
#include <stdbool.h>

#include "globals.h"
#include "ascii_rectangle.h"

static bool running = true;

static LARGE_INTEGER globalPerformanceFrequency;
static LARGE_INTEGER currentClock;
static LARGE_INTEGER newClock;
float deltaTime = 0.0f;

static void update_delta_time()
{
	QueryPerformanceCounter(&newClock);
	deltaTime = (float)(newClock.QuadPart - currentClock.QuadPart) / (float)globalPerformanceFrequency.QuadPart;
	currentClock = newClock;
}

static HANDLE hOutput;

CHAR_INFO buffer[SCREEN_WIDTH][SCREEN_HEIGHT];
COORD dwBufferSize = {SCREEN_WIDTH, SCREEN_HEIGHT};
COORD dwBufferCoord = {0, 0};
SMALL_RECT rcRegion = {0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1};

int main()
{
	QueryPerformanceFrequency(&globalPerformanceFrequency);
	QueryPerformanceCounter(&currentClock);
	
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	
	CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOutput, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hOutput, &cursorInfo);
	
	AsciiRectangle *ar = create_ascii_rectangle();
	ar->cPos = 8;
	ar->lPos = 10;
	ar->width = 5;
	ar->height = 6;
	ar->color = 0x0E;
	ar->character = 'q';
	
	
	
	
	while(running)
	{
		update_delta_time();
		
		while (_kbhit())
		{
			int code = _getch();
			if (code == 27)
			{
				running = false;
			}
		}

		for (unsigned int i = 0; i < SCREEN_WIDTH; i++)
		{
			for (unsigned int j = 0; j < SCREEN_HEIGHT; j++)
			{
				buffer[i][j].Char.AsciiChar = 0;
				buffer[i][j].Attributes = 0;
			}
		}
		
		buffer[5][10].Char.UnicodeChar = 'H';
		buffer[5][10].Attributes = 0x0E;
		buffer[3][10].Char.UnicodeChar = 'B';
		buffer[3][10].Attributes = 0x0E;
		buffer[4][10].Char.UnicodeChar = 'M';
		buffer[4][10].Attributes = 0x0E;
		
		//draw_ascii_rectangle(ar);
			
		WriteConsoleOutput(hOutput, (CHAR_INFO *)buffer, dwBufferSize, dwBufferCoord, &rcRegion);
	}
	
	cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hOutput, &cursorInfo);
	
	return 0;
}