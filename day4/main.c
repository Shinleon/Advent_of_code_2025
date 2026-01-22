#include <stdio.h>
#include <stdlib.h>

#define STARTINGMEM 4096 

typedef struct Grid {
  char* storage;
  int rows;
  int columns;
  int total_mem;
} Grid;

int main(void) {
  FILE* f = fopen("input.txt", "r");
  if (f == NULL) {
    fprintf(stderr, "can't find input file\n");
  }
  Grid* g = malloc(sizeof(Grid));
  g->total_mem = STARTINGMEM;
  g->rows = 0;
  g->columns = -1;
  g->storage = malloc(sizeof(int)*(g->total_mem));

  int indices_this_row = 0; 
  char c;
  int t_adj = 0;
  while((c = fgetc(f)) != EOF) {
    if(c == '\n' || c == '\r') {
      if(g->columns == -1) { 
        g->columns = indices_this_row;
      } else if (g->columns != indices_this_row) {
        fprintf(stderr, "Input text needs to be a rectangle\n");
        return 1;
      }
      g->rows++;
      indices_this_row = 0;
    } else {
      int adjustment = 0;
      adjustment += g->columns != -1 ? 
                      g->rows*g->columns + indices_this_row : 
                      indices_this_row;
      if (adjustment != t_adj) {
        fprintf(stderr, "not computing adjustment properly\tadj: %d, true_adj: %d \n", adjustment, t_adj);
      }
      if (g->total_mem < adjustment) {
        g->storage = realloc(g->storage, sizeof(char)*2*(g->total_mem));
        g->total_mem *= 2;
        if (g->storage == NULL) {
          fprintf(stderr, "Couldn't allocate enough memory\n");
          return 1;
        }
      }
      g->storage[adjustment] = c;
      indices_this_row++;
      t_adj++;
    }
  }
  /*
  for(int i = 0; i< g->rows*g->columns; i++) {
    printf("%c",g->storage[i]);
    if(((i+1)%g->columns) == 0){
      printf("\n");
    }
  }
  */
  int prev_total = -1;
  int total = 0;
  while(prev_total != total){
    printf("%d, %d\n", prev_total, total);
    prev_total = total;
    for(int r = 0; r < g->rows; r++) {
      for(int c = 0; c < g->columns; c++) {
        int index = r*g->columns + c;
        if (g->storage[index] == '@'){
          int n = -1;
          int e = 1;
          int s = 1;
          int w = -1;
          if(r <= 0){
            // truncate prev row
            n = 0;
          }
          if (r >= g->rows-1) {
            // truncate next row
            s = 0;
          }
          if(c < 1) {
            // truncate prev column
            w = 0;
          }
          if (c >= g->columns - 1) {
            // truncate next column
            e = 0;
          }
          // we have all direction bounds
          //    and can loop through them
          int count = 0;
          for (int ud = n; ud <= s; ud++) {
            for(int lr = w; lr <= e; lr++){
              if (lr != 0 || ud != 0) {
                //check if storage + ud/lr == '@'
                int c_index = index + ud * g->columns + lr;
                if (g->storage[c_index] == '@'){
                  count++;
                }
              }
            }
          }
          if (count < 4) {
            g->storage[index] = '.';
            total += 1;
          }

        }
      }
    }
  }

  printf("total: %d\n", total);
}
