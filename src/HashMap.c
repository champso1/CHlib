#include "CHlib/HashMap.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


HashMap hmInit()
{
	HashMap hm;
	memset(&hm, 0, sizeof hm);
	hm.table = calloc(HASHTABLE_SIZE, sizeof *hm.table);
	return hm;
}

void hmDeinit(HashMap hm)
{
	if (hm.table != NULL)
		free(hm.table);
}



// hashing function
static unsigned long __hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return (hash % HASHTABLE_SIZE);
}



int hmAdd(HashMap* map, const char* key, int val)
{
	unsigned long hash_code = __hash((unsigned char*)key);
	if (map->table[hash_code].type == HM_FILLED)
	{
		fprintf(stderr, "[WARNING] Hash table slot at index %ld is filled.\n", hash_code);
		return 1;
	}
	map->table[hash_code] = (HashTableEntry){
		.val = val,
		.type = HM_FILLED,
	};
	map->len += 1;
	
	return 0;
}


HashTableEntry hmGet(HashMap map, const char* key)
{
	unsigned long hash_code = __hash((unsigned char*)key);
	return map.table[hash_code];
}



void hmPrint(HashTableEntry entry)
{
	if (entry.type == HM_EMPTY)
	{
		fprintf(stderr, "[WARNING] This entry is empty.\n");
		return;
	}

	printf("%d\n", entry.val);
}
