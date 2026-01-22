#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define ASCIIZERO (48)
#define ASCIININE (57)

typedef struct Range {
  uint64_t min;
  uint64_t max;
} Range;

int main(void) {
  printf("day five, part one\n");
  int range_size = 30;
  int range_used = 0;
  Range* ranges = malloc(sizeof(Range)*range_size);

  FILE* f = fopen("input.txt", "r");
  if (f == NULL) {
    fprintf(stderr, "couldn't open file\n");
  }

  char c;
  bool dashSeen = false;
  uint64_t temp = 0;
  int total = 0;
  while((c = fgetc(f)) != EOF){
    if (c >= ASCIIZERO && c <= ASCIININE){
      temp = temp*10 + (c-ASCIIZERO);
    } else if (c == '-') {
      dashSeen = true;
      ranges[range_used].min = temp;
      temp = 0;
    } else {
      if (dashSeen) {
        ranges[range_used].max = temp;
        printf("range found %lu, %lu\n", ranges[range_used].min, ranges[range_used].max);
        dashSeen = false;
        range_used++;
        if (range_used >= range_size) {
          int new_size = range_size*2;
          Range* n_ranges = realloc(ranges, sizeof(Range)*new_size);
          if (n_ranges == NULL) {
            fprintf(stderr, "Reallocation failed\n");
          } 
          ranges = n_ranges;
          range_size = new_size;
        }
      } 
      temp = 0;
    }
  }

  // so we have all the ranges in <ranges>
  // need to find cout of all unique values in the range in <ranges>

  printf("total: %d\n", total);
  free(ranges);
}
