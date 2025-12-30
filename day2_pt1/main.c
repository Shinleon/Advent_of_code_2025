#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define ZEROASCIIVAL (48)

struct intConstructor {
  char* char_rep;
  int char_limit;
  long long num_rep;
  int nu_digits;
};

struct InvalidID {
  int upper;
  int lower;
  int nu_digits;
};

struct InvalidID* InvalidID_nextFromIC(struct intConstructor* ic) {
  // split ic in two
  //  Grab upper portion and grab lower portion
  char* splittable = ic->char_rep;
  int upper = 0;
  if (ic->nu_digits % 2 == 1) {
    upper = 1;
  }
  int lower = 0;
  for(int i = 0; i < ic->nu_digits; i++) {
    int* to_modify = (i < (ic->nu_digits)/2) ? &upper : &lower;
    *to_modify = *to_modify * 10 + ((int)ic->char_rep[i] - ZEROASCIIVAL);
  }
  struct InvalidID* ret = malloc(sizeof(struct InvalidID));
  ret->upper = upper;
  ret->lower = lower;
  ret->nu_digits = (ic->nu_digits+1)/2;
  if (ic->nu_digits % 2 == 1) {
    // we prefixed with 1 so we are always greater than ic->num_rep if we clone upper to lower
    ret->lower = ret->upper;
  } else {
    // we didn't add 1 so we might be less than lower if we clone upper to lower
    if (ret->upper < ret->lower) {
      ret->upper+=1;
    }
    ret->lower = ret->upper;
  }
  return ret;
}

struct InvalidID* InvalidID_nextFromInvalidID(struct InvalidID* id) {
  id->upper += 1;
  if(id->upper == pow(10, id->nu_digits)) {
    // need to go up digits
    id->nu_digits += 1;
  }
  id->lower = id->upper;
  return id;
}

long long InvalidID_getIntRepresentation(struct InvalidID* id) {
  return ((long long)id->upper * (long long)pow(10, id->nu_digits) + id->lower);
}

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
  long long result = 0;
  while(lower->nu_digits <= upper->nu_digits) {
    bool possible = true;
    struct InvalidID* _temp = InvalidID_nextFromIC(lower);
    while (possible) {
      printf("current Invalid id = \'%d%d\'\n", _temp->upper, _temp->lower);
      possible = false;
    }
  }
  return 0ll;
}

int main(void) {
  FILE* f = fopen("input.txt", "r");
  char c;
  struct intConstructor* upper = NULL;
  struct intConstructor* lower = NULL;
  struct intConstructor** current = &lower;
  long long total = 0;
  while((c = fgetc(f)) != EOF) {
    if (c == ',' || c == '\n') {
      // we know the full range: TODO, figure out invalids in range
      printf("we have found our range\n");
      printf("\tlower = %lld, upper = %lld\n", lower->num_rep, upper->num_rep);
      struct InvalidID* nextinvalid = InvalidID_nextFromIC(lower);
      long long intRepInvalid = InvalidID_getIntRepresentation(nextinvalid);
      printf("\tnext invalid = %lld, %lld\n", intRepInvalid, upper->num_rep - intRepInvalid);
      while (intRepInvalid < upper->num_rep) {
        total += intRepInvalid;
        printf("\t\t added invalid: %lld\n", intRepInvalid);
        nextinvalid = InvalidID_nextFromInvalidID(nextinvalid);
        intRepInvalid = InvalidID_getIntRepresentation(nextinvalid);
        printf("\tnext invalid = %lld, %lld\n", intRepInvalid, upper->num_rep - intRepInvalid);
      }
      intConstructor_reset(upper);
      intConstructor_reset(lower);
      current = &lower;
    } else if (c == '-') {
      current = &upper;
    } else {
      *current = (intConstructor_attachDigit(*current, ((int)c) - ZEROASCIIVAL));
    }
  }
  printf("total: %lld\n", total);
  return 0;
}
