#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int Puzzle[9][9];


//Checking if a particular sudoku puzzle is solvable
int satisfy()
{
	char satisfiability[10];

	FILE *solution = fopen("minisat_outputFile.txt","r");
	fgets(satisfiability,10,solution);
	fclose(solution);

	if(satisfiability[0]=='U') return 0;
	else return 1;

}

//Write the puzzle in a file
void WritePuzzleInFile(char *filename)
{
	FILE *puzzle = fopen(filename, "w");
	for(int i=0;i<9;++i)
	{
		for(int j=0;j<9;++j)
		{
			if(Puzzle[i][j]>0)
				if(j==8)
					fprintf(puzzle,"%d",Puzzle[i][j]);
				else
					fprintf(puzzle,"%d ",Puzzle[i][j]);
			else
				if(j==8)
					fprintf(puzzle,".");
				else
					fprintf(puzzle,". ");
		}
		fprintf(puzzle,"\n");
	}
	fclose(puzzle);	
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

	}
	else printf("Solution doesn't exist\n");
}



//Function 
void generateRandomPartOfSolvedPuzzle(char *filename)
{
	srand(time(0));
	int x;
	int c;
	c = rand() % 10 + 1;


	while(c--)
	{
		
		x = rand() % 1000;
		while(x>=729) x = rand() % 1000;

		Puzzle[((x)%81)/9][((x)%81)%9] = (x)/81 + 1;

	}

	WritePuzzleInFile(filename);
	
}

//Generating A Random Solved Sudoku Puzzle
void generateRandomSolvedPuzzle(char *filename)
{
	char execute[1000] = "./sudokuPlusSolver ";
	strcat(execute,filename);

	generateRandomPartOfSolvedPuzzle(filename);
	system(execute);

	while(!satisfy())
	{

		for(int i=0;i<9;++i) for(int j=0;j<9;++j) Puzzle[i][j]=0;
	
		generateRandomPartOfSolvedPuzzle(filename);
		system(execute);
	}

	DecodeOutputToPuzzleSolution();

	WritePuzzleInFile(filename);

}



//Function to count the number of solutions of the given sudoku
int NumberOfSolutions()
{
	int count = 0;
	int satisfiability = 1;
	int data;
    char sat[10];

	while(satisfiability)
	{
		
		FILE *output = fopen("minisat_outputFile.txt","r");
		FILE *input = fopen("minisat_inputFile.txt","a");
		fgets(sat,10,output);
		if(sat[0]=='U') return count;
		else
		{
			if(count==2) return count;

			for(int i=0;i<729;++i)
			{
				fscanf(output,"%d",&data);
				fprintf(input,"%d ",-data);
				
			}
			fprintf(input,"0\n");
			
			count++;
			
			
			
		}
		
		fclose(input);
		fclose(output);
		
		// FILE *reinput = fopen("minisat_inputFile.txt","w");
		// fseek(reinput,9,SEEK_SET);
		// fprintf(reinput," %lld",12393+tempcount+count);
		// fclose(reinput);
		
		system("minisat minisat_inputFile.txt minisat_outputFile.txt");

	}

	return count;
}

//
int verify(char *filename)
{
	char execute[1000] = "./sudokuPlusSolver ";
	strcat(execute,filename);
	int answer = 1;
	int numberOfSolutions = 0;

	for(int i=0;i<9;++i)
	{
		for(int j=0;j<9;++j)
		{
			if(Puzzle[i][j]!=0)
			{
				int temp = Puzzle[i][j];
				Puzzle[i][j] = 0;
				WritePuzzleInFile(filename);
				system(execute);
				numberOfSolutions = NumberOfSolutions();
				if(numberOfSolutions == 1)
					return 1;
				Puzzle[i][j] = temp;
				WritePuzzleInFile(filename);
			}
		}
	}

	return 0;

}




void generatePuzzle(char *filename)
{
	int a,b,temp,numberOfSolutions;
	char execute[1000] = "./sudokuPlusSolver ";
	strcat(execute,filename);
	
	
	while(1)
	{
		a = 100;b = 100;
		while(a>8) a = rand() % 10;
		while(b>8) b = rand() % 10;

		while(Puzzle[a][b]==0)
		{
			a = 100;b = 100;
			while(a>8) a = rand() % 10;
			while(b>8) b = rand() % 10;
		}

		temp = Puzzle[a][b];
		Puzzle[a][b] = 0;
		WritePuzzleInFile(filename);
		system(execute);
		numberOfSolutions = NumberOfSolutions();
		printf("\n\n%d\n\n",numberOfSolutions);
		if(numberOfSolutions != 1)
		{
			 Puzzle[a][b] = temp;
			 if(!verify(filename)) break;
		}
		WritePuzzleInFile(filename);
	}
}


int main(int argc, char* argv[])
{
	//Storing argument as a different name, Just to make it more descriptive
	char filename[1000];
	strcpy(filename,argv[1]);

	//Initialising 2D array representing our NewPuzzle
	for(int i=0;i<9;++i) for(int j=0;j<9;++j) Puzzle[i][j]=0;

	//Generating a Random solved puzzle
	generateRandomSolvedPuzzle(filename);	

	//Generating a Question!
	generatePuzzle(filename);

	return 0;

}