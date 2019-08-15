#include "../Project-04-Sudoku/Sudoku.h"
#include "../Project-04-FileOps/FileOps.h"

int main(int argc, char** argv)
{
	ifstream ins;
	ofstream outs;
	string inFileName, outStatistic;

	Sudoku puzzle = Sudoku(outs);

	// Fisplay welcome message
	cout << "-----------Welcome to Sudoku Solver Program-----------" << endl;
	outs << "-----------Welcome to Sudoku Solver Program-----------" << endl;

	// Ask for and open the input and output files
	OpenFileStreams(ins, inFileName, outs, outStatistic);

	// Read input file, and populate the puzzle accordingly
	PreparePuzzle(inFileName, puzzle, ins, outs);

	puzzle.solver.solveSudoku();

	puzzle.printGrid();

	puzzle.solver.printSolveMetrics();

	ins.close();
	outs.close();

	return 0;
}