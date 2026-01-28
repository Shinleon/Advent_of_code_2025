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

int compare_Range(const void* small, const void* large){
  Range* s = (Range*) small;
  Range* l = (Range*) large;
  if (s->min != l->min){
    return  s->min < l->min ? -1 : 1;
  } 
  return s-> max < l-> max ? -1: 1;
}

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
  qsort(ranges, range_used, sizeof(Range), compare_Range);
  int to_modify = 0;
  bool merging_last = false;
  for(int j = 1; j < range_used; j++) {
    if (ranges[to_modify].max + 1 >= ranges[j].min) {
      ranges[to_modify].max = ranges[j].max > ranges[to_modify].max ? ranges[j].max : ranges[to_modify].max;
      merging_last = true;
    } else {
      to_modify += 1;
      ranges[to_modify].max = ranges[j].max;
      ranges[to_modify].min = ranges[j].min;
      merging_last = false;
    }
  }
  if (merging_last) {
    to_modify += 1;
  }
  uint64_t total = 0;
  for(int i = 0; i< to_modify; i++) {
    total += ranges[i].max - ranges[i].min + 1;
    printf("[%ld, %ld]:\t size: %ld, total: %ld\n", 
           ranges[i].min, ranges[i].max, ranges[i].max - ranges[i].min +1, total);
  }
  printf("to_modify: %d, original length: %d\n", to_modify, range_used);
  printf("total: %ld\n", total);
  free(ranges);
}
