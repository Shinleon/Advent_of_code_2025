#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define DEFAULTALLOC (128)

static int split_count = 0;

typedef struct {
  int* laserIndexes;
  int allocated;
  int used;
} LineData;

LineData* LineData_init(void){
  LineData* ret = malloc(sizeof(LineData));
  int* temp = malloc(sizeof(int)*DEFAULTALLOC);
  if(ret == NULL || temp == NULL) {
    fprintf(stderr, "Couldn't allocate for LineData\n");
    return NULL;
  }
  ret->laserIndexes = temp;
  ret->allocated = DEFAULTALLOC;
  ret->used = 0;
  return ret;
}

bool LineData_contains(LineData* ld, int to_find){
  for(int i = 0; i< ld->used; i++){
    if(ld->laserIndexes[i] == to_find){
      return true;
    }
  }
  return false;
}

int LineData_insert(LineData* ld, int insertable){
  if(!LineData_contains(ld, insertable)){
    if (ld->used >= ld->allocated) {
      int* temp = realloc(ld->laserIndexes, ld->allocated*sizeof(int)*2);
      if(temp == NULL){
        fprintf(stderr, "Failed to rallocate for lineData insert\n");
        return 1;
      }
      ld->laserIndexes = temp;
    }
    ld->laserIndexes[ld->used] = insertable;
    ld->used += 1;
  }
  return 0;
}

void LineData_clear(LineData* ld) {
  memset(ld->laserIndexes, 0,  ld->allocated*sizeof(int));
  ld->used = 0;
}

void LineData_free(LineData* ld) {
  if (ld->laserIndexes){
    free(ld->laserIndexes);
  }
  free(ld);
}

void applyLine(char* data, int len, LineData* currentlasers, LineData* result){
  LineData* removed = LineData_init();
  for(int i = 0; i< len; i++){
    // if char at data[i] == '^', look for a corresponding laser in currentlasers
    //    and add to result set i-1 and i+1;
    // else if char == 'S'
    //    just add the index i to result set;
    if (data[i] == 'S'){
      LineData_insert(result, i);
    } else if (data[i] == '^') {
      if (LineData_contains(currentlasers, i)){
        LineData_insert(removed, i);
        LineData_insert(result, i-1);
        LineData_insert(result, i+1);
        split_count++;
      }
    }
  }
  for (int i = 0; i< currentlasers->used; i++){
    if(!LineData_contains(removed, currentlasers->laserIndexes[i])){
      LineData_insert(result,currentlasers->laserIndexes[i]);
    }
  }
  LineData_free(removed);
}

int intCompare(const void* x, const void* y){
  return (*(int*)x) - (*(int*)y);
}

int main(void) {
  // after each row, reasses where we are splitting from.
  // so we have a list of indexes where lazers are dropping from.
  // we can then usethe next row of the input file and 
  // by looping through the list of indexes, if the next row of text
  // has a '^' at that index, then we split it into it's left and right
  // until the last line of the text file.
  FILE* f = fopen("intput.txt", "r");
  if(f == NULL){
    fprintf(stderr, "couldn't find input file");
  }
 
  char c;
  int line_len = 0;
  int index = 0;
  int allocated = 128;
  char* x = malloc(sizeof(char)*allocated);
  LineData* currline = LineData_init();
  LineData* result = LineData_init();
  bool not_finished = true;
  while((c = fgetc(f)) != EOF) {
    printf("%c", c);
    if(c != '\n'){
      not_finished = true;
      x[index] = c;
      index++;
      if(index >= allocated) {
        char* _temp = realloc(x, sizeof(char)*allocated*2);
        if(_temp == NULL) {
          fprintf(stderr, "Couldn't reallocate more line char\n");
        }
        x = _temp;
      }
    } else {
      not_finished = false;
      applyLine(x, index, currline, result);
      LineData* temp = currline;
      currline = result;
      result = temp;
      LineData_clear(result);
      line_len = index;
      index = 0;
    }
  }
  for(int i = 0; i < line_len; i++){
    printf("%d", i%10);
  }
  printf("\n");
  qsort(currline->laserIndexes, currline->used, sizeof(int), intCompare);
  printf("\n >>>\n");
  for(int i = 0; i < currline->used; i++){
    printf("%d, ", currline->laserIndexes[i]);
  }
  printf("\n>>>\n");
  if (not_finished) {
    printf("one more line\n");
  }
  printf("used: %d\n", currline->used);
  printf("split count: %d\n", split_count);
  LineData_free(currline);
  LineData_free(result);
}
