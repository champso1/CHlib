#include "Utils.h"


u32 readFile(str filePath, u8** buf) {
	// first we want to get the size
    FILE* file = NULL;
	file = fopen(filePath, "rb");
	PTR_ASSERT(file, "[ERROR] Could not open file: %s\n", filePath);
	
	fseek(file, 0, SEEK_END);
    u32 fileSize = ftell(file);
    fclose(file);

	// then allocate a buffer with the given size
	u8* fileBuf = NULL;
	fileBuf = malloc(fileSize * (sizeof *fileBuf) + 1); // +1 for null character
	PTR_ASSERT(fileBuf, "[ERROR] Could not allocate memory for file buffer.\n");
	memset(fileBuf, '\0', fileSize + 1);

	file = fopen(filePath, "r");
	PTR_ASSERT(file, "[ERROR] Could not reopen file in text mode.\n");
	fread(fileBuf, sizeof *fileBuf, fileSize, file);
	fclose(file);

	*buf = fileBuf;
	return fileSize;
}

