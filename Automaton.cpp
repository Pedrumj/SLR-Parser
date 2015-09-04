#include "Automaton.h"
#include "LinkedList.h"
#include "Grammar.h"
#include "Create_object.h"
#include "Closure.h"
#include "Class_Generic.h"
#include <Windows.h>

struct internals{
	struct Grammar *Grammar;
};



//Searches all closures in the automaton for one with a kernel matching the input production. Returns null if 
//a match is not found
struct Closure *FindClosure(struct Automaton*__ptrInput, struct Link  *__ptrNewItem, 
	struct LinkedListNode *__ptrClosureLinks){
	struct LinkedListNode *_ptrClosures = (struct LinkedListNode *)__ptrInput->Closures->Head;
	struct Closure *_ptrClosure;
	struct Link *_ptrValue;

	//check current closure for any outoing links that have already matched a closure. This is 2 pervent creating
	//2 closures with the same state. If something is found in this loop the closure will have more than one kernel
	//item
	while (__ptrClosureLinks->Next !=NULL){
		_ptrValue = (struct Link *)__ptrClosureLinks->Value;
		if (_ptrValue->Token == __ptrNewItem->Token && _ptrValue->NextClosure !=NULL){
			return _ptrValue->NextClosure;
		}
		__ptrClosureLinks = __ptrClosureLinks->Next;	
	}

	//check other closures. 
	while (_ptrClosures->Next !=NULL){
		_ptrClosure = (struct Closure *)_ptrClosures->Value;
		if (_ptrClosure->MatchKernelItem(_ptrClosure, __ptrNewItem->NewItem)==1){
			return _ptrClosure;
		}
		_ptrClosures= _ptrClosures->Next;
	}
	return NULL;
}


//Generates closures for the automaton. 
void GenerateClosures(struct Automaton *__ptrInput){
	struct internals * _ptrInternals = (struct internals *)__ptrInput->internals;
	struct LinkedListNode *_ptrClosures;
	int _flagChanged;
	struct Closure *_ptrClosure;
	struct Link *_ptrLink;
	struct LinkedListNode *_ptrLinkNode;
	struct Closure *_ptrNextClosure,* _ptrNewClosure;
	int _state =1;
	_flagChanged = 1;

	while (_flagChanged !=0){
 		_flagChanged = 0;
		_ptrClosures = (struct LinkedListNode*)__ptrInput->Closures->Head;
		while (_ptrClosures->Next!=NULL){
			_ptrClosure = (struct Closure *)_ptrClosures->Value;
			_ptrLinkNode = _ptrClosure->Links->Head;
			while (_ptrLinkNode->Next!=NULL){
				_ptrLink = (struct Link *)_ptrLinkNode->Value;
				if (_ptrLink->IsAccept ==0){
					_ptrNextClosure = FindClosure(__ptrInput, _ptrLink, _ptrClosure->Links->Head);
					if (_ptrNextClosure!=NULL){
						_ptrLink->NextClosure = _ptrNextClosure;
						_flagChanged += _ptrNextClosure->AddItemToClosure(_ptrNextClosure, _ptrLink->NewItem->intProduction,
							_ptrLink->NewItem->intDot, 1);
					}
					else{
						_ptrNewClosure= (struct Closure *)Create_Object(CLOSURE);
						_ptrNewClosure = _ptrNewClosure->init(_ptrNewClosure, _ptrInternals->Grammar, _state);
						_state ++;
						_flagChanged += _ptrNewClosure->AddItemToClosure(_ptrNewClosure, _ptrLink->NewItem->intProduction,
							_ptrLink->NewItem->intDot, 1);
						__ptrInput->Closures->Add(_ptrNewClosure, __ptrInput->Closures);
						_ptrLink->NextClosure = _ptrNewClosure;
					
					}
				}
				
				_ptrLinkNode = _ptrLinkNode->Next;
			}
			_ptrClosures = _ptrClosures->Next;
		}

	}
	

}

struct Automaton *init(struct Automaton *__ptrInput, struct Grammar* __ptrGrammar ){
	struct internals * _ptrInternals = (struct internals *)__ptrInput->internals;
	struct Closure *_ptrClosure = (struct Closure*)Create_Object(CLOSURE);
	_ptrClosure  = _ptrClosure->init(_ptrClosure, __ptrGrammar, 0);
	_ptrClosure->AddItemToClosure(_ptrClosure, 0, 0, 1);
	_ptrInternals->Grammar = __ptrGrammar;
	__ptrInput->Closures->Add(_ptrClosure, __ptrInput->Closures);
	GenerateClosures(__ptrInput);
	return __ptrInput;
}





void Print(struct Automaton *__ptrInput){
	//struct internals *_ptrInternals = (struct internals *)__ptrInput->internals;
	struct LinkedListNode *_ptrClosures = (struct LinkedListNode *)__ptrInput->Closures->Head;
	struct Closure *_ptrClosure;
	while (_ptrClosures->Next !=NULL){
		_ptrClosure=(struct Closure*)_ptrClosures->Value;
		_ptrClosure->Print(_ptrClosure);
		_ptrClosures = _ptrClosures->Next;
	}
}

 static void * c_ctor(){
	struct Automaton *_ptrOutput = (struct Automaton *)malloc(sizeof(struct Automaton));
	struct internals *_ptrInternals  = (struct internals *)malloc(sizeof(struct internals));
	struct LinkedList *_ptrClosures = (struct LinkedList *)Create_Object(LINKEDLIST);
	_ptrClosures = _ptrClosures->init(_ptrClosures, sizeof(struct Closure));
	_ptrOutput->Closures = _ptrClosures;
	_ptrOutput->internals = _ptrInternals;
	_ptrOutput->init = init;
	_ptrOutput->Print =Print;
	return _ptrOutput;
}

struct  Class_Generic _AUTOMATON = {c_ctor};

const void *AUTOMATON = &_AUTOMATON;
