#include "stdafx.h"
#include "Stack.h"




/* The grammer used in this example is as below:
E->TE'
E'->+TE'|Eps
T->FT'
T'->*FT'|Eps
F->(E)|id

the input string is:
"id+id*id"

the output will be the parse tree


The Nonterminals are mapped as follow:
[E,  0]
[E', 1]
[T,  2]
[T', 3]
[F,  4]


The Terminals are mapped as follow:
[+,    5]
[*',   6]
[id,   7]
[(,    8]
[),    9]
[Eps, 10]
[$,   11]

For more information about how to use the project please see the documentation
*/

int **InitializeMatrix(int __RowCount, int __ColCount);
int main(){

//The grammer matrix
	int **Grammer = InitializeMatrix(8, 11);
	//The index of the Nonterminal for each row of the grammer matrix
	int *Rows = (int *)malloc(sizeof(int)*8);

	int *_input = ((int *)malloc(sizeof(int)*5));
	struct ParseTree *_ptrResult;
	//E->TE'
	Grammer[0][1]=1;
	Grammer[0][2]=0;
	Rows[0] =0;
	//E'->+TE'
	Grammer[1][1]=2;
	Grammer[1][2]=1;
	Grammer[1][5]=0;
	Rows[1] =1;
	//E'->Eps
	Grammer[2][10]=0;
	Rows[2] = 1;
	//T->FT'
	Grammer[3][3]=1;
	Grammer[3][4]=0;
	Rows[3] =2;
	//T'->*FT'
	Grammer[4][3]=2;
	Grammer[4][4]=1;
	Grammer[4][6]=0;
	Rows[4] =3;
	//T'->Eps
	Grammer[5][10]=0;
	Rows[5] = 3;
	//F->(E)
	Grammer[6][0]=1;
	Grammer[6][8]=0;
	Grammer[6][9]=2;
	Rows[6] =4;
	//F->id
	Grammer[7][7]=0;
	Rows[7] =4;

	//initialize input tokens
	//id
	_input[0] = 7;
	//+
	_input[1] = 5;
	//id
	_input[2] = 7;
	///*
	_input[3] = 6;
	//id
	_input[4] = 7;

	_ptrResult=Parse(Grammer, Rows, 8, 5, 5, _input, 5);
	PrintTree(_ptrResult);
	return 0;
}

//Prints the input matrix
void PrintMat(int **__matInput, int __RowCount, int __ColCount){
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
int **InitializeMatrix(int __RowCount, int __ColCount){
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