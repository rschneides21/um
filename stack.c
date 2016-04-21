


#include "stack.h"
#include "stdbool.h"
#include "stdlib.h"
#include "assert.h"
#include "stdio.h"


struct Stack_T {
	int top;
	int max_size;
	unsigned arr[];
};

 bool Stack_is_empty(Stack_T stack){
	if(stack->top < 0){
		return true;
	}
	else
		return false;
}

static inline bool Stack_is_full(Stack_T stack){
	if(stack->top == stack->max_size - 1) {
		return true;
	}
	else
		return false;
}

static inline Stack_T Stack_resize(Stack_T stack){
	int max_size = stack->max_size; 
	Stack_T new_stack = Stack_init(2 * max_size);
	new_stack->max_size = max_size * 2;
	new_stack->top = stack->top;

	for(int i = 0; i < stack->top + 1; i++){
		new_stack->arr[i] = stack->arr[i];
	}

	Stack_free(stack);
	return new_stack;
}

 Stack_T Stack_init(int max_size){
	Stack_T stack = malloc(sizeof(*stack) + (max_size * sizeof(unsigned)));
	assert(stack != NULL);

	stack->top = -1;
	stack->max_size = max_size;
	return stack;
}
 unsigned Stack_pop(Stack_T stack){
	assert(Stack_is_empty(stack) == false);
	return stack->arr[stack->top--];
}

 Stack_T Stack_push(Stack_T stack, unsigned val){
	if(Stack_is_full(stack)){
		Stack_T new_stack = Stack_resize(stack);
		new_stack->arr[++new_stack->top] = val;
		return new_stack;
	}
	else{
		stack->arr[++stack->top] = val;
		return stack;
	}
}

 void Stack_free(Stack_T stack){
	free(stack);
}
