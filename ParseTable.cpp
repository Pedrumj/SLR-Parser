#include <Windows.h>
#include "Grammar.h"
#include "Create_object.h"
#include "Automaton.h"
#include "Class_Generic.h"
#include "ParseTable.h"
#include "Closure.h"
#include <stdio.h>
#include "Follow.h"

struct internals{
	struct Grammar *Grammar;
	struct  Automaton *Automaton;
	int **Action;
	int **Reduce;
	int **Accept;
	int **Goto;
	int RowCount;
	int NonTerminalCount;
	int TerminalCount;
};

 void PrintMatrix(int **__ptrInput, int __countRows, int __countColumns);
 void GenerateActionTable(struct ParseTable *__ptrInput);
 int GetClosureState(struct Closure* __ptrOutgoing, struct LinkedListNode *__ptrClosures);
 int IsTerminal(int __Token, int __NonTerminalCount, int __TerminalCount);
 int **InitiateMatrix( int __rowCount, int __colCount);
 int GetStateCount(struct LinkedListNode *__ptrClosures);

 void AddReduce(struct ParseTable *__ptrInput, int __intState, int __intProduction){
	struct internals *_ptrInternals  = (struct internals *)__ptrInput->internals;
	int _intNonterminal = _ptrInternals->Grammar->Rows[__intProduction];
	struct LinkedListNode *_ptrFollowA = _ptrInternals->Grammar->Follow->arrFollow[_intNonterminal]->Head;
	int _intFolValue;
	while (_ptrFollowA->Next !=NULL){
		_intFolValue= *(int *)_ptrFollowA->Value;
		if (_ptrInternals->Grammar->IsEndMarker(_intFolValue, _ptrInternals->Grammar) ==1){
			_ptrInternals->Reduce[__intState][_intFolValue-_ptrInternals->NonTerminalCount-1] =__intProduction;
		}
		else{
			_ptrInternals->Reduce[__intState][_intFolValue-_ptrInternals->NonTerminalCount	] =__intProduction;
		}
		_ptrFollowA = _ptrFollowA->Next;
	}
}
int IsDotEnd(struct ItemInClosure *__ptrClItem, struct Grammar *__ptrGrammar){
	if (__ptrGrammar->GetItemAfterDot(__ptrGrammar, __ptrClItem->intProduction, __ptrClItem->intDot)==-1){
		return 1;
	}
	else{
		return 0;
	}
}

void RedCl(struct Closure *__ptrClosure, struct ParseTable *__ptrInput){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	struct LinkedListNode *_ptrClItems;
	struct ItemInClosure *_ptrClItem;
	_ptrClItems = __ptrClosure->Items->Head;
	while (_ptrClItems->Next !=NULL){
		_ptrClItem = (struct ItemInClosure *)_ptrClItems->Value;
		if(IsDotEnd(_ptrClItem, _ptrInternals->Grammar) ==1){
			if (_ptrClItem->intProduction !=0){
				AddReduce(__ptrInput, __ptrClosure->State, _ptrClItem->intProduction);
			}
		}
		_ptrClItems = _ptrClItems->Next;
	}
}
void GenerateReduceTable(struct ParseTable *__ptrInput){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	struct LinkedListNode *_ptrClosures = _ptrInternals->Automaton->Closures->Head;
	struct Closure *_ptrClosure;

	_ptrInternals->Reduce = InitiateMatrix(GetStateCount(_ptrClosures), _ptrInternals->TerminalCount + 1);

	while (_ptrClosures->Next !=NULL){
		_ptrClosure = (struct Closure *)_ptrClosures->Value;
		RedCl(_ptrClosure, __ptrInput);
		_ptrClosures = _ptrClosures->Next;
	}

}


void GenerateAcceptTable(struct ParseTable *__ptrInput){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	struct LinkedListNode *_ptrClosures = _ptrInternals->Automaton->Closures->Head;
	struct Closure *_ptrClosure;
	struct LinkedListNode *_ptrLinks;
	struct Link *_ptrLink;

	_ptrInternals->Accept = InitiateMatrix(GetStateCount(_ptrClosures), _ptrInternals->TerminalCount + 1);

	while (_ptrClosures->Next !=NULL){
		_ptrClosure = (struct Closure *)_ptrClosures->Value;
		_ptrLinks = _ptrClosure->Links->Head;
		while (_ptrLinks->Next !=NULL){
			_ptrLink = (struct Link *)_ptrLinks->Value;
			if (_ptrLink->IsAccept ==1){
				_ptrInternals->Accept[_ptrClosure->State][_ptrInternals->Grammar->End_Marker-_ptrInternals->NonTerminalCount-1]=
					1;
			}			
			_ptrLinks = _ptrLinks->Next;
		}
		_ptrClosures = _ptrClosures->Next;

	}
}


void GenerateGotoTable(struct ParseTable *__ptrInput){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	struct LinkedListNode *_ptrClosures = _ptrInternals->Automaton->Closures->Head;
	struct Closure *_ptrClosure;
	struct LinkedListNode *_ptrLinks;
	struct Link *_ptrLink;
	int _state =0;
	_ptrInternals->Goto = InitiateMatrix(GetStateCount(_ptrClosures), _ptrInternals->NonTerminalCount -1);
	
	while (_ptrClosures->Next !=NULL){
		_ptrClosure = (struct Closure *)_ptrClosures->Value;
		_ptrLinks = _ptrClosure->Links->Head;
		while (_ptrLinks->Next !=NULL){
			_ptrLink = (struct Link *)_ptrLinks->Value;
			if (_ptrInternals->Grammar->IsNonTerminal(_ptrInternals->Grammar, _ptrLink->Token)==1){

				_ptrInternals->Goto[_ptrClosure->State][_ptrLink->Token-1]=_ptrLink->NextClosure->State;
			}
			_ptrLinks = _ptrLinks->Next;
		}
		_ptrClosures = _ptrClosures->Next;
	
	}
}

struct ParseTable * init(struct ParseTable *__ptrInput, int **__Grammar, int *__Rows, int __countRows, 
	int __countNonterminals, int __countTerminals){
	struct Grammar * _ptrGrammar = (struct Grammar *)Create_Object(GRAMMAR);
	struct Automaton *_ptrAutomaton = (struct Automaton *)Create_Object(AUTOMATON);
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	_ptrGrammar = _ptrGrammar->init(_ptrGrammar, __Grammar, __Rows, __countRows, __countNonterminals, __countTerminals);


	_ptrAutomaton = _ptrAutomaton->init(_ptrAutomaton, _ptrGrammar);
	
	_ptrInternals->Automaton = _ptrAutomaton;
	_ptrInternals->Grammar = _ptrGrammar;
	_ptrInternals->RowCount = __countRows;
	_ptrInternals->NonTerminalCount = __countNonterminals;
	_ptrInternals->TerminalCount = __countTerminals;
	GenerateActionTable(__ptrInput);
	GenerateReduceTable(__ptrInput);
	GenerateAcceptTable(__ptrInput);
	GenerateGotoTable(__ptrInput);

	
	return __ptrInput;
	}



void GenerateActionTable(struct ParseTable *__ptrInput){
	struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	struct LinkedListNode *_ptrClosures = _ptrInternals->Automaton->Closures->Head;
	struct Closure *_ptrClosure;
	struct LinkedListNode *_ptrLinks;
	struct Link *_ptrLink;
	int _state =0;
	_ptrInternals->Action = InitiateMatrix(GetStateCount(_ptrClosures), _ptrInternals->TerminalCount + 1);

	while (_ptrClosures->Next !=NULL){
		_ptrClosure = (struct Closure *)_ptrClosures->Value;
		_ptrLinks = _ptrClosure->Links->Head;
		while (_ptrLinks->Next !=NULL){
			_ptrLink = (struct Link *)_ptrLinks->Value;
			if (IsTerminal(_ptrLink->Token, _ptrInternals->NonTerminalCount, _ptrInternals->TerminalCount)==1){

				_ptrInternals->Action[_ptrClosure->State][_ptrLink->Token-_ptrInternals->NonTerminalCount]=
					_ptrLink->NextClosure->State;
			}
			_ptrLinks = _ptrLinks->Next;
		}
		_ptrClosures = _ptrClosures->Next;
		_state ++;
	}
}
int GetStateCount(struct LinkedListNode *__ptrClosures){
		int _state;
	_state =0;
	while (__ptrClosures->Next!=NULL){

		_state ++;
		__ptrClosures = __ptrClosures->Next;
	}
	return _state;
}

int IsTerminal(int __Token, int __NonTerminalCount, int __TerminalCount){
	if (__Token >= __NonTerminalCount && __Token <__TerminalCount+__NonTerminalCount){
		return 1;
	}
	else{
		return 0;
	}
}
int **InitiateMatrix( int __rowCount, int __colCount){
	int **_ptrOutput= (int **)malloc(sizeof(int *)*__rowCount);
	for (int i= 0; i< __rowCount; i++){
		_ptrOutput[i]= (int *)malloc(sizeof(int)*__colCount);
		for (int j =0; j< __colCount;j++){
			_ptrOutput[i][j] = -1;
		}
	}

	return _ptrOutput;
}

 void PrintMatrix(int **__ptrInput, int __countRows, int __countColumns){
	  printf("Printing Matrix\n");
	 for (int i = 0; i<__countRows;i++){
		 for (int j = 0; j < __countColumns;j++){
			printf("%4d", __ptrInput[i][j]);
		 }
		 printf("\n");
	 }
 }

 void NextGoto(struct ParseTable *__ptrParseTable, int __state, int *__nextState, int __reduceNTer){
	 struct internals *_ptrInternals = (struct internals *)__ptrParseTable->internals;
	*__nextState = _ptrInternals->Goto[__state][__reduceNTer-1];
 
 }

  void NextAc_Ter(struct ParseTable *__ptrParseTable, int __nextToken, int __state, int *__nextState, EnumActions *__actionType, 
	   int *__reduceNTer){
	
 }

 void NextAction(struct ParseTable *__ptrParseTable, int __nextToken, int __state, int *__nextState, EnumActions *__actionType, 
	 int *__reduceNTer, int *__reducePoC){
	struct internals *_ptrInternals = (struct internals *)__ptrParseTable->internals;
	int _colIndex;
	if (_ptrInternals->Grammar->IsEndMarker(__nextToken, _ptrInternals->Grammar)==1){
		_colIndex = __nextToken-1-_ptrInternals->NonTerminalCount;
	}
	else{
		_colIndex = __nextToken-_ptrInternals->NonTerminalCount;
	}
	if (_ptrInternals->Action[__state][_colIndex] != -1){
		*__actionType = EnumActions::SHIFT;
		*__nextState = _ptrInternals->Action[__state][_colIndex];
	}
	else if(_ptrInternals->Reduce[__state][_colIndex] != -1){
		*__actionType = EnumActions::REDUCE;
		*__reduceNTer = _ptrInternals->Grammar->Rows[_ptrInternals->Reduce[__state][_colIndex]];
		*__reducePoC = _ptrInternals->Grammar->ProTokCount(_ptrInternals->Grammar, _ptrInternals->Reduce[__state][_colIndex]);
	}
	else if(_ptrInternals->Accept[__state][_colIndex] != -1){
		*__actionType = EnumActions::ACCEPT;
	}
	else{
		*__actionType = EnumActions::ERROR_A;
	}
	
 }

  static void * c_ctor(){
	struct ParseTable *_ptrOutput = (struct ParseTable *)malloc(sizeof(struct ParseTable));
	struct internals *_ptrInternals  = (struct internals *)malloc(sizeof(struct internals));
	struct Automaton *_ptrAutomaton = (struct Automaton *)malloc(sizeof(struct Automaton));
	_ptrInternals->Automaton = _ptrAutomaton;
	_ptrOutput->internals = _ptrInternals;
	_ptrOutput->init = init;
	_ptrOutput->NextAction = NextAction;
	_ptrOutput->NextGoto = NextGoto;

	return _ptrOutput;
}
struct  Class_Generic _PARSETABLE = {c_ctor};

const void *PARSETABLE = &_PARSETABLE;
