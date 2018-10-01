#include <stdio.h>
#include <stdlib.h>

//Function to encode data from given puzzle to corresponding extra clauses 
void EncodePuzzleData()
{
	long long int count = 0; //To count the no. of extra clauses needed using the data given in puzzle
	char data;    //variable to read data from file

	//Read Puzzle data and append the corresponding encoding of the input to clauses in minisat input file
	FILE *readpuzzle = fopen("puzzle_1.txt","r");
	FILE *minisatInput = fopen("minisat_inputFile.txt","w");

	fprintf(minisatInput,"p cnf 729 12393\n");   //Telling that it is in cnf and giving information about no. of clauses and variables

    //Reading data and appending clauses from given data
	for(int i=0;i<9;++i)
	{
		for(int j=0;j<9;++j)
		{
			fscanf(readpuzzle,"%c",&data);

			if(data != '.')		fprintf(minisatInput, "%d\n",( (data-49)*81 + i*9 + j+1 )),count++;

			fscanf(readpuzzle,"%c",&data);
		}
	}
	//Editing the number of clauses based on extra clauses from given data
	fseek(minisatInput,9,SEEK_SET);
	fprintf(minisatInput," %lld",12393+count);

	//Closing files
	fclose(readpuzzle);
	fclose(minisatInput);
}


int main()
{
	EncodePuzzleData();
	CellConditions();
	RowCondition();
	ColumnCondition();
	BoxCondition();
	DiagonalCondition();
}