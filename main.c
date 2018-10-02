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

CHAR_INFO buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

int main()
{
	QueryPerformanceFrequency(&globalPerformanceFrequency);
	QueryPerformanceCounter(&currentClock);
    
    COORD dwBufferSize = {SCREEN_WIDTH, SCREEN_HEIGHT};
    COORD dwBufferCoord = {0, 0};
    SMALL_RECT rcRegion = {0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1};
	
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	
	CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOutput, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hOutput, &cursorInfo);
    
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode); 
    SetConsoleMode(hInput, prevMode & ~ENABLE_QUICK_EDIT_MODE);
	
	AsciiRectangle *ar = create_ascii_rectangle();
	ar->xPos = 8;
	ar->yPos = 10;
	ar->width = 5;
	ar->height = 4;
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
			} else if (code == 'z') {
                if (ar->yPos != 0)
                {
                    ar->yPos--;
                }
            } else if (code == 's') {
                if (ar->yPos + ar->height != SCREEN_HEIGHT)
                {
                    ar->yPos++;
                }
            } else if (code == 'q') {
                if (ar->xPos != 0)
                {
                    ar->xPos--;
                }
            } else if (code == 'd') {
                if (ar->xPos + ar->width != SCREEN_WIDTH)
                {
                    ar->xPos++;
                }
            }
        }
        
        for (unsigned int i = 0; i < SCREEN_HEIGHT; i++)
        {
            for (unsigned int j = 0; j < SCREEN_WIDTH; j++)
            {
                buffer[i][j].Char.AsciiChar = 0;
                buffer[i][j].Attributes = 0;
            }
        }
        
        draw_ascii_rectangle(ar);
        
        WriteConsoleOutput(hOutput, (CHAR_INFO *)buffer, dwBufferSize, dwBufferCoord, &rcRegion);
    }
    
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hOutput, &cursorInfo);
    
    SetConsoleMode(hInput, prevMode);
    
    return 0;
}