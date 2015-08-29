#include "Stdafx.h"
#include <Windows.h>


struct stack{
struct stack *previous;
int value;
};

void Push(struct stack **__ptrHeader, int value);
struct stack *Pop(struct stack **__ptrHeader);
