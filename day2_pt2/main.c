#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

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
  int quant_ids;
  int reserved_mem;
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

long long InvalidID_getIntRepresentation(struct InvalidID* id) {
  // TODO : adjust by nu_reps
  long long result = 0;
  for(int i = 0; i < id->nu_reps; i++){
    result = result * (long long)pow(10, id->nu_digits) + id->phrase;
  }
  return result;
}

struct InvalidIDPossibilities* InvalidIDPossibilities_FromIC(struct intConstructor* lower, struct intConstructor* upper) {
  // we need to know how many digits are in lower
  // factorize no_digits, e.g. 12 digit no., needs 4 repetitions of 3 digits
  //                              && 3 repetitions of 4 digits
  while(primeList == NULL || primeList->length < PRIMELIMIT) {
    primeList = PrimeGenerator_nextPrime(primeList);
  }
  int nu_d = lower->nu_digits;
  struct InvalidIDPossibilities* poss = malloc(sizeof(struct InvalidIDPossibilities)*PRIMELIMIT);
  poss->reserved_mem = PRIMELIMIT;
  poss->quant_ids = 0;
  struct InvalidID* to_add = malloc(sizeof(struct InvalidID)*PRIMELIMIT);
  poss->ids = to_add;
  while (nu_d <= upper->nu_digits) {
    // TODO: get prime decomp of nu_d
    // if nu_d == lower->nu_digits, lock to min value > lower
    // else start with 1*10^(nu_d/reps-1)
    // add to poss
    // increase nu_d until condition nu_d > upper->nu_digits
    int* _t = malloc(sizeof(int)*PRIMELIMIT);
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
    // TODO: each prime is a nu_reps, and quotient of lower->nu_digits/nu_reps = invalidId's->nu_digits
  
    //loop through <_t> and for each prime slot (2, 3, 5, 7, etc.) take all other prime factors
    // and create the other option (repetitions, length of repetition) (12-> (3,4) (2, 6))
    for(int i = 0; i < PRIMELIMIT; i++) {
      if(_t[i] >= 1) {
        int repetitions = primeList->contents[i];
        int dpr = nu_d;
        dpr = dpr/repetitions;
        printf("possibility: %d repetition of  %d digit(s).\n", repetitions, dpr);
        struct InvalidID* curr_id = poss->ids + poss->quant_ids;
        for(int _ic_idx = 0; _ic_idx < dpr; _ic_idx++) {
          curr_id->phrase = curr_id->phrase * 10 + ((int)lower->char_rep[_ic_idx] - ZEROASCIIVAL);
        }
        curr_id->nu_digits = dpr;
        curr_id->nu_reps = repetitions;
        if (InvalidID_getIntRepresentation(curr_id) < lower->num_rep) {
          curr_id->phrase += 1;
        }
        poss->quant_ids += 1; // we've added one item to our poss struct
      }
    }
    free(_t);
    nu_d++;
  }  
  return poss;
}

struct InvalidID* InvalidID_nextFromInvalidID(struct InvalidID* id) {
  id->phrase += 1;
  if(id->phrase == pow(10, id->nu_digits)) {
    // need to go up digits
    id->nu_digits += 1;
  }
  return id;
}

void InvalidID_incrementPhrase(struct InvalidID* id) {
  if (id->phrase != 0) {
    int x = id->phrase;
    bool allNines = true;
    while (x > 0) {
      int _ = x % 10;
      allNines = allNines && (_ == 9);
      if (!allNines){
        break;
      }
      x = x/10;
    }
    if (allNines) {
      id->nu_digits += 1;
    }
  } else {
    id->nu_digits = 1;
  }
  id->phrase += 1;
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

int InvalidID_allSameDigit(struct InvalidID* id) {
  int warp = id->phrase;
  int ret = -2;
  while (warp > 0) {
    int skim = warp % 10;
    if (ret == -2) {
      ret = skim;
    } else {
      if (ret != skim) {
        ret = -1;
        break;
      }
    }
    warp /= 10;
  }
  return (ret == -2 ? -1: ret);
}

// return the total of all invalids between
 long long InvalidIDPossibilities_findSumOfInvalids(struct InvalidIDPossibilities* poss, struct intConstructor* upper) {
  // loop through poss and keep increasing phrase and acquiring invID_getIntRep to sum as long as 
  //   less than IC upper
  long long total = 0;
  for(int i = 0; i < poss->quant_ids; i++){
    struct InvalidID* moving = poss->ids + i;
    long long _u = upper->num_rep;
    long long _m = InvalidID_getIntRepresentation(moving);
    bool allSameDigitSeen[10];
    while( _m < _u){
      int seen_digit = InvalidID_allSameDigit(moving);
      if (seen_digit == -1) {
        printf("\t\tadding in %lld, nu_digits: %d, reps: %d\n", _m, moving->nu_digits, moving->nu_reps);
        total += _m;
      } else {
        if(allSameDigitSeen[seen_digit] == false) {
          printf("\t\tadding in %lld, nu_digits: %d, reps: %d\n", _m, moving->nu_digits, moving->nu_reps);
          total += _m;
          allSameDigitSeen[seen_digit] = true;
        } else {
          printf("\t\tskipping %lld\n", _m);
        }
      }
      InvalidID_incrementPhrase(moving);
      _m = InvalidID_getIntRepresentation(moving);
    }
  }
  return total;
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
      if (lower->nu_digits != upper->nu_digits) {
        printf("\tthere is a digit change from %d to %d digits\n", lower->nu_digits, upper->nu_digits);
      }
      // TODO Stuff to do here
      // Factorize no_digits
      struct InvalidIDPossibilities* poss = InvalidIDPossibilities_FromIC(lower, upper);
      for(int i = 0; i < poss->quant_ids; i++) {
        printf("\t\tpossibility: %lld\n", InvalidID_getIntRepresentation(poss->ids+i));
      }
      total += InvalidIDPossibilities_findSumOfInvalids(poss, upper);
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
