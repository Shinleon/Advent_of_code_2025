#ifndef HASHUTILS_
#define HASHUTILS_

int hashEightBytesToFour(void* hashable);

// return positive if less > greater
//  0 if less == greater
//  negative is less < greater
int compareEightBytes(void* less, void* greater);

#endif
