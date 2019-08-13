#include "Sudoku.h"

#include <vector>
#include <fstream>

using namespace std;

int main(int argc, char** argv)
{
	Sudoku su = Sudoku();
	su.setValue(1, 1, 9);
	su.solver.solveSudoku();
	su.getValue(1, 1);
}