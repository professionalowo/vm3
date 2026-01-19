#include <assert.h>
#include <stdlib.h>

#include "stack.h"

struct stack *stack_new (void) {
  struct stack *stack = malloc (sizeof *stack);
  assert(stack != NULL);
  stack->vals = NULL;
  stack->size = 0;

  return stack;
}

void stack_push (struct stack *stack, stackval_t val) {
  stack->vals = realloc(stack->vals, (stack->size + 1) * sizeof (stackval_t));
  assert(stack->vals != NULL);
  stack->vals[stack->size++] = val;
}

void stack_set (struct stack *stack, int pos, stackval_t val) {
  assert(stack->size > pos);
  stack->vals[pos] = val;
}

stackval_t stack_pop (struct stack *stack) {
  assert(stack->size > 0);
  return stack->vals[--stack->size];
}

stackval_t stack_peek (struct stack *stack) {
  assert(stack->size > 0);
  return stack->vals[stack->size - 1];
}

int stack_size (struct stack *stack) {
  return stack->size;
}

int stack_empty (struct stack *stack) {
  return stack_size(stack) == 0;
}

void stack_free (struct stack *stack) {
  free(stack->vals);
  free(stack);
}

