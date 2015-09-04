#include <Windows.h>
#include "LinkedList.h"
#include "Grammar.h"
#include "Class_Generic.h"
#include "Create_object.h"
#include <stdio.h>
#include "Follow.h"
#include "First.h"


struct internals{
	//struct Grammar *Grammar;
	int countRows;
	int countNonterminals;
	int countTerminals;
};


int CompareFollow(void *__ptr1, void *__ptr2){
	int _val1 = *(int *)__ptr1;
	int _val2 = *(int *)__ptr2;
	if (_val1 == _val2){
		return 1;
	}
	else{
		return 0;
	}
}

void Print_Follow(struct Follow *__ptrInput){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	struct LinkedListNode *_ptrPrFollow_Node;

	printf("Printing Follow");
	printf("\n");
	for (int i = 0; i< _ptrInternals->countNonterminals;i++){
		_ptrPrFollow_Node = __ptrInput->arrFollow[i]->Head;
		while (_ptrPrFollow_Node->Next !=NULL){
			 printf("%-4d", *(int *)_ptrPrFollow_Node->Value);
			 _ptrPrFollow_Node = _ptrPrFollow_Node->Next;
		}
		printf("\n");
	}
}


//Adds the items in the FOLLOW set of the production __intProduction, to the FOLLOW set of the nonterminal __intNonterminal
int AdFol_PrFirst(int __intNonterminal, int __intProduction, struct Follow *__ptrFollow, struct Grammar *__ptrGrammar){
	int _intNonTerminal_Pr = __ptrGrammar->Rows[__intProduction];
	struct LinkedListNode *_ptrPr_First = __ptrFollow->arrFollow[_intNonTerminal_Pr]->Head;	
	int _flagChanged =0;
	while (_ptrPr_First->Next !=NULL){
		_flagChanged += __ptrFollow->arrFollow[__intNonterminal]->AddUnique(_ptrPr_First->Value, __ptrFollow->arrFollow[__intNonterminal], 
			CompareFollow);
		_ptrPr_First =_ptrPr_First->Next;
	}


	return _flagChanged;
}


//Adds the items in the FOLLOW set of the __intNext nonterminal to the follow set of the __intNonterminal'th nontemrinal
int AdFol_ItNx_NTer(int __intNonterminal, int __intNextNonTerminal, struct Follow *__ptrFollow, struct Grammar *__ptrGrammar, 
					int *__flagEpsilong){
	struct LinkedListNode *_ptrFollowNode = __ptrFollow->arrFollow[__intNextNonTerminal]->Head;
	int _flagChanged =0;
	int _flagEpsilon = 0;
	int _intFollowValue;
	
	while (_ptrFollowNode->Next !=NULL){
		_intFollowValue = *(int *)_ptrFollowNode->Value;
		_flagEpsilon += __ptrGrammar->IsEpsilon(_intFollowValue, __ptrGrammar);
		_flagChanged += __ptrFollow->arrFollow[__intNonterminal]->AddUnique(&_intFollowValue,__ptrFollow->arrFollow[__intNonterminal], 
			CompareFollow);
		_ptrFollowNode = _ptrFollowNode->Next;
	}
	return _flagChanged;			
	
}


//Adds the items in the FOLLOW set of the __intNext item to the follow set of the __intNonterminal'th nontemrinal
int AdFol_ItNx(int __intNonterminal, int __intNext, struct Follow *__ptrFollow, struct Grammar *__ptrGrammar, 
			   int __intProduction){
	int _flagChanged =0;
	int _flagEpsilon;
	if (__ptrGrammar->IsTerminal(__ptrGrammar, __intNext) ==1){
		_flagChanged+= __ptrFollow->arrFollow[__intNonterminal]->AddUnique(&__intNext,
			__ptrFollow->arrFollow[__intNonterminal], CompareFollow);
	}
	else{
		_flagChanged +=AdFol_ItNx_NTer(__intNonterminal, __intNext, __ptrFollow, __ptrGrammar, &_flagEpsilon);
		if (_flagEpsilon !=0){
			_flagChanged+= AdFol_PrFirst(__intNonterminal, __intProduction, __ptrFollow, __ptrGrammar);
		}
				
	}
	return _flagChanged;
}

//Loops through the tokens in the __intProduction'th production until the nontemrinal __intNonterminal is found. it then 
//Adds all the items in the FOLLOW set of the following item to the FOLLOW set of the __intNonterminal'th nonterminal.
int AdFol_Pr(int __intProduction, int __intNonterminal, struct Follow *__ptrFollow, struct Grammar *__ptrGrammar){
	struct internals *_ptrInternal = (struct internals *)__ptrFollow->internals;
	struct LinkedListNode *_ptrPr_Node = __ptrGrammar->GetProduction(__ptrGrammar, __intProduction)->Head;
	int _intPrIt_Value;
	int _intPrIt_Value_Next;
	int _flagChanged =0;
	while (_ptrPr_Node->Next !=NULL){
		_intPrIt_Value = *(int *)_ptrPr_Node->Value;
		if (_intPrIt_Value == __intNonterminal){
			if (_ptrPr_Node->Next->Next!=NULL){
				_intPrIt_Value_Next =*(int *) _ptrPr_Node->Next->Value;
				_flagChanged += AdFol_ItNx(__intNonterminal, _intPrIt_Value_Next, __ptrFollow, __ptrGrammar, 
					__intProduction);
			}
			else{
				_flagChanged+=AdFol_PrFirst(__intNonterminal, __intProduction, __ptrFollow, __ptrGrammar);
			}
		}
		_ptrPr_Node= _ptrPr_Node->Next;
	}
	return _flagChanged;

}


//Adds the endmarker to the first nonterminal
int AddEm_S(struct Follow *__ptrInput, struct Grammar *__ptrGrammar){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	int _flagChanged =0;
	int _intEm =_ptrInternals->countNonterminals +_ptrInternals->countTerminals +1;
	_flagChanged +=__ptrInput->arrFollow[0]->AddUnique(&_intEm, __ptrInput->arrFollow[0], CompareFollow);
	return _flagChanged;
}

//Adds the follow valuues to the sets for each token in each production
int AddFollow(struct Follow *__ptrInput, struct Grammar *__ptrGrammar){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;

	int _flagChanged =0;

	for (int i =0; i < _ptrInternals->countNonterminals;i++){
		for (int j =0; j <_ptrInternals->countRows;j++){
			_flagChanged+=AdFol_Pr(j, i,__ptrInput,  __ptrGrammar);
		}
	}

	return _flagChanged;
}


//Allocates memory for the FOLLOW array
void InitFirstArray(struct Follow *__ptrInput, struct Grammar *__ptrGrammar){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;

	__ptrInput->arrFollow = (struct LinkedList **)malloc(sizeof(struct LinkedList *)*_ptrInternals->countNonterminals);
	for (int i =0;i < _ptrInternals->countNonterminals;i++){
		__ptrInput->arrFollow[i] = (struct LinkedList *)Create_Object(LINKEDLIST);
		__ptrInput->arrFollow[i] = __ptrInput->arrFollow[i]->init(__ptrInput->arrFollow[i], sizeof(int)); 
	} 
}



//Generates the FOLLOW sets for the nontemrinals
void GenerateFollow(struct Follow *__ptrInput, struct Grammar *__ptrGrammar){
		struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
		InitFirstArray(__ptrInput, __ptrGrammar);
		AddEm_S(__ptrInput,__ptrGrammar);
		while (AddFollow(__ptrInput, __ptrGrammar)!=0){

		}

}

struct Follow * init(struct Follow *__ptrFollow, struct Grammar *__ptrGrammar, int __countRows, int __countNonterminals, 
	int __countTerminals){
	struct internals *_ptrInternals ;
	
	_ptrInternals =	(struct internals *)__ptrFollow->internals;
	_ptrInternals->countNonterminals = __countNonterminals;
	_ptrInternals->countTerminals = __countTerminals;
	_ptrInternals->countRows = __countRows;
	
	GenerateFollow(__ptrFollow, __ptrGrammar);

	return __ptrFollow;
}


static void *c_ctor(){
	struct Follow *_ptrFollow = (struct Follow *)malloc(sizeof(struct Follow));
	_ptrFollow->internals = (struct internals *)malloc(sizeof(struct internals *));
	_ptrFollow->init = init;
	return _ptrFollow;
}
struct Class_Generic _FOLLOW = {c_ctor};
const void *FOLLOW = &_FOLLOW;
