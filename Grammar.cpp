#include <Windows.h>
#include "LinkedList.h"
#include "Grammar.h"
#include "Class_Generic.h"
#include "Create_object.h"
#include <stdio.h>
#include "Follow.h"


struct internals{
	int **Grammer;

	int countRows;
	int countNonterminals;
	int countTerminals;
	struct LinkedList **arrGrammerList;

};
struct internals _internals = {0, 0, 0, 0, 0};
void *internals = &_internals;

int IsNonTerminal(const struct Grammar *__ptrGramamr, int __intItem){
	struct internals *_ptrInternals = (struct internals *)__ptrGramamr->internals;
	if (__intItem <_ptrInternals->countNonterminals && __intItem >= 0){
		return 1;
	}
	else{
		return 0;
	}
}

int IsTerminal(const struct Grammar *__ptrGramamr, int __intItem){
	struct internals *_ptrInternals = (struct internals *)__ptrGramamr->internals;
	if (__intItem >=_ptrInternals->countNonterminals){
		return 1;
	}
	else{
		return 0;
	}
}


//Returns the __ItemIndex'th item of the produciton specified by __RowIndex. Returns -1 if not found
int GetItemInProduction(int **__Grammer, int __RowIndex, int __ColumnCount, int __ItemIndex){
	for (int i =0; i <__ColumnCount; i++){
		if (__Grammer[__RowIndex][i] == __ItemIndex){
			return i;
		}
	}
	return -1;
}

int * GetProduction(int **__Grammer, int __rowIndex, int __countColumns, int *__countOutput){
	int *_output = (int *)malloc(sizeof(int)*__countColumns);
	for (int i = 0; i < __countColumns; i++){
		_output[i] = GetItemInProduction(__Grammer, __rowIndex, __countColumns, i);
		if (_output[i] ==-1){
			*__countOutput = i;
			break;
		}
	}

	return _output;
}


struct LinkedList **GrammarToList(int **__Grammer, int __countRows, int __countColumns){
		int *_arrProduction;
		int _countOutput;
		int *Value;
		struct LinkedList **__lstOutput = (struct LinkedList **)malloc(sizeof(struct LinkedList *)*__countRows);
		struct LinkedList *_ptrNew;
		for (int i = 0; i < __countRows; i++){
			_arrProduction = GetProduction(__Grammer, i, __countColumns, &_countOutput);
			_ptrNew = (struct LinkedList *)Create_Object(LINKEDLIST);
			_ptrNew = _ptrNew->init(_ptrNew, sizeof(int));
			for (int j = 0; j < _countOutput;j++){
				Value = (int *)malloc(sizeof(int));
				*Value = _arrProduction[i];
				_ptrNew->Add((void *)&_arrProduction[j], _ptrNew);
				Value =(int *)_ptrNew->Head->Value;
			}
			__lstOutput[i] = _ptrNew;
		}
		return __lstOutput;
}

int IsEpsilon(int __intItem, struct Grammar * __ptrGrammar){
		struct internals *_ptrInternals = (struct internals *)__ptrGrammar->internals;
		if (__intItem == _ptrInternals->countNonterminals +_ptrInternals->countTerminals){
			return 1;
		}
		return 0;
}

int IsEndMarker(int __intItem, struct Grammar * __ptrGrammar){
		struct internals *_ptrInternals = (struct internals *)__ptrGrammar->internals;
		if (__intItem == _ptrInternals->countNonterminals +_ptrInternals->countTerminals+1){
			return 1;
		}
		return 0;
}



Grammar *init(struct Grammar *__ptrGrammar, int **__Grammar, int *__Rows, int __countRows, int __countNonterminals, int __countTerminals){

	struct internals *_ptrInternals ;
	
	_ptrInternals =	(struct internals *)__ptrGrammar->internals;
	_ptrInternals->Grammer = __Grammar;
	__ptrGrammar->Rows = __Rows;
	
	_ptrInternals->countRows = __countRows;
	_ptrInternals->countNonterminals=__countNonterminals;
	_ptrInternals->countTerminals = __countTerminals;

	__ptrGrammar->End_Marker = __countNonterminals +__countTerminals +1;
	_ptrInternals->arrGrammerList = GrammarToList(__Grammar, __countRows, __countNonterminals+__countTerminals+1);
	__ptrGrammar->First = (struct First *)Create_Object(FIRST);
	__ptrGrammar->First = __ptrGrammar->First->init(__ptrGrammar->First, __ptrGrammar, __countRows, __countNonterminals, __countTerminals); 
	__ptrGrammar->Follow = (struct Follow *)Create_Object(FOLLOW);
	__ptrGrammar->Follow = __ptrGrammar->Follow->init(__ptrGrammar->Follow, __ptrGrammar, __countRows, __countNonterminals, __countTerminals); 

	return __ptrGrammar;
}








void Print(struct Grammar *__ptrGrammar){
	struct internals * _ptrInternals = (struct internals *) __ptrGrammar->internals;
	int _countRows = _ptrInternals->countRows;

	struct LinkedList **lstGrammar = _ptrInternals->arrGrammerList;
	struct LinkedListNode *_ptrTemp;

	for (int i = 0; i < _countRows;i++){
		_ptrTemp = lstGrammar[i]->Head;
		int *Value;
		Value =(int *)_ptrTemp->Value;
		printf("%d ",*((int *)(_ptrTemp->Value)));
		while (_ptrTemp->Next->Next !=NULL){
			_ptrTemp = _ptrTemp->Next;
			printf("%d ", *((int *)_ptrTemp->Value));
			
		}
		printf("\n");
	}
}


struct LinkedList *GetProduction(const struct Grammar*__ptrGrammar, int __intProduction){
	struct internals * _ptrInternals = (struct internals *)__ptrGrammar->internals;
	return _ptrInternals->arrGrammerList[__intProduction];
}


struct LinkedList *GetRows(const struct Grammar*__ptrGrammar, int __intNonTerminal){
	struct internals * _ptrInternals = (struct internals *)__ptrGrammar->internals;
	struct LinkedList *_ptrOutput = (struct LinkedList *)Create_Object(LINKEDLIST);
	_ptrOutput = _ptrOutput->init(_ptrOutput, sizeof(int));

	int _countRows = _ptrInternals->countRows;
	for (int i =0; i<_countRows;i++){
		if (__ptrGrammar->Rows[i] ==__intNonTerminal){
			_ptrOutput->Add(&i, _ptrOutput);
		}	
	}
	return _ptrOutput;
}

int GetItemAfterDot(const struct Grammar *__ptrGramamr, int __intProduction, int __intDot){
	struct LinkedList *_ptrProduction =__ptrGramamr->GetProduction(__ptrGramamr, __intProduction);
	struct LinkedListNode *_ptrNode = _ptrProduction->Head;
	for (int i =0; i < __intDot ;i++){
		_ptrNode  = _ptrNode->Next;
	}
	if (_ptrNode->Next==NULL){
		return -1;
	}
	else{
		return *(int *)_ptrNode->Value;	
	}

}
int ProTokCount(struct Grammar *__ptrGrammar, int __intProd){
	struct internals *_ptrInternals = (struct internals *)__ptrGrammar->internals;
	struct LinkedListNode *_ptrHead = _ptrInternals->arrGrammerList[__intProd]->Head;
	int _intOutput = 0;
	while (_ptrHead->Next !=NULL){
		_intOutput++;
		_ptrHead =_ptrHead->Next ;
	}

	return _intOutput;
	
}

static void *c_ctor(){
	struct Grammar *_ptrGrammar = (struct Grammar *)malloc(sizeof(struct Grammar));
	_ptrGrammar->internals = (struct internals *)malloc(sizeof(struct internals *));
	_ptrGrammar->init = init;
	_ptrGrammar->Print =  Print;
	_ptrGrammar->GetProduction = GetProduction;
	_ptrGrammar->GetItemAfterDot = GetItemAfterDot;
	_ptrGrammar->IsNonTerminal = IsNonTerminal;
	_ptrGrammar->GetRows = GetRows;
	_ptrGrammar->IsTerminal = IsTerminal;
	_ptrGrammar->IsEpsilon = IsEpsilon;
		_ptrGrammar->IsEndMarker = IsEndMarker;
		_ptrGrammar->ProTokCount = ProTokCount;
	return _ptrGrammar;
}
struct Class_Generic _GRAMMAR = {c_ctor};
const void *GRAMMAR = &_GRAMMAR;
