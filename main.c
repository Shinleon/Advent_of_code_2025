#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ZEROASCIIVAL (48)

struct intConstructor {
  char* char_rep;
  int char_limit;
  long long num_rep;
  int nu_digits;
};

//  create a intConstructor or attach a Digit to one
//    if ic is null, create, else attach a Digit
//  if t i not a single digit integer, we have issues, so return NULL
//    No other reason to return NULL
struct intConstructor* intConstructor_attachDigit(struct intConstructor* ic, int t) {
  if (t < 0 || t > 10) {
    fprintf(stderr, "intConstructor_attachDigit expects a single digit as its second param, %d", t);
    return NULL;
  }
  if (ic == NULL) {
    ic = malloc(sizeof(struct intConstructor));
    ic->char_limit = 21;
    ic->nu_digits = 0;
    ic->num_rep = 0;
    ic->char_rep = malloc(ic->char_limit*sizeof(char));
  }
  if (ic->nu_digits + 1 >= ic->char_limit - 1) {
    // we require a resizing as the number of digits in the integer is greater than
    //  we have space fore in our char_rep
    ic->char_limit *= 2;
    char* new_charArr = malloc(ic->char_limit*sizeof(char));
    // copy over
    for (int i = 0; i < ic->char_limit; i++) {
      new_charArr[i] = ic->char_rep[i];
    }
    free(ic->char_rep);
    ic->char_rep = new_charArr;
  }
  // add new digit to char representation 
  ic->char_rep[ic->nu_digits] = (char)(t+ZEROASCIIVAL); // add the desired char to the char_rep
  ic->char_rep[(ic->nu_digits)+1] = '\0'; // add endline char;
  ic->nu_digits += 1; // increase the number of digits by 1
  ic->num_rep = ic->num_rep * 10 + t; // add the new int to the end of the int_rep
  return ic;
}

void intConstructor_reset(struct intConstructor* ic) {
  ic->char_rep[0] = '\0';
  ic->num_rep = 0;
  ic-> nu_digits = 0;
}

// return the total of all invalids between
long long intConstructor_findSumOfInvalids(struct intConstructor* lower, struct intConstructor* upper) {
  // check if *lower is currently invalid (xyz|xyz) (two repeating sequences)
  int return_flag = 0;
  while(lower->nu_digits <= upper->nu_digits) {
    bool possible = true;
    while (possible) {
      // god I need to figure this part out
    }
  }
  return return_flag;
}

int main(void) {
  FILE* f = fopen("input.txt", "r");
  char c;
  struct intConstructor* upper = NULL;
  struct intConstructor* lower = NULL;
  struct intConstructor** current = &lower;
  long long total = 0;
  while((c = fgetc(f)) != EOF) {
    if (c == '\n') {
      // nothing to do, end of file/line
    } else if (c == ',') {
      // end of range, set upper 
      printf("%s\tchar_limit: %d\tnum_rep: %lld\tnu_digits: %d\n", 
        upper->char_rep, upper->char_limit, upper->num_rep, upper->nu_digits);
      intConstructor_reset(upper);
      current = &lower;
      printf("\t>>> end\n");
      // we know the full range now.
      // TODO: do some operation with upper and lower
      // [abcd - wxyz]
      // construct increase on lower
      // check to make sure it's below upper
      // if so, add it to total
    } else if (c == '-') {
      // start of range, set lower
      printf("\t>>> start\n");
      printf("%s\tchar_limit: %d\tnum_rep: %lld\tnu_digits: %d\n", 
        lower->char_rep, lower->char_limit, lower->num_rep, lower->nu_digits);
      intConstructor_reset(lower);
      current = &upper;
    } else {
      // inside a range's value
      *current = (intConstructor_attachDigit(*current, ((int)c) - ZEROASCIIVAL));
    }
  }
  printf("total: %lld\n", total);
  return 0;
}
