#include "Stdafx.h"
#include "Stack.h"
void Push(struct stack **__ptrHeader, int value){
	struct stack *_ptrNew = (struct stack *)malloc(sizeof(struct stack));
	_ptrNew->value = value;
	_ptrNew->previous = *__ptrHeader;
	*__ptrHeader  =_ptrNew;
}

struct stack *Pop(struct stack **__ptrHeader){
	struct stack *_ptrTemp = (struct stack *)malloc(sizeof(struct stack));
	_ptrTemp = *__ptrHeader;
	*__ptrHeader = (*__ptrHeader)->previous;
	free(_ptrTemp);
	_ptrTemp = NULL;
	
	return *__ptrHeader;
}