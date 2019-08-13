#include "Sudoku.h"

#include <vector>
#include <fstream>

using namespace std;

int main(int argc, char** argv)
{
	ofstream out("output.txt");
	Sudoku su = Sudoku(&out);

	su.setValue(1, 1, 9);
	su.solver.solveSudoku();
	su.getValue(1, 1);

	su.printGrid();
}