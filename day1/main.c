#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define START_VALUE (50)

struct intArrList {
  struct intArrNode* head;
  struct intArrNode* tail;
  int length;
};

struct intArrNode {
  struct intArrNode* next;
  int data;
};

struct intArrList* append_intArrList(struct intArrList* arrlist, int to_append) {
  struct intArrNode* node_to_append = malloc(sizeof(struct intArrNode));
  node_to_append->data = to_append;
  if (arrlist == NULL) {
    arrlist = malloc(sizeof(struct intArrList));
    arrlist->head = node_to_append;
    arrlist->tail = node_to_append;
  } else {
    arrlist->tail->next = node_to_append;
    arrlist->tail = arrlist->tail->next;
  }
  arrlist->length += 1;
  return arrlist;
}

//TODO: implement a string output;
char* toStr_intArrList(struct intArrList* arrlist) {
  char* char_result = NULL;
  return char_result;
}

void free_intArrList(struct intArrList* arrlist) {
  if(arrlist == NULL) {
    return;
  }
  struct intArrNode* head = arrlist->head;
  while (head != NULL) {
    struct intArrNode* next = head->next;
    free(head);
    head = next;
  }
  free(arrlist);
}

struct intArrList* processInputfile(FILE* f) {
  char line[256]; // store a line
  struct intArrList* result = NULL;
  while(fgets(line, sizeof(line), f)) {
    int char_index = 0;
    int multiplier = 1;
    int total = 0;
    // L/R decision
    multiplier = (line[char_index] == 'L') ? -1 : 1 ;
    char_index += 1; //past the L/R decision
    while(line[char_index] != '\0' && line[char_index] != '\n') {
      total = total * 10 + (((int)line[char_index])  - 48 );
      char_index += 1;
    }
    total *= multiplier;
    result = append_intArrList(result, total);
  }
  return result;
}

int main(int argc, char** argv) {
  FILE* f = fopen("input.txt", "r");
  if (f == NULL) {
    fprintf(stderr, "Input file not able to be opened");
    return -1;
  }
  struct intArrList* rotations = processInputfile(f);
  int start = 50;
  struct intArrNode* head = rotations->head;
  int zero_trigger = 0;
  while(head != NULL) {
    int to_do = head->data;
    // start is greater than or equal to 0 and less than 100;
    int over_rot = abs(to_do)/100;
    int remainder = (to_do < 0) ? -1*(abs(to_do) % 100) : to_do % 100;
    zero_trigger += over_rot;
    if (start != 0 && (start + remainder <= 0 || start + remainder >= 100)) {
      zero_trigger += 1;
    }
    start = (start + remainder + 100) % 100;
    head = head->next;
  }
  printf("day 1: zero_trigger: %d\n", zero_trigger);
  free_intArrList(rotations);
  fclose(f);
}

