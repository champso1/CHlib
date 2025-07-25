#include "CHlib/Utils.h"


i32 readFile(str file_path, u8** buf) {
	// first we want to get the size
    FILE* file = null;
	file = fopen(file_path, "rb");
	if (file == null)
		return -1;
	
	fseek(file, 0, SEEK_END);
    u32 file_size = ftell(file);
    fclose(file);

	// then allocate a buffer with the given size
	u8* file_buf = null;
	file_buf = malloc(file_size * (sizeof *file_buf) + 1); // +1 for null character
	if (file_buf == null)
		return -2;

	memset(file_buf, '\0', file_size + 1);

	file = fopen(file_path, "r");
	if (file == null)
		return -3;

	fread(file_buf, sizeof *file_buf, file_size, file);
	fclose(file);

	*buf = file_buf;
	return file_size;
}

