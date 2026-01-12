#include <stdio.h>

#define ASCIIZERO  (48)

int assignAll(int x[], int len_of_x, int assignment_val) {
  if (x == NULL) {
    return -1;
  }
  for(int i = 0; i < len_of_x; i++) {
    x[i] = assignment_val;
  }
  return 0;
}

int main(void){
  printf("day3\n");
  char file_name[] = "input.txt";
  FILE* f = fopen(file_name, "r");
  if (f == NULL){
    fprintf(stderr, "couldn't open file %s", file_name);
    return -1;
  }
  char c;
  int digits[10] = {};
  int highest_so_far = 0;
  int total = 0;
  while ((c = fgetc(f)) != EOF){
    if (c == '\n'){
      printf("\n");
      for(int i = 9; i >= 0; i--){
        if (digits[i] != -1) {
          total += i*10 + digits[i];
          printf("total: %d\n", i*10 + digits[i]);
          break;
        }
      }
      assignAll(digits, 10, -1);
      highest_so_far = 0;
    } else {
      // we can assume c is a digit '0' -> '9'
      printf("%c", c);
      int powerVal = (int)(c - ASCIIZERO);
      digits[highest_so_far] = digits[highest_so_far] > powerVal? digits[highest_so_far] : powerVal;
      highest_so_far = highest_so_far > powerVal ? highest_so_far : powerVal; 
    }
  }
  printf("total: %d\n", total);
  fclose(f);
}
