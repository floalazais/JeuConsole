#include "error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

__attribute__((noreturn)) void error(char *format, ...)
{
	char buffer[1024] = "ERROR : ";
	va_list arguments;
	va_start(arguments, format);
    vsprintf(buffer + 8, format, arguments);
	puts(buffer);
	va_end(arguments);
	exit(EXIT_FAILURE);
}