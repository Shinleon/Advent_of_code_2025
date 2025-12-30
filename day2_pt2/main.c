#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "HashSet.h"
#include "utils.h"

#define ZEROASCIIVAL (48)
#define PRIMELIMIT (17)

struct intConstructor {
  char* char_rep;
  int char_limit;
  long long num_rep;
  int nu_digits;
};

struct InvalidID {
  int phrase;
  int nu_digits;
  int nu_reps;
};

struct PrimeGenerator {
  int* contents;
  int limit;
  int length;
};

struct InvalidIDPossibilities {
  int quantity;
  struct InvalidID* ids;
};

struct PrimeGenerator* PrimeGenerator_nextPrime(struct PrimeGenerator* g) {
  if (g == NULL) {
    g = malloc(sizeof(struct PrimeGenerator));
    g->limit = 100;
    g->contents = malloc(sizeof(int)*g->limit);
    g->length = 0;
  } else if (g->length == g->limit) {
    int* temp = g->contents;
    int n_limit = g->limit*2;
    g->contents = malloc(sizeof(int)*n_limit);
    for(int i = 0; i < g->length; i++) {
      g->contents[i] = temp[i];
    }
    g->limit = n_limit;
  }
  
  if (g->length == 0) {
    g->length = 1;
    g->contents[0] = 2;
  } else {
    bool prime_found = false;
    int to_check = g->contents[g->length - 1] + 1;
    while(!prime_found) {
      //check if prime,
      bool to_check_is_prime = true;
      int* factor = g->contents;
      while (*factor <= (int)sqrt((double)to_check)) {
        if (to_check % *factor == 0) {
          to_check_is_prime = false;
          break;
        }
        factor += 1;
      }
      if(!to_check_is_prime) {
        to_check++;
      } else {
        prime_found = true;
      }
    }
    g->contents[g->length] = to_check;
    g->length += 1;
  }
  return g;
}

struct PrimeGenerator* primeList = NULL;

struct InvalidIDPossibilities* InvalidID_nextFromIC(struct intConstructor* ic) {
  // we need to know how many digits are in ic
  // factorize no_digits, e.g. 12 digit no., needs 4 repetitions of 3 digits
  //                              && 3 repetitions of 4 digits
  while(primeList == NULL || primeList->length < PRIMELIMIT) {
    primeList = PrimeGenerator_nextPrime(primeList);
  }
  int* _t = malloc(sizeof(int)*PRIMELIMIT);
  int _index = 0;
  int nu_d = ic->nu_digits;
  for (int i = 0; i < PRIMELIMIT; i++) {
    while(nu_d != 1 && nu_d % primeList->contents[i] == 0) {
      nu_d = nu_d / primeList->contents[i];
      _t[i] += 1;
    }
  }
  for(int i = 0; i < PRIMELIMIT; i++) {
    if (_t[i] != 0) {
      printf("%d, %d |", primeList->contents[i], _t[i]);
    }
  }
  printf("\n");
  // we have the factorization in <_t> 
  // we can use that to gen possible Invalid ID's
  // TODO: each prime is a nu_reps, and quotient of ic->nu_digits/nu_reps = invalidId's->nu_digits

  //loop through <_t> and for each prime slot (2, 3, 5, 7, etc.) take all other prime factors
  // and create the other option (repetitions, length of repetition) (12-> (3,4) (2, 6))
  struct InvalidIDPossibilities* poss = malloc(sizeof(struct InvalidIDPossibilities)*PRIMELIMIT);
  for(int i = 0; i < PRIMELIMIT; i++) {
    if(_t[i] >= 1) {
      int repetitions = primeList->contents[i];
      int dpr = ic->nu_digits;
      dpr = dpr/repetitions;
      printf("possibility: %d repetition of  %d digit(s).\n", repetitions, dpr);
    }
  }
  return NULL;
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
  // TODO: adjust by nu_reps
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
      // TODO Stuff to do here
      // Factorize no_digits
      InvalidID_nextFromIC(upper);
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
