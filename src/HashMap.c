#include "HashMap.h"

#include <stdio.h>
#include <string.h>


HashMap hmInit() {

	HashMap hm;
	memset(&hm, 0, sizeof hm);
	
	return hm;
}


static unsigned long __hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return (hash % HASHTABLE_SIZE);
}



int hmAdd(HashMap* map, const char* key, int val) {
	unsigned long hashCode = __hash((unsigned char*)key);
	if (map->hashTable[hashCode].type == FILLED) {
		fprintf(stderr, "[WARNING] Hash table slot at index %ld is filled.\n", hashCode);
		return 1;
	}
	map->hashTable[hashCode] = (HashTableEntry){
		.val = val,
		.type = FILLED,
	};
	map->numElements += 1;
	
	return 0;
}


HashTableEntry hmGet(HashMap map, const char* key) {
	unsigned long hashCode = __hash((unsigned char*)key);
	return map.hashTable[hashCode];
}



void hmPrint(HashTableEntry entry) {
	if (entry.type == EMPTY) {
		fprintf(stderr, "[WARNING] This entry is empty.\n");
		return;
	}

	printf("%d\n", entry.val);
}
