#include <stdio.h>
#include <conio.h>
#include "error.h"
#include "map.h"
#include "globals.h"
#include "ascii_rectangle.h"

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

bool inputKeysBefore[INPUT_KEY_COUNT];
bool inputKeysNow[INPUT_KEY_COUNT];

static void is_input_key_supported(InputKey inputKey)
{
	if (inputKey < 0 || inputKey > INPUT_KEY_COUNT)
	{
		error("testing unsupported input key : %d.", inputKey);
	}
}

bool is_input_key_up(InputKey inputKey)
{
	is_input_key_supported(inputKey);
	return !inputKeysNow[inputKey];
}

bool is_input_key_down(InputKey inputKey)
{
	is_input_key_supported(inputKey);
	return inputKeysNow[inputKey];
}

bool is_input_key_released(InputKey inputKey)
{
	is_input_key_supported(inputKey);
	return inputKeysBefore[inputKey] && !inputKeysNow[inputKey];
}

bool is_input_key_pressed(InputKey inputKey)
{
	is_input_key_supported(inputKey);
	return !inputKeysBefore[inputKey] && inputKeysNow[inputKey];
}

void update_input_keys()
{
	memcpy(inputKeysBefore, inputKeysNow, sizeof (inputKeysBefore));
}

bool compare_rectangle(SMALL_RECT *a, SMALL_RECT *b)
{
	return a->Top == b->Top && a->Left == b->Left && a->Right == b->Right && a->Bottom == b->Bottom;
}

CHAR_INFO buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

int main()
{   
	HWND consoleWindow = GetConsoleWindow();
	LONG oldWindowStyle = GetWindowLong(consoleWindow, GWL_STYLE);
	SetWindowLong(consoleWindow, GWL_STYLE, oldWindowStyle & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
	

    COORD dwBufferSize = {SCREEN_WIDTH, SCREEN_HEIGHT};
    COORD dwBufferCoord = {0, 0};
    SMALL_RECT rcRegion = {0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1};
	
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	
	CHAR_INFO oldBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
	ReadConsoleOutput(hOutput, (CHAR_INFO *)oldBuffer, dwBufferSize, dwBufferCoord, &rcRegion);
	
	CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOutput, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hOutput, &cursorInfo);
    
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, prevMode & ~ENABLE_QUICK_EDIT_MODE & ~ENABLE_MOUSE_INPUT & ~ENABLE_PROCESSED_INPUT);
	
	AsciiRectangle *ar = create_ascii_rectangle();
	ar->xPos = 8;
	ar->yPos = 10;
	ar->width = 5;
	ar->height = 4;
	ar->color = COLOR_WHITE;
	
	CONSOLE_SCREEN_BUFFER_INFO CSBInfo;
	
	int nbFrame = 0;
	bool pressed;
	
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
			free_map(map);
			map = create_map("map.map");
		}
		
		if (pressed || nbFrame == 0)
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
			
			update_map(map);
			draw_map(map);
			
			WriteConsoleOutput(hOutput, (CHAR_INFO *)buffer, dwBufferSize, dwBufferCoord, &rcRegion);
		}
		nbFrame++;
    }
	WriteConsoleOutput(hOutput, (CHAR_INFO *)oldBuffer, dwBufferSize, dwBufferCoord, &rcRegion);
    
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hOutput, &cursorInfo);
    
    SetConsoleMode(hInput, prevMode);
	
	SetWindowLong(consoleWindow, GWL_STYLE, oldWindowStyle);
    
    return 0;
}