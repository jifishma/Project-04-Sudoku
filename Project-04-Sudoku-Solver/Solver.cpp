#include "../Project-04-Sudoku/Sudoku.h"
#include "../Project-04-FileOps/FileOps.h"

int main(int argc, char** argv)
{
	ifstream ins;
	ofstream outs;
	string inFileName, outStatistic;

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

	system("pause");
	return 0;
}