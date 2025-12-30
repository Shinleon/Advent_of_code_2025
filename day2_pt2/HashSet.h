#ifndef _HASHSET_
#define _HASHSET_

#include <stdbool.h>

#define DEFAULTCAPACITY 3
#define DEFAULTDEPTH 3

/* * * * * * *
 * Hashset struct;
 *  -> has `capacity` rows
 *  -> has an integer prefix in each row to tell you how many in an individual row
 *  -> has `depth` item slots after the integer prefix
 */
struct HashSet {
  int capacity;   // how many slots we currently have
  int depth;      // how many items per slot
  unsigned long itemSize;   // size of an individual item in bytes
  void* storage; // where the items will go
  int (*hashFunc) (void*);
  int (*compareFunc) (void*, void*);
  int (*freeFunc) (void*);
};

struct HashSet* HashSet_initialize(unsigned long itemsize, int (*hashFunc) (void*), int (*compareFunc) (void*, void*), int (*freeFunc) (void*));

int HashSet_insertElement(struct HashSet* set, void* elementAddr);

bool HashSet_findElement(struct HashSet* set, void* elementAddr);

void HashSet_free(struct HashSet* toFree);

void HashSet_print(struct HashSet* toPrint);

#endif

