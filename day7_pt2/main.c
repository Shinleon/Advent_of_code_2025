#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int main(void) {
  FILE* f = fopen("input.txt", "r");
  char c;
  int line_count = 0;
  int char_limit = 0;
  int char_count = 0;
  int initial_S_index = -1;
  uint64_t* storage = NULL;
  uint64_t* previous = NULL;
  while((c = fgetc(f)) != EOF) {
    if(line_count == 0){
      if(c == '\n'){
        char_limit = char_count;
        char_count = -1;
        printf("char_limit: %d\n", char_limit);
        storage = malloc(char_limit*sizeof(uint64_t));
        previous = malloc(char_limit*sizeof(uint64_t));
        if (storage == NULL || previous == NULL) {
          fprintf(stderr, "Couldn't allocate mem for storage\n");
          return 1;
        }
        previous[initial_S_index] = 1;
        line_count++;
      } else if (c == 'S'){
        initial_S_index = char_count;
      }
      char_count++;
    } else {
      if(c == '\n'){
        // swap the previous and storage;
        uint64_t* t = storage;
        storage = previous;
        memset(storage, 0, char_limit*sizeof(uint64_t));
        previous = t;
        char_count = -1; // set to neg1 because we will add to it outside the branch
        line_count++;
      } else if (c == 'S'){
        initial_S_index = char_count;
      } else if (c == '^') {
        storage[char_count - 1] += previous[char_count];
        storage[char_count + 1] += previous[char_count];
        storage[char_count] = 0;
      } else if (c == '.') {
        storage[char_count] += previous[char_count];
      }
      char_count++;
    }
  }
  uint64_t total = 0;
  for(int i = 0; i< char_limit; i++){
    total += previous[i];
  } 

  printf("total: %ld\n", total);
  free(storage);
  free(previous);
}
