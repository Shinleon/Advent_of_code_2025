#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define DEFAULTALLOC (1024)
#define LINECOUNT (5)
typedef struct {
  char* chars;
  int allocated;
  int used;
} Line;

// l and r are both inclusive
uint64_t doOperation(Line* lines, int l, int r){
  uint64_t ret = 0;
  int count = sizeof(int)*(r-l+1);
  int* data = malloc(count);
  memset(data, 0, count);
  for(int i = l; i <= r; i++){
    // stuff TODO
    // go down the columns and construct in digit by digit [0, 5)
    for(int _j = 0; _j < LINECOUNT; _j++){
      char c = lines[_j].chars[i];
      printf("%c,",c);
      if(c >= '0' && c <= '9') {
        data[i-l] = data[i-l]*10 + (int)(c-'0');
      }
    }
    printf("\n");
  }
  for(int i = 0; i< r-l+1; i++){
    printf("%d, ", data[i]);
  }
  char operation = lines[LINECOUNT-1].chars[l];
  if (operation == '+'){
    for(int i = 0; i<r-l+1; i++){
      ret += data[i];
    }
  } else {
    ret = 1;
    for(int i = 0; i<r-l+1; i++){
      ret *= data[i];
    }
  }
  printf("operation: %c \n", operation);
  free(data);
  return ret;
}


int main(void) {
  printf("day6 part2\n");
  Line* x = malloc(sizeof(Line)*LINECOUNT);
  FILE* f = fopen("input.txt", "r");
  char c;
  Line* curr = x;
  while((c = fgetc(f)) != EOF) {
    if(c == '\n'){
      curr += 1;
    } else {
      if (curr->chars == NULL) {
        curr->chars = malloc(sizeof(char)*DEFAULTALLOC);
        curr->allocated = DEFAULTALLOC;
        curr->used = 0;
      }
      curr->chars[curr->used] = c;
      curr->used += 1;
      if (curr->used >= curr->allocated) {
        char* _t = realloc(curr->chars, (curr->allocated)*sizeof(char)*2);
        if (_t == NULL){
          fprintf(stderr, "Couldn't realloc for line mem\n");
        }
        curr->allocated *= 2;
        curr->chars = _t;
      }
    }
  }
  // we know the last line has the operands
  printf("used per line: %d\n", x->used);
  int l = 0;
  int r = 1;
  Line* ops = x+LINECOUNT-1;
  uint64_t total = 0;
  while(r < ops->used){
    if(ops->chars[r] == '+' || ops->chars[r] == '*' || r+1 == ops->used) {
      // we have reached the end;
      // we can grab from l -> r-2
      int modifier = -2;
      if(r+1 == ops->used){
        modifier = 0;
      }
      total += doOperation(x, l, r+modifier);
      // this is the last step to update to next window
      l = r;
    } 
    r++;
  }
  printf("total: %ld\n", total);
  return 0;
}
