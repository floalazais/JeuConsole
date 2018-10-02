#include <stdarg.h>
#include <Windows.h>
#include <stdio.h>
#include "error.h"

__attribute__((noreturn)) void error(char *format, ...)
{
	char buffer[1024] = "ERROR : ";
	va_list arguments;
	va_start(arguments, format);
    vsprintf(buffer + 8, format, arguments);
	va_end(arguments);
	strcat(buffer, "\n");
	puts(buffer);
	exit(EXIT_FAILURE);
}