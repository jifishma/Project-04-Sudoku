#include "Sudoku.h"

#pragma region Sudoku Methods
/*
Function Name: constructor with ofstream pointer
Author Name: Jeffrey Fishman
Creation Date: 08/12/2019
Modification Date: 08/12/2019
Purpose: initialize the grid with value NULL (0), and cache an out file stream for use with metrics
*/
Sudoku::Sudoku(ofstream& outStream)
{
	outs = &outStream;
	Sudoku();
}

Sudoku::Sudoku(const Sudoku& puzzle)
{
	operator=(puzzle);
}

Sudoku& Sudoku::operator=(const Sudoku& puzzle)
{
	// We'll always have a 9x9 grid initialized by our header declaration
	for (int rowID = 0; rowID <= 9; ++rowID)
	{
		for (int colID = 0; colID <= 9; ++colID)
		{
			this->grid[rowID][colID] = puzzle.grid[rowID][colID];
		}
	}

	this->outs = puzzle.outs;
	this->state = puzzle.state;

	if (puzzle.state == State::Creating)
		this->creator.setNumHints(this->creator.getNumHints());

	return *this;
}

/*
Function Name: constructor
Author Name: Chong Zhang, Jeffrey Fishman
Creation Date: 08/05/2019
Modification Date: 08/05/2019
Purpose: initialize the grid with value NULL (0)
*/
Sudoku::Sudoku()
{
	// We'll always have a 9x9 grid initialized by our header declaration
	for (int rowID = 0; rowID <= 9; ++rowID)
	{
		for (int colID = 0; colID <= 9; ++colID)
		{
			grid[rowID][colID] = NULL;
		}
	}
}

/*
Function Name: setGrid
Author Name: Chong Zhang
Creation Date: 08/06/2019
Modification Date: 08/07/2019
Purpose: assign grid with the given array
*/

void Sudoku::setGrid(int array[10][10])
{
	for (int rowID = 1; rowID <= 9; ++rowID)
	{
		for (int colID = 1; colID <= 9; ++colID)
		{
			grid[rowID][colID] = array[rowID][colID];
		}
	}
}

/*
Function Name: printGrid
Author Name: Chong Zhang, Jeffrey Fishman
Creation Date: 08/05/2019
Modification Date: 08/12/2019
Purpose: display current number of hints, current grid (unassigned location marked as X)
*/

void Sudoku::printGrid()
{
	if (state == State::Creating)
	{
		cout << "Number of hints: " << creator.getNumHints() << endl;
		*outs << "Number of hints: " << creator.getNumHints() << endl;
	}

	//display current grid
	for (int rowID = 1; rowID <= 9; ++rowID)
	{
		for (int colID = 1; colID <= 9; ++colID)
		{
			//unsigned location marked as X
			if (grid[rowID][colID] == NULL)
			{
				cout << "X ";
				*outs << "X ";
			}
			else
			{
				cout << grid[rowID][colID] << " ";
				*outs << grid[rowID][colID] << " ";
			}

			if (!(colID % 3) && colID != 9)
			{
				cout << "| ";
				*outs << "| ";
			}
		}

		if (!(rowID % 3) && rowID != 9)
		{
			cout << setw(21) << setfill('-') << endl << "-";
			*outs << setw(21) << setfill('-') << endl << "-";
		}

		cout << endl;
		*outs << endl;
	}
	cout << endl;
	*outs << endl;
}

/*
Function Name: getValue
Author Name: Chong Zhang
Creation Date: 08/07/2019
Modification Date: 08/07/2019
Purpose: return the value of the given location (rowID, colID)
*/

int Sudoku::getValue(int rowID, int colID) const
{
	return grid[rowID][colID];
}

/*
Function Name: setValue
Author Name: Chong Zhang
Creation Date: 08/07/2019
Modification Date: 08/07/2019
Purpose: set the value of the given location (rowID, colID)
*/

void Sudoku::setValue(int rowID, int colID, int value)
{
	grid[rowID][colID] = value;
}

/*
Function Name: validToPlace
Author Name: Chong Zhang
Creation Date: 08/07/2019
Modification Date: 08/07/2019
Purpose: return if the given value is valid to place in the given location (rowID, colID)
*/

bool Sudoku::validToPlace(int rowID, int colID, int value)
{
	return !solver.usedInRow(rowID, value) && !solver.usedInColumn(colID, value) && !solver.usedInSubGrid(rowID, colID, value);
}

/*
Function Name: equality operator overload
Author Name: Jeffrey Fishman
Creation Date: 08/13/2019
Modification Date: 08/13/2019
Purpose: check if the two puzzle grids are identical
*/
bool Sudoku::operator==(Sudoku& rhs) const
{
	for (int row = 1; row <= 9; ++row)
	{
		for (int col = 1; col <= 9; ++col)
		{
			if (this->getValue(row, col) != rhs.getValue(row, col))
			{
				return false;      //multiple solutions
			}
		}
	}
	return true;				//unique solution
}


/*
Function Name: equality operator overload
Author Name: Jeffrey Fishman
Creation Date: 08/13/2019
Modification Date: 08/13/2019
Purpose: check if the two puzzle grids are not identical
*/
bool Sudoku::operator!=(Sudoku& rhs) const
{
	return !operator==(rhs);
}
#pragma endregion

#pragma region Creator Methods
/*
Function Name: constructor
Author Name: Jeffrey Fishman
Creation Date: 08/13/2019
Modification Date: 08/13/2019
Purpose: explicit reference to outer Sudoku class
*/
Sudoku::Sudoku::Creator::Creator(Sudoku& parent)
	:sudoku(parent)
{ }

/*
Function Name: setNumHints
Author Name: Chong Zhang
Creation Date: 08/07/2019
Modification Date: 08/07/2019
Purpose: set numHints
*/

void Sudoku::Creator::setNumHints(int hints)
{
	numHints = hints;
}

/*
Function Name: getNumHints
Author Name: Chong Zhang
Creation Date: 08/07/2019
Modification Date: 08/07/2019
Purpose: return the number of hints
*/

int Sudoku::Creator::getNumHints() const
{
	return numHints;
}

/*
Function Name: incNumHints
Author Name: Chong Zhang
Creation Date: 08/07/2019
Modification Date: 08/07/2019
Purpose: increase the number of hints by one
*/

void Sudoku::Creator::incNumHints()
{
	numHints++;
}

/*
Function Name: decNumHints
Author Name: Chong Zhang
Creation Date: 08/07/2019
Modification Date: 08/07/2019
Purpose: decrease the number of hints by one
*/

void Sudoku::Creator::decNumHints()
{
	numHints--;
}
#pragma endregion

#pragma region Solver Methods
/*
Function Name: selectUnassignedLocation
Author Name: Chong Zhang, Jeffrey Fishman
Creation Date: 08/07/2019
Modification Date: 08/12/2019
Purpose: seach an unassigned location, if the grid has no empty location, return 0
*/

pair<int, int>* Sudoku::Solver::selectUnassignedLocation(int rowID, int colID) const
{
	while (rowID <= 9)
	{
		while (colID <= 9)
		{
			if (sudoku.grid[rowID][colID] == NULL)
				return &make_pair(rowID, colID);

			colID++;
		}
		colID = 1;
		rowID++;
	}

	return nullptr;    //no empty location
}

/*
Function Name: constructor
Author Name: Jeffrey Fishman
Creation Date: 08/13/2019
Modification Date: 08/13/2019
Purpose: explicit reference to outer class
*/
Sudoku::Sudoku::Solver::Solver(Sudoku& parent)
	:sudoku(parent)
{ }

/*
Function Name: usedInRow
Author Name: Chong Zhang, Jeffrey Fishman
Creation Date: 08/07/2019
Modification Date: 08/07/2019
Purpose: return if the given value (value) has been used in row
*/
bool Sudoku::Solver::usedInRow(int rowID, int value)
{
	for (int col = 1; col <= 9; ++col)
	{
		numComparisons++;
		if (sudoku.grid[rowID][col] == value)
			return true;    //value has been used in row 
	}
	return false;
}



/*
Function Name: usedInColumn
Author Name: Chong Zhang, Jeffrey Fishman
Creation Date: 08/07/2019
Modification Date: 08/07/2019
Purpose: return if the given value (value) has been used in column
*/

bool Sudoku::Solver::usedInColumn(int colID, int value)
{
	for (int row = 1; row <= 9; ++row)
	{
		numComparisons++;
		if (sudoku.grid[row][colID] == value)
			return true;    //value has been used in column 
	}
	return false;
}

/*
Function Name: usedInSubGrid
Author Name: Chong Zhang
Creation Date: 08/07/2019
Modification Date: 08/07/2019
Purpose: return if the given value (value) has been used in the sub-grid
*/

bool Sudoku::Solver::usedInSubGrid(int rowID, int colID, int value)
{
	int subGridStartRowID = ((rowID - 1) / 3) * 3;
	int subGridStartColID = ((colID - 1) / 3) * 3;

	for (int row = 1; row <= 3; ++row)
	{
		for (int col = 1; col <= 3; ++col)
		{
			numComparisons++;
			if (sudoku.grid[subGridStartRowID + row][subGridStartColID + col] == value)
				return true;    //value has been used in sub-grid 
		}
	}
	return false;
}

/*
Function Name: solveSudoku
Author Name: Chong Zhang, Jeffrey Fishman
Creation Date: 08/07/2019
Modification Date: 08/12/2019
Purpose: solve sudoku (start from value 1), if no solution, return false. Supports solving in two directions (1-9 and 9-1).
*/

bool Sudoku::Solver::solveSudoku(bool solveBackwards)
{
	// return a pair that points to the first empty cell of the puzzle
	pair<int, int>* cell = selectUnassignedLocation();

	// if we don't find any empty cells, we've succeeded and can exit safely
	if (cell == nullptr)
		return true;    //success

	int rowID = cell->first, colID = cell->second;
	int from = 1, to = 9, increment = 1;

	// If we plan to solve backwards, set the 'if' statement variable values appropriately
	if (solveBackwards)
	{
		from = 9;
		to = 1;
		increment = -1;
	}

	for (int value = from; solveBackwards ? value >= to : value <= to; value += increment)
	{
		// if we don't already have this value in the row, column, or sub-grid...
		if (sudoku.validToPlace(rowID, colID, value))
		{
			// tentatively place the value there, and continue solving the puzzle
			sudoku.grid[rowID][colID] = value;

			if (sudoku.state == State::Creating)
				sudoku.creator.incNumHints();

			// we still want to compute sub grid stats, so we won't short-circuit this block with the bool flag first
			if (computeCompletedSubGrid(rowID, colID) && sudoku.state == State::Solving)
			{
				cout << "Completed sub-grid with value " << value << " at (" << rowID << ", " << colID << ")" << endl;
				*sudoku.outs << "Completed sub-grid with value " << value << " at (" << rowID << ", " << colID << ")" << endl;

				sudoku.printGrid();
			}

			if (solveSudoku(solveBackwards))
				return true;                    // if we manage to make it here, we've solved the whole puzzle

			// If we made it here, out tentative value didn't work out, and we'll try the next one
			sudoku.grid[rowID][colID] = NULL;

			if (sudoku.state == State::Creating)
				sudoku.creator.decNumHints();
		}
	}

	// If we made it here, then we haven't chosen a correct tentative value somewhere previously, and the puzzle is in an unsolvable state
	Sudoku::Solver::numBacktracks++;
	return false;   //failure, start backtracking
}

bool Sudoku::Solver::solveSudokuWithTimer(bool solveBackwards)
{
	auto start = chrono::high_resolution_clock::now();
	bool result = solveSudoku(solveBackwards);
	auto end = chrono::high_resolution_clock::now();

	solveTimer = end - start;
	return result;
}

/*
Function Name: computeCompletedSubGrid
Author Name: Jeffrey Fishman
Creation Date: 08/12/2019
Modification Date: 08/12/2019
Purpose: return if given cell's subgrid is completely populated or not
*/

bool Sudoku::Solver::computeCompletedSubGrid(int rowID, int colID)
{
	int subGridStartRowID = ((rowID - 1) / 3) * 3;
	int subGridStartColID = ((colID - 1) / 3) * 3;

	for (int row = 1; row <= 3; ++row)
	{
		for (int col = 1; col <= 3; ++col)
		{
			int& cellValue = sudoku.grid[subGridStartRowID + row][subGridStartColID + col];
			if (cellValue == NULL)
				return false;
		}
	}

	numSubGridIterations++;
	return true;
}

/*
Function Name: printSolveMetrics
Author Name: Jeffrey Fishman
Creation Date: 08/12/2019
Modification Date: 08/14/2019
Purpose: print out all collected metrics while solving
*/

void Sudoku::Solver::printSolveMetrics()
{
	cout << "Solver stats: " << endl;
	*sudoku.outs << "Solver stats: " << endl;

	cout << "\tHints given: " << sudoku.creator.getNumHints() << endl;
	*sudoku.outs << "\tHints given: " << sudoku.creator.getNumHints() << endl;

	cout << "\tComparisons made: " << numComparisons << endl;
	*sudoku.outs << "\tComparisons made: " << numComparisons << endl;

	cout << "\tBacktracks completed: " << numBacktracks << endl;
	*sudoku.outs << "\tBacktracks completed: " << numBacktracks << endl;

	cout << "\tNumber of sub-grid iterations: " << numSubGridIterations << endl;
	*sudoku.outs << "\tNumber of sub-grid iterations: " << numSubGridIterations << endl;

	cout << "\tComparisons to Backtracks ratio: " << (numComparisons / (double)numBacktracks) << ":1" << endl;
	*sudoku.outs << "\tComparisons to Backtracks ratio: " << (numComparisons / (double)numBacktracks) << ":1" << endl;

	cout << "\tBacktracks to Hints ratio: " << (numBacktracks / (double)sudoku.creator.getNumHints()) << ":1" << endl;
	*sudoku.outs << "\tBacktracks to Hints ratio: " << (numBacktracks / (double)sudoku.creator.getNumHints()) << ":1" << endl;

	if (solveTimer.count())
	{
		cout << "\tAverage solve time: " << solveTimer.count() << " seconds" << endl;
		*sudoku.outs << "\tAverage solve time: " << solveTimer.count() << " seconds" << endl;
	}

	cout << endl;
	*sudoku.outs << endl;
}

/*
Function Name: getNumComparisons, getNumBacktracks, getNumSubGridIterations
Author Name: Chong Zhang
Creation Date: 08/16/2019
Modification Date: 08/16/2019
Purpose: return the number of comparisons, backtracks and sub-grid interations
*/

int Sudoku::Solver::getNumComparisons()
{
	return numComparisons;
}

int Sudoku::Solver::getNumBacktracks()
{
	return numBacktracks;
}

int Sudoku::Solver::getNumSubGridIterations()
{
	return numSubGridIterations;
}
#pragma endregion