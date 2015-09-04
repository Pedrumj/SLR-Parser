#ifndef	FOLLOW_H
#define	FOLLOW_H
#include "Grammar.h"
#include "LinkedList.h"

extern const void * FOLLOW;

struct Follow{
	void *internals;
	struct Follow * (*init)(struct Follow *__ptrFirst, struct Grammar *__ptrGrammar, int __countRows, int __countNonterminals, 
	int __countTerminals);
	//an array of linked list of FOLLOW items. the number of rows is the number of nonterminals
	struct LinkedList **arrFollow;
	void (*Print)(struct Grammar *__ptrGrammar);
	
};

#endif	FOLLOW_H
