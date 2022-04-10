#include "stack.h"

void push(struct Stack *this, uint64_t val) { this->list[++this->index] = val; }

uint64_t pop(struct Stack *this) { return this->list[this->index--]; }

uint64_t at(struct Stack *this, int64_t idx) { return this->list[idx]; }

uint64_t top(struct Stack *this) { return this->list[this->index]; }

int64_t size(struct Stack *this) { return this->index + 1; }