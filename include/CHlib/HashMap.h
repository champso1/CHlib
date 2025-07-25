#ifndef __HASH_MAP_H
#define __HASH_MAP_H

/*
 * This is a super simple Hash Map implementation.
 *
 * It works only for string keys and int values,
 * and the table itself is moderately small (10 000).
 *
 * The purpose of this is to be used within some other project of mine,
 * in which I know that there will be only minimal usage.
 */



#define HASHTABLE_SIZE 10000



/// I did it this way to more easily decide whether a slot was filled or not.
typedef struct _HashTableEntry
{
	int val;
	enum
	{
		HM_EMPTY = 0,
		HM_FILLED,
	} type;
} HashTableEntry;


typedef struct _HashMap
{
	int len;
	HashTableEntry *table;
} HashMap;


/// Initializes a hashmap with all zeros (unfilled slots).
extern HashMap hmInit();

/// frees malloced data
extern void hmDeinit();

/// Adds a string,int pair to the has table, with the strings being keys
/// Returns 1 if key exists (considered an error), 0 on success
extern int hmAdd(HashMap* map, const char* key, int val);

/// Gets the int val associated with the given key.
/// Simply returns the entry at that location;
/// what to do if it is empty is left to the user.
extern HashTableEntry hmGet(HashMap map, const char* key);

/// Prints the value within the @a entry,
/// or if it is empty, prints a warning.
extern void hmPrint(HashTableEntry entry);










#endif // __HASH_MAP_H
