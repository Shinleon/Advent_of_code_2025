#include <stdio.h>
#include <stdlib.h>

#define ASCIIZERO (48)

int main(void){
  printf("day3_pt2\n");
  FILE* f = fopen("input.txt", "r");
  if (f == NULL) {
    fprintf(stderr, "no file found for input\n");
    return 1;
  }

  char c;
  int size = 128;
  int* line = malloc(sizeof(int)*size);
  int* digits = calloc(12, sizeof(int));
  int index = 0;
  unsigned long long total = 0;
  while((c = fgetc(f)) != EOF){
    if(c =='\n') {
      // up to and not including index
      // find largest digit in range [0, index - 12]
      int left_inclusive = 0; // this should be to the right of the last
                              //  selected digit
      for(int i = 12; i >= 1; i--) {
        for(int j = left_inclusive; j <= index - i; j++) {
          if(digits[12-i] < line[j]) {
            digits[12-i] = line[j];
            left_inclusive = j+1;
          }
        }
      }
      for(int i = 0; i< index; i++) {
        printf("%d", line[i]);
      }
      printf("\n");
      printf("digits: ");
      for(int j= 0; j < 12; j++) {
        printf("%d", digits[j]);
      }
      printf("\n");
      index = 0;
      unsigned long long _temp = 0;
      for(int i = 0; i < 12; i++) {
        printf("%llu, %d\n", _temp, digits[i]);
        _temp = _temp*(10llu) + digits[i];
        digits[i] = 0;
      }
      printf("temp: %llu\n", _temp);
      total += _temp;
    } else {
      line[index] = (int)(c - ASCIIZERO);
      index++;
    }
  }
  printf("total: %llu\n", total);
  free(line);
  free(digits);
}
