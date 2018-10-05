#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <Windows.h>

#include "error.h"
#include "map.h"
#include "buffer.h"
#include "keyboard.h"
#include "graphics.h"

static LARGE_INTEGER globalPerformanceFrequency;
static LARGE_INTEGER currentClock;
static LARGE_INTEGER newClock;
static float deltaTime = 0.0f;

static void update_delta_time()
{
	QueryPerformanceCounter(&newClock);
	deltaTime = (float)(newClock.QuadPart - currentClock.QuadPart) / (float)globalPerformanceFrequency.QuadPart;
	currentClock = newClock;
}

static void update_input_keys()
{
	memcpy(inputKeysBefore, inputKeysNow, sizeof (inputKeysBefore));
}

static bool compare_rectangle(SMALL_RECT *a, SMALL_RECT *b)
{
	return a->Top == b->Top && a->Left == b->Left && a->Right == b->Right && a->Bottom == b->Bottom;
}

static HWND consoleWindow;
static LONG oldWindowStyle;

static HANDLE hOutput;
static CONSOLE_CURSOR_INFO cursorInfo;

static HANDLE hInput;
static DWORD prevMode;

static CHAR_INFO oldBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
static COORD dwBufferSize = {SCREEN_WIDTH, SCREEN_HEIGHT};
static COORD dwBufferCoord = {0, 0};
static SMALL_RECT rcRegion = {0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1};

void restore_console()
{
	WriteConsoleOutput(hOutput, (CHAR_INFO *)oldBuffer, dwBufferSize, dwBufferCoord, &rcRegion);
    
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hOutput, &cursorInfo);
    
    SetConsoleMode(hInput, prevMode);
	
	SetWindowLong(consoleWindow, GWL_STYLE, oldWindowStyle);
}

__attribute__((noreturn)) void error(char *format, ...)
{
	restore_console();
	char buffer[1024] = "ERROR : ";
	va_list arguments;
	va_start(arguments, format);
    vsprintf(buffer + 8, format, arguments);
	va_end(arguments);
	strcat(buffer, "\n");
	puts(buffer);
	exit(EXIT_FAILURE);
}

CHAR_INFO buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

int main()
{   
	consoleWindow = GetConsoleWindow();
	oldWindowStyle = GetWindowLong(consoleWindow, GWL_STYLE);
	SetWindowLong(consoleWindow, GWL_STYLE, oldWindowStyle & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
	
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	
	ReadConsoleOutput(hOutput, (CHAR_INFO *)oldBuffer, dwBufferSize, dwBufferCoord, &rcRegion);
	
    GetConsoleCursorInfo(hOutput, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hOutput, &cursorInfo);
    
    hInput = GetStdHandle(STD_INPUT_HANDLE);
    
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, prevMode & ~ENABLE_QUICK_EDIT_MODE & ~ENABLE_MOUSE_INPUT & ~ENABLE_PROCESSED_INPUT);
	
	CONSOLE_SCREEN_BUFFER_INFO CSBInfo;
	
	int nbFrame = 0;
	bool pressed;
	bool won = false;
	
	Map *map = create_map("map.map");
	
	QueryPerformanceFrequency(&globalPerformanceFrequency);
	QueryPerformanceCounter(&currentClock);
	
	while(true)
	{
		update_delta_time();
		update_input_keys();
		
		GetConsoleScreenBufferInfo(hOutput, &CSBInfo);
		if (!compare_rectangle(&CSBInfo.srWindow, &rcRegion))
		{
			SetConsoleWindowInfo(hOutput, true, &rcRegion);
		}
		
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			break;
		}
		
		inputKeysNow[INPUT_KEY_Z] = GetAsyncKeyState('Z');
		inputKeysNow[INPUT_KEY_S] = GetAsyncKeyState('S');
		inputKeysNow[INPUT_KEY_Q] = GetAsyncKeyState('Q');
		inputKeysNow[INPUT_KEY_D] = GetAsyncKeyState('D');
		inputKeysNow[INPUT_KEY_R] = GetAsyncKeyState('R');
		pressed = inputKeysNow[INPUT_KEY_Z] || inputKeysNow[INPUT_KEY_S] || inputKeysNow[INPUT_KEY_Q] || inputKeysNow[INPUT_KEY_D] || inputKeysNow[INPUT_KEY_R];
		
		if (is_input_key_pressed(INPUT_KEY_R))
		{
			won = false;
			free_map(map);
			map = create_map("map.map");
		}
		
		if ((pressed || nbFrame == 0) && !won)
		{
			pressed = false;
			for (unsigned int i = 0; i < SCREEN_HEIGHT; i++)
			{
				for (unsigned int j = 0; j < SCREEN_WIDTH; j++)
				{
					buffer[i][j].Char.AsciiChar = 0;
					buffer[i][j].Attributes = 0;
				}
			}
			
			if (!won)
			{
				won = update_map(map);
			} else {
				draw_text("YOU WON !", map->width + 1, map->height / 2, COLOR_GREEN);
			}
			draw_map(map);
			
			draw_square(1, map->height + 2, COLOR_CYAN);
			draw_square(3, map->height + 2, COLOR_BLUE);
			draw_text("PLAYER ON / NOT ON SENSOR", 5, map->height + 2, COLOR_WHITE);
			
			draw_square(1, map->height + 4, COLOR_GREEN);
			draw_square(3, map->height + 4, COLOR_RED);
			draw_text("BOX ON / NOT ON SENSOR", 5, map->height + 4, COLOR_WHITE);
			
			draw_square(1, map->height + 6, COLOR_YELLOW);
			draw_text("SENSOR", 3, map->height + 6, COLOR_WHITE);
			
			WriteConsoleOutput(hOutput, (CHAR_INFO *)buffer, dwBufferSize, dwBufferCoord, &rcRegion);
		}
		nbFrame++;
    }
	restore_console();
    
    return 0;
}