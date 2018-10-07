/*                     -------------------------------
                       |  A SUDOKU+ puzzle Generator |
                       -------------------------------
																	--- made by Aniket Sanghi as an assignment under Prof. Subhajit Roy

A sudoku+ is a sudoku puzzle with normal sudoku constraints added with a constraint that in each diagonal, no 2 cells can have same digit

HOW TO RUN:
-----------

						./sudokuPlusPuzzleCreator puzzle4.txt

where,  puzzle4.txt is a file where you want your new puzzle to be written down.



Preposition encoding:
--------------------

Preposition --- Aijk, where i represents the digit placed in jk(th) cell


Variables can be feeded in 1-729 number format so ijk converted to number by   (i-1)*81 + (j-1)*9 + k !

*/



//----------  Code Begins --------//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//A 2D array to store puzzle values
int Puzzle[9][9];


//Checking if a particular sudoku puzzle is solvable (A particular logic statement is satisfiable)
int satisfy()
{
	char satisfiability[10];

	//Read the miniSAT output file and read the first line which say about SAT or UNSAT
	FILE *solution = fopen("minisat_outputFile.txt","r");
	fgets(satisfiability,10,solution);
	fclose(solution);

	if(satisfiability[0]=='U') return 0;    //If it says UNSAT then return 0 (not Satisfiable) 
	else return 1;

}

//A function that will write our 2D array puzzle into the file following that . representation
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


//Function to Convert returned output of minisat in the 2D array format
void DecodeOutputToPuzzleSolution()
{
	char satisfiability[10];
	int data;

	FILE *solution = fopen("minisat_outputFile.txt","r");

	fgets(satisfiability,10,solution);      //Get satisfiability value
	if(satisfiability[0]!='U')
	{
		for(int i=0;i<729;++i)
		{
			fscanf(solution,"%d",&data);

			if(data>0)
			{
				Puzzle[((data-1)%81)/9][((data-1)%81)%9] = (data-1)/81 + 1;     //converting 1-729 values of variables to corresponding 2D array form
			}
		}

		fclose(solution);

	}
	else printf("Solution doesn't exist\n");
}



//Function to generate a random puzzle which is satisfiable (may have multiple solution) to get a initial filled sudoku+
void generateRandomPartOfSolvedPuzzle(char *filename)
{
	srand(time(0));                             //Setting the seed of rand function with respect to system time
	int x,c;
	c = rand() % 10 + 1;						//Choosing that how many numbers to be taken true initially (out of 729 variables)


	while(c--)
	{
		
		x = rand() % 1000;
		while(x>=729) x = rand() % 1000;				//Until we get a number less than 729

		Puzzle[((x)%81)/9][((x)%81)%9] = (x)/81 + 1;	//Decoding that number to give the puzzle grid its value

	}

	WritePuzzleInFile(filename);						//Write in the file which will be feeded to sudokuPlusSolver.c
	
}


//Generating A Random Solved Sudoku Puzzle using the Random solvable puzzle
void generateRandomSolvedPuzzle(char *filename)
{
	char execute[1000] = "./sudokuPlusSolver ";
	strcat(execute,filename);							// execute = command to run sudokuPlusSolver with the puzzle file

	//generate the Random sudoku puzzle 
	generateRandomPartOfSolvedPuzzle(filename);			
	system(execute);									//Try to solve it

	while(!satisfy())									//Until we get a puzzle that is solvable
	{

		for(int i=0;i<9;++i) for(int j=0;j<9;++j) Puzzle[i][j]=0;	//Re-initialise 2D array puzzle grid
	
		generateRandomPartOfSolvedPuzzle(filename);		//Again generate random Sudoku puzzle question until we get a solvable puzzle
		system(execute);
	}

	DecodeOutputToPuzzleSolution();						//Write the solved puzzle in the 2D grid and file 

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
			if(count==2) return count;					//We just need to check multiple solution so this limit of 2 is set. If you want actual figure then comment this line

			for(int i=0;i<729;++i)
			{
				fscanf(output,"%d",&data);				// Keep on writing negation of our answer to input file 
				fprintf(input,"%d ",-data);				// so as to get a new interpretation that gives true
				
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


//Function which checks if our current puzzle can further be reduced
int verify(char *filename)
{
	char execute[1000] = "./sudokuPlusSolver ";
	strcat(execute,filename);
	int answer = 1;
	int numberOfSolutions = 0;

	//Looping over all grid values
	for(int i=0;i<9;++i)
	{
		for(int j=0;j<9;++j)
		{
			if(Puzzle[i][j]!=0)								//Values which are not zero
			{
				int temp = Puzzle[i][j];
				Puzzle[i][j] = 0;							// Check
				WritePuzzleInFile(filename);				// If upon removing any value
				system(execute);							// The puzzle still give one solution
				numberOfSolutions = NumberOfSolutions();	// Then return 1 implying we can further shorten the puzzle
				if(numberOfSolutions == 1)					
					return 1;
				Puzzle[i][j] = temp;
				WritePuzzleInFile(filename);
			}
		}
	}
	// If it give multiple solution upon removing any value then return 0
	return 0;

}



//Function to generate minimal puzzle that has exactly 1 solution using our RandomSolvedPuzzle.
void generatePuzzle(char *filename)
{
	srand(time(0));
	int a,b,temp,numberOfSolutions;
	char execute[1000] = "./sudokuPlusSolver ";
	strcat(execute,filename);
	
	
	while(1)
	{
		a = 100;b = 100;
		while(a>8) a = rand() % 10;									// consider random row
		while(b>8) b = rand() % 10;									// And random column

		while(Puzzle[a][b]==0)										//Until at that value we have an element (1-9)
		{
			a = 100;b = 100;
			while(a>8) a = rand() % 10;
			while(b>8) b = rand() % 10;
		}

		//Remove that value from that cell
		temp = Puzzle[a][b];
		Puzzle[a][b] = 0;

		//Then check if now the puzzle still gives unique solution
		WritePuzzleInFile(filename);
		system(execute);
		numberOfSolutions = NumberOfSolutions();

		//If it doesn't give unique solution then don't remove this element and check if it is the final solution!?
		if(numberOfSolutions != 1)
		{
			 Puzzle[a][b] = temp;
			 if(!verify(filename)) break;
		}
		WritePuzzleInFile(filename);
	}
}


//Function to print puzzle
void printingPuzzle()
{
	printf("\n\n\n # The Randomly generated minimal sudoku+ puzzle with unique solution \n\n\n");
	for(int i=0;i<9;++i)
		{
			for(int j=0;j<9;++j)
			{
				if(Puzzle[i][j]==0)
					printf(".");
				else
					printf("%d",Puzzle[i][j]);
				if(j!=8) printf(" ");
			}
			printf("\n");
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

	//Output the puzzle
	printingPuzzle();

	return 0;

}