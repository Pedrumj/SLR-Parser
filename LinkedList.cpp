#include "Class_Generic.h"
#include "LinkedList.h"
#include <Windows.h>

void Add( void * __ptrValue, struct LinkedList *__ptrList){

	struct LinkedListNode *_ptrHead = __ptrList->Head;
	struct LinkedListNode *_ptrNew = (struct LinkedListNode *)malloc(sizeof(struct LinkedListNode));
	_ptrNew->Value =NULL;
	_ptrNew->Next = NULL;


	while (_ptrHead->Next !=NULL){
		_ptrHead = _ptrHead->Next;	
	
	}
	_ptrHead->Value =malloc(__ptrList->Size);
	memcpy(_ptrHead->Value, __ptrValue,__ptrList->Size);
	_ptrHead->Next = _ptrNew;

}

struct LinkedList * init(struct LinkedList *_ptrInput, int __Size){

	_ptrInput->Size = __Size;

	return _ptrInput;
}

int AddUnique(void * __Value, struct LinkedList * const __ptrList, int (*Compare)(void *__ptr1, void *__ptr2)){
	int _flagFound =0;
	struct LinkedListNode *_ptrHead = __ptrList->Head;
	while (_ptrHead->Next !=NULL){
		if (Compare(_ptrHead->Value, __Value)==1){
			return 0;
		}
		_ptrHead=_ptrHead->Next;
	}

	if (_flagFound ==0){
		__ptrList->Add(__Value, __ptrList);
		return 1;
	}
}

 static void * c_ctor(){
	struct LinkedList *_ptrOutput = (struct LinkedList *)malloc(sizeof(struct LinkedList));
	struct LinkedListNode *_ptrNode = (struct LinkedListNode *)malloc(sizeof(struct LinkedListNode));
	_ptrNode->Value =NULL;
	_ptrNode->Next = NULL;
	_ptrOutput->Head = _ptrNode;
	_ptrOutput->init = init;
	_ptrOutput->Add = Add;

	_ptrOutput->AddUnique =  AddUnique;

	return _ptrOutput;
}

struct  Class_Generic _LINKEDLIST = {c_ctor};

const void *LINKEDLIST = &_LINKEDLIST;


