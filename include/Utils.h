#ifndef __UTILS_H_
#define __UTILS_H_

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



#define PTR_ASSERT(ptr, ...) if(!(ptr)) {fprintf(stderr, __VA_ARGS__); exit(1);}



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



typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

typedef char i8;
typedef short i16;
typedef int i32;
typedef long i64;

typedef float f32;
typedef double f64;


typedef const char* str;




extern u32 readFile(str filePath, u8** buf);





#endif // #define __UTILS_H_
