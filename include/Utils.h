#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


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
			fprintf(stderr, ANSI_COLOR_YELLOW "[WARNING] " ANSI_COLOR_RESET); \
		} else {									\
				fprintf(stderr, ANSI_COLOR_RED "[ERROR] " ANSI_COLOR_RESET); \
			} fprintf(stderr, __VA_ARGS__);								\
	} while (0)




unsigned char* readFileData(const char* file_path) {
	size_t file_size;

	// open in binary and use seek fn's to get the total size
	FILE* file = fopen(file_path, "rb");
	if (file == NULL) {
		LOG_MESSAGE(LOG_ERROR, "(readFileData) Could not open file in binary mode: %s.\n", file_path);
		return NULL;
	}
	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	fclose(file); file = NULL;

	// allocate memory and fill with null characters
	char* buf = NULL;
	buf = malloc((file_size+1) * (sizeof *buf));
	memset(buf, '\0', file_size+1);
	
	// reopen in normal reading mode and read the contents
	file = fopen(file_path, "r");
	fseek(file, 0, SEEK_SET);
	if (file == NULL) {
		LOG_MESSAGE(LOG_ERROR, "(readFileData) Could not open file to read contents: %s.\n", file_path);
		return NULL;
	}
	fread(buf, 1, file_size, file);
	fclose(file);

	LOG_MESSAGE(LOG_INFO, "(readFileData) File contents are:\n");
	fprintf(stderr, "%s\n", buf);

	return buf;
}








#endif // #define _UTILS_H_
