#ifndef _STACK_H
#define _STACK_H

typedef int stackval_t;

struct stack {
  stackval_t *vals;
  int size;
};



struct stack   *stack_new  (void);
void       stack_push (struct stack *stack, stackval_t val);
void       stack_set  (struct stack *stack, int pos, stackval_t val);
stackval_t stack_pop  (struct stack *stack);
stackval_t stack_peek (struct stack *stack);
void       stack_free (struct stack *stack);
int        stack_size (struct stack *stack);
int        stack_empty(struct stack *stack);


#endif /* _STACK_H */
