#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define ASCIIZERO (48)
#define DEFAULTALLOC (5000)

typedef struct NumLines {
  int allocated;
  int used;
  int* nums;
} NumLines;

typedef enum Operation {
  ADD,
  MULT
} Operation;

uint64_t do_op(Operation op, int i, int* nums){
  if (op == MULT) {
    uint64_t result = (uint64_t)nums[i] * nums[i+1000] * nums[i+2000] * nums[i+3000];
    return result;
  }
  return (uint64_t)nums[i] + nums[i+1000] + nums[i+2000] + nums[i+3000];
}

int main(void) {
  printf("day 6 part one(1)\n");
  FILE* f = fopen("input.txt", "r");
  char c;
  NumLines* data = malloc(sizeof(NumLines));
  data->nums = malloc(sizeof(int)*DEFAULTALLOC);
  data->allocated = DEFAULTALLOC;
  data->used = 0;
  data->nums[0] = 0;
  bool previous_whitespace = true;
  while((c = fgetc(f)) != EOF) {
    if (c == ' ') {
      previous_whitespace = true;
    } else if (c == '\n') {
      previous_whitespace = true;
    } else if (c >= '0' && c <= '9') {
      if (previous_whitespace) {
        data->used += 1;
        previous_whitespace = false;
        data->nums[data->used - 1] = 0;
      }
      int* building = (data->nums)+data->used - 1;
      *building = *building*10 + (int)c - ASCIIZERO;
    } else if (c == '*' || c ==  '+'){
      data->used +=1;
      previous_whitespace = false;
      data->nums[data->used - 1] = c == '*' ? MULT : ADD;
    }
    if (data->used >= data->allocated - 1) {
      int* t = realloc(data->nums, data->allocated*sizeof(int)*2);
      if (t == NULL) {
        fprintf(stderr, "Couldn't allocate more memory\n");
      }
      data->nums = t;
      data->allocated *= 2;
    }
 
  }
  uint64_t total = 0;
  for(int i = 0; i < 1000; i++) {
    uint64_t result = 0;
    result = do_op(data->nums[4000 + i], i, data->nums);
    total += result;
  }
  printf("total: %ld\n", total);
}
