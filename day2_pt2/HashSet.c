#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "HashSet.h"
#include "utils.h"

struct HashSet* HashSet_initialize(unsigned long itemsize, int (*hashFunc) (void*), int (*compareFunc) (void*, void*), int (*freeFunc) (void*)) {
  struct HashSet* ret = malloc(sizeof(struct HashSet));
  ret->capacity = DEFAULTCAPACITY;
  ret->depth = DEFAULTDEPTH;
  ret->storage = malloc(ret->capacity * ret->depth * itemsize + ret->capacity*sizeof(int));
  ret->hashFunc = hashFunc == NULL ? hashEightBytesToFour : hashFunc;
  ret->compareFunc = compareFunc == NULL ? compareEightBytes : compareFunc;
  ret->freeFunc = freeFunc; // This could be NULL if we don't need to free what goes into the HashSet
  ret->itemSize = itemsize;
  return ret;
}

int HashSet_find(struct HashSet* set, void* elementAddr) {
  int found = -1;
  int bucket_mem_size = set->depth * set->itemSize + sizeof(int);
  for(int i = 0; i < set->capacity; i++) {
    int* quant_this_bucket = (int*)((char*)set->storage + i * bucket_mem_size);
    for (int j = 0; j < *quant_this_bucket; j++) {
      void* to_compare = (void*)((char*)quant_this_bucket + sizeof(int) + j*set->itemSize);
      int result = set->compareFunc(elementAddr, to_compare);
      if (0 == result) {
        return i;
      }
    }
  }
  return found;
}

bool HashSet_findElement(struct HashSet* set, void* elementAddr) {
  bool ret = (-1 != HashSet_find(set, elementAddr));
  return ret;
}

struct HashSet* HashSet_resize(struct HashSet* s) {
  // double depth and capacity
  int new_cap = s->capacity * 2;
  int new_dep = s->depth * 2;
  char* rplcmnt = (char*) malloc(sizeof(int)*new_cap + new_cap*new_dep*s->itemSize);
  // loop through s->storage and rehash and insert each elemnt into rplcmnt
  for (int i = 0; i < s->capacity; i++) {
    // loop through each bucket
    int* bucket_quant = (int*)(((char*)s->storage) + i*(s->depth*s->itemSize + sizeof(int)));
    printf("bucket %d, %d/%d\n", i, *bucket_quant, s->depth);
    for(int j = 0; j < *bucket_quant; j++) {
      // loop through all bucket items in s->storage
      void* item = (void*) (((char*)bucket_quant) + j*s->itemSize + sizeof(int));
      int rebucket = s->hashFunc(item) % new_cap;
      int* new_buck_q = (int*)(rplcmnt + (new_dep*s->itemSize + sizeof(int))*rebucket);
      void* n_loc = (void*)(((char*)new_buck_q)+ sizeof(int) +s->itemSize*(*new_buck_q));
      for (int k = 0; k < s->itemSize; k++) {
        ((char*)n_loc)[k] = ((char*)item)[k];
      }
      *new_buck_q = *new_buck_q + 1;
    }
  }
  s->capacity = new_cap;
  s->depth = new_dep;
  free(s->storage);
  s->storage = (void*) rplcmnt;
  return s;
}

int HashSet_insertElement(struct HashSet* set, void* elementAddr) {
  if (set == NULL || elementAddr == NULL) {
    return -1;
  }
  int bucket = HashSet_find(set, elementAddr);
  if (bucket == -1) {
    bucket = set->hashFunc(elementAddr) % set->capacity;
    if (*(int*)(((char*)set->storage) + sizeof(int)*bucket + set->depth*set->itemSize*bucket) >= 
        set->depth) {
      set = HashSet_resize(set);
    }
    // bucket & capacity may have changed due to resizing
    bucket = set->hashFunc(elementAddr) % set->capacity;
    int* bucket_quant = (int*)((char*)set->storage + bucket*(sizeof(int)+set->depth*set->itemSize));
    void* copy_location = (void*)((char*)bucket_quant  + sizeof(int) + set->itemSize*(*bucket_quant));
    for (int i = 0; i< set->itemSize; i++) {
      ((char*)copy_location)[i] = ((char*)elementAddr)[i];
    }
    *bucket_quant = *bucket_quant + 1;
  }
  return 0;
}

void HashSet_free(struct HashSet* toFree) {
  free(toFree->storage);
  free(toFree);
}

void HashSet_print(struct HashSet* toPrint) {
  for(int i = 0; i < toPrint->capacity; i++ ) {
    int* rowHeader = ((int*)((char*)toPrint->storage + sizeof(int)*i + toPrint->depth*i*toPrint->itemSize));
    if (*rowHeader != 0) {
      printf("row %d |\t %d/%d |\t", i, *rowHeader, toPrint->depth);
      char* first_element = ((char*)rowHeader) + sizeof(int);
      for(int j = 0; j < *rowHeader; j++) {
        printf("%lld", *((long long*) first_element));
        printf(" | ");
        first_element += toPrint->itemSize;
      }
    printf("\n");
    }

  }
}


