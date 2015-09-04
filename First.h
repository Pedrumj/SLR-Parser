#ifndef	FIRST_H
#define	FIRST_H
#include "Grammar.h"
#include "LinkedList.h"

extern const void * FIRST;

struct First{
	void *internals;
	struct First * (*init)(struct First *__ptrFirst, struct Grammar *__ptrGrammar, int __countRows, int __countNonterminals, 
	int __countTerminals);
	//array of linked lists of First items. The length of the array is the number of rows in the grammar matrix
	struct LinkedList **arrFirst;
	void (*Print)(struct Grammar *__ptrGrammar);
	
};

#endif	FIRST_H
