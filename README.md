# Sudoku-plus
An assignment done under Prof. Subhajit Roy

A sudoku+ puzzle is a normal sudoku puzzle with an additional constraint that in each diagonal, no two cells can have the same digit.

## AIM
A program to solve a sudoku+ puzzle using miniSAT and another program to create a random minimal sudoku+ puzzle with a unique solution.

---
# Dependencies

- [miniSAT](http://minisat.se/)

# How To Run
- To solve a sudoku+ puzzle stored in puzzle.txt file

			 gcc -o sudokuPlusSolver sudokuPlusSolver.c	 

             ./sudokuPlusSolver puzzle.txt
             
- To create a random sudoku+ puzzle and store it in puzzle_1.txt
				
			 gcc -o sudokuPlusPuzzleCreator sudokuPlusPuzzleCreator.c
			 
             ./sudokuPlusPuzzleCreator puzzle_1.txt
             
---

# Algorithm

### Sudoku+ solver:

Converting the sudoku+ puzzle given to the corresponding DIMACS representation and feeding it to miniSAT.
    
### Sudoku+ generator: 
  
  At first generate a random sudoku solved puzzle by first choosing randomnly some random number of cells and filling them with random numbers and then feeding it to ./sodokuPlusSolver until you get a solved puzzle.
 
 Then from the Solved puzzle randomly remove values such that the sudoku+ still have a unique solution. Do this until the situation comes when removing any value gives multiple solutions.
             
             
