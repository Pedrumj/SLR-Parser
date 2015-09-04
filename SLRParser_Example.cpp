#include "Create_object.h"
#include <stdio.h>
#include <Windows.h>
#include "SLRParser.h"
#include "ParseTree.h"
//
//* The matGrammar used in this example is as below:
//E->E+T|T
//T->T*F|F
//F->(E)|id
//
//
//
//the input string is:
//"id+id*id"
//
//the output will be the parse tree
// 
//
//The Nonterminals are mapped as follow:
//[E,  0]
//[E', 1]
//[T,  2]
//[T', 3]
//[F,  4]
//
//
//The Terminals are mapped as follow:
//[+,    5]
//[*',   6]
//[id,   7]
//[(,    8]
//[),    9]
//[Eps, 10]
//[$,   11 ]
//
//For more information about how to use the project please see the documentation
//*/

int **InitializeMatrix(int __RowCount, int __ColCount);
void PrintMat(int **__matInput, int __RowCount, int __ColCount);


int main(){

//The matGrammar matrix
	int **matGrammar = InitializeMatrix(7, 10);
	//The index of the Nonterminal for each row of the matGrammar matrix
	int *Rows = (int *)malloc(sizeof(int)*7);

	int *_input = ((int *)malloc(sizeof(int)*5));
	struct ParseTree *_ptrResult;
	//E'->E
	matGrammar[0][1]=0;
	Rows[0] =0;
	//E->E+T
	matGrammar[1][1]=0;
	matGrammar[1][4]=1;
	matGrammar[1][2]=2;
	Rows[1] =1;
	//E->T
	matGrammar[2][2]=0;
	Rows[2] = 1;
	//T->T*F
	matGrammar[3][2]=0;
	matGrammar[3][5]=1;
	matGrammar[3][3]=2;
	Rows[3] =2;
	//T->F
	matGrammar[4][3]=0;
	Rows[4] =2;
	//F->(E)
	matGrammar[5][7]=0;
	matGrammar[5][1]=1;
	matGrammar[5][8]=2;
	Rows[5] = 3;
	//F->id
	matGrammar[6][6]=0;
	Rows[6] =3;

	//initialize input tokens
	//id
	_input[0] = 6;
	//+
	_input[1] = 4;
	//id
	_input[2] = 6;
	//*
	_input[3] = 5;
	//id
	_input[4] = 6;
	//$
	_input[5] = 10;

	


	struct SLRParser *_ptrParser = (struct SLRParser *)Create_Object(SLRPARSER);
	struct ParseTree *_ptrParseTree;
	_ptrParser = _ptrParser->init(_ptrParser, matGrammar, Rows, 7, 4, 5);
	_ptrParseTree=_ptrParser->Parse(_ptrParser, _input);
	_ptrParseTree->PrintTree(_ptrParseTree);

	return 0;
}

//Prints the input matrix
static void PrintMat(int **__matInput, int __RowCount, int __ColCount){
	char *_strPrint = (char *)malloc(sizeof(char) * 1000);
	char *_num2str =  (char *)malloc(sizeof(char) * 10);
	_num2str[0] = '\0';
	_strPrint[0] = '\0';
	for (int i = 0; i < __RowCount;i++){
		for (int j =0; j<__ColCount;j++){
			
			printf("%4d", __matInput[i][j]);
		}
		printf("\n");
	}
}

//Creates a dynamic 2d matrix and sets all the elements to -1
static int **InitializeMatrix(int __RowCount, int __ColCount){
	int **_matOutput = (int **)malloc(sizeof(int *)*__RowCount);
	for (int i = 0; i<__ColCount; i++){
		_matOutput[i] = (int *)malloc(sizeof(int )*__ColCount);
	}
	for (int i = 0; i< __RowCount; i++){
		for (int j = 0; j < __ColCount; j ++){
			_matOutput[i][j] = -1;
		}
	}
	return _matOutput;
}


