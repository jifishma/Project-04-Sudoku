#include "../Project-04-Sudoku/Sudoku.h"
#include "../Project-04-FileOps/FileOps.h"

void CreateCreatorInputFile(Sudoku puzzle, ofstream& outs);

int main(int argc, char** argv)
{
	ifstream ins;
	ofstream outs;
	string inFileName, outFileName, outStatistic;

	Sudoku puzzle = Sudoku(outs);
	puzzle.state = State::Solving;

	// Display welcome message
	cout << "-----------Welcome to Sudoku Solver Program-----------" << endl;

	// Ask for and open the input and output files
	OpenFileStreams(ins, inFileName, outs, outStatistic);

	// Print welcome message to file
	outs << "-----------Welcome to Sudoku Solver Program-----------" << endl;

	// Read input file, and populate the puzzle accordingly
	PreparePuzzle(inFileName, puzzle, ins, outs);

	// Show the initial puzzle grid after loading
	cout << "Initial puzzle: " << endl;
	outs << "Initial puzzle: " << endl;
	puzzle.printGrid();

	// Solve the puzzle, showing iteratations of solved sub-grids as we progress
	puzzle.solver.solveSudokuWithTimer();

	// Once fully solved, print the complete puzzle grid
	cout << "Final puzzle: " << endl;
	outs << "Final puzzle: " << endl;
	puzzle.printGrid();

	// Once we get here, print out collected metrics from finding a solution
	puzzle.solver.printSolveMetrics();

	// Close the file streams
	CloseFileStreams(ins, outs);



	//ask for output file name
	cout << "Please enter output file name: ";
	cin >> outFileName;

	//open output file
	outs.open(outFileName);

	//create input file for Sudoku creator
	CreateCreatorInputFile(puzzle, outs);

	//close output file
	outs.close();

	system("pause");
	return 0;
}


/*
Function Name: CreateCreatorInputFile
Author Name: Chong Zhang
Creation Date: 08/05/2019
Modification Date: 08/17/2019
Purpose: create input file for Sudoku creator
*/

void CreateCreatorInputFile(Sudoku puzzle, ofstream& outs)
{
	for (int rowID = 1; rowID <= 9; ++rowID)
	{
		for (int colID = 1; colID <= 9; ++colID)
		{
			cout << rowID << " " << colID << " " << puzzle.getValue(rowID, colID) << endl;
			outs << rowID << " " << colID << " " << puzzle.getValue(rowID, colID) << endl;
		}
	}
}