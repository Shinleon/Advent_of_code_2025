#include <limits.h>
#include <stdio.h>

#define LOWER4BYTEMASK (0xFFFFFFFF)

int hashEightBytesToFour(void* hashable) {
  long h = *((long long*) hashable);
  int upper = (h >> 32) & LOWER4BYTEMASK;
  int lower = (h & LOWER4BYTEMASK);
  return lower ^ upper;
}

int compareEightBytes(void* less, void* greater) {
  // return less - greater
  long long preface = *((long long*)less) - *((long long*)greater);
  int sign = preface < 0 ? -1 : (preface > 0 ? 1: 0);
  preface = preface/INT_MAX;
  if (sign != 0) {
    preface += sign;
  }
  return preface;
}
