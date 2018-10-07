#include <stdio.h>
#include <stdlib.h>

struct cell
{
	int r;
	int c;
};

int Puzzle[9][9];


//Function to encode data from given puzzle to corresponding extra clauses 
void EncodePuzzleData(char *filename)
{
	long long int count = 0; //To count the no. of extra clauses needed using the data given in puzzle
	char data;    //variable to read data from file

	//Read Puzzle data and append the corresponding encoding of the input to clauses in minisat input file
	
	FILE *readpuzzle = fopen(filename,"r");
	if(readpuzzle == NULL) {printf("\n\nNo such file exist. Considering empty sudoku\n\n");return;}
	FILE *minisatInput = fopen("minisat_inputFile.txt","w");

	fprintf(minisatInput,"p cnf 729 12393\n");   //Telling that it is in cnf and giving information about no. of clauses and variables

    //Reading data and appending clauses from given data
	for(int i=0;i<9;++i)
	{
		for(int j=0;j<9;++j)
		{
			fscanf(readpuzzle,"%c",&data);

			if(data != '.')	
			{
				fprintf(minisatInput, "%d 0\n",( (data-49)*81 + i*9 + j+1 ));
				count++;
				Puzzle[i][j]=data-48;
			}
			else Puzzle[i][j]=0;

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

//Function to write clauses that describes that every cell should have atleast one number
void CellCondition1()
{
	FILE *minisatInput = fopen("minisat_inputFile.txt","a");

	//Looping over every cell
	for(int r=0;r<9;++r)
	{
		for(int c=0;c<9;++c)
		{
			for(int d=0;d<9;++d)
				fprintf(minisatInput,"%d ", (d*81 + r*9 + c+1));    //Condition that every cell should contain atleast one digit
			fprintf(minisatInput,"0\n");
		}
	}

	fclose(minisatInput);
}

//Function to write clauses that tells that every cell should have atmost one number
void CellCondition2()
{
	FILE *minisatInput = fopen("minisat_inputFile.txt","a");

	//Looping over every cell
	for(int r=0;r<9;++r)
	{
		for(int c=0;c<9;++c)
		{
			for(int d1=0;d1<9;++d1)
			{
				for(int d2=d1+1;d2<9;++d2)
				{
					fprintf(minisatInput, "%d %d 0\n", -(d1*81 + r*9 + c+1) , -(d2*81 + r*9 + c+1));
				}
			}
		}
	}

	fclose(minisatInput);
}

//Function to write clauses that tells that no two cells in a row should have same digit
void RowCondition()
{
	FILE *minisatInput = fopen("minisat_inputFile.txt","a");

	//Looping over every digit and every row
	for(int d=0;d<9;++d)
	{
		for(int r=0;r<9;++r)
		{
			for(int c1=0;c1<9;++c1)
			{
				for(int c2=c1+1;c2<9;++c2)
				{
					fprintf(minisatInput, "%d %d 0\n", -(d*81 + r*9 + c1+1) , -(d*81 + r*9 + c2+1));
				}
			}
		}
	}

	fclose(minisatInput);
}

//Function to write clauses that tells that no two cells in a column should have same digit
void ColumnCondition()
{
	FILE *minisatInput = fopen("minisat_inputFile.txt","a");

	//Looping over every digit and every column
	for(int d=0;d<9;++d)
	{
		for(int c=0;c<9;++c)
		{
			for(int r1=0;r1<9;++r1)
			{
				for(int r2=r1+1;r2<9;++r2)
				{
					fprintf(minisatInput, "%d %d 0\n", -(d*81 + r1*9 + c+1) , -(d*81 + r2*9 + c+1));
				}
			}
		}
	}

	fclose(minisatInput);
}

//Function to write clauses that tells that no two cells in a box should have same digit
void BoxCondition()
{
	FILE *minisatInput = fopen("minisat_inputFile.txt","a");

	//Looping over all possible digits
	for(int d=0;d<9;++d)
	{
		for(int ri=0;ri<9;ri+=3)
		{
			for(int ci=0;ci<9;ci+=3)
			{
				struct cell box[9];
				for(int i=0;i<9;++i) box[i].r = ri + i/3,box[i].c = ci + i%3;

				for(int i=0;i<9;++i)
					for(int j=i+1;j<9;++j)
						fprintf(minisatInput,"%d %d 0\n", -(d*81 + box[i].r*9 + box[i].c+1), -(d*81 + box[j].r*9 + box[j].c+1));
			}
		}
	}

	fclose(minisatInput);
}

//Function to write clauses that tells that no two cells in a diagonal should have same digit
void DiagonalCondition()
{
	FILE *minisatInput = fopen("minisat_inputFile.txt","a");

	for(int d=0;d<9;++d)
	{
		for(int i=0;i<9;++i)
		{
			for(int j=i+1;j<9;++j)
			{
				//left top to right bottom diagonal
				fprintf(minisatInput,"%d %d 0\n", -(d*81 + i*9 + i+1), -(d*81 + j*9 + j+1));

				//Right top to left bottom diagonal
				fprintf(minisatInput,"%d %d 0\n", -(d*81 + i*9 + 8-i+1), -(d*81 + j*9 + 8-j+1));

			}
		}
	}  



	fclose(minisatInput);	
}



//Function to Convert returned output of minisat and print the solution of puzzle
void DecodeOutputToPuzzleSolution()
{
	char satisfiability[10];
	int data;

	FILE *solution = fopen("minisat_outputFile.txt","r");

	fgets(satisfiability,10,solution);
	if(satisfiability[0]!='U')
	{
		for(int i=0;i<729;++i)
		{
			fscanf(solution,"%d",&data);

			if(data>0)
			{
				Puzzle[((data-1)%81)/9][((data-1)%81)%9] = (data-1)/81 + 1;
			}
		}

		fclose(solution);

		//Printing the Solution
		for(int i=0;i<9;++i)
		{
			for(int j=0;j<9;++j)
			{
				printf("%d",Puzzle[i][j]);
				if(j!=8) printf(" ");
			}
			printf("\n");
		}
	}
	else printf("Solution doesn't exist\n");
}


int main(int argc, char *argv[])
{
	if(argc==1) return 0;
	EncodePuzzleData(argv[1]);
	CellCondition1();
	CellCondition2();
	RowCondition();
	ColumnCondition();
	BoxCondition();
	DiagonalCondition();

	system("minisat minisat_inputFile.txt minisat_outputFile.txt");

	DecodeOutputToPuzzleSolution();

	

	return 0;
}





