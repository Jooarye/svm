#include <stdint.h>

#define SIZE 1000

#ifndef __H_STACK
#define __H_STACK

struct Stack {
  uint64_t list[SIZE];
  int64_t index;
};

void push(struct Stack *, uint64_t);
uint64_t pop(struct Stack *);

uint64_t at(struct Stack *, int64_t);
uint64_t top(struct Stack *);
int64_t size(struct Stack *);

#endif