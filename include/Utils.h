#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>


// colors for printing to the terminal
#define ANSI_COLOR_RED         "\x1b[31m"
#define ANSI_COLOR_GREEN       "\x1b[32m"
#define ANSI_COLOR_YELLOW      "\x1b[33m"
#define ANSI_COLOR_BLUE        "\x1b[34m"
#define ANSI_COLOR_MAGENTA     "\x1b[35m"
#define ANSI_COLOR_CYAN        "\x1b[36m"

#define ANSI_COLOR_RESET       "\x1b[0m"



typedef unsigned int LOG_CODE;

#define LOG_INFO      1
#define LOG_WARNING   2
#define LOG_ERROR     3



#define LOG_MESSAGE(code, ...)											\
	do {																\
		if ((code) == LOG_INFO)	{										\
			fprintf(stderr, ANSI_COLOR_GREEN "[INFO] " ANSI_COLOR_RESET); \
		} else if ((code) == LOG_WARNING) {								\
			fprintf(stderr, ANSI_COLOR_YELLOW "[INFO] " ANSI_COLOR_RESET); \
		} else {									\
				fprintf(stderr, ANSI_COLOR_RED "[INFO] " ANSI_COLOR_RESET); \
			} fprintf(stderr, __VA_ARGS__);								\
	} while (0)
















#endif // #define _UTILS_H_
