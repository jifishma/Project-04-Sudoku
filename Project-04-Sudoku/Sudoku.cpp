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

	this->enableSubGridPrinting = puzzle.enableSubGridPrinting;
	this->outs = puzzle.outs;

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
	cout << creator.getNumHints() << endl;
	*outs << creator.getNumHints() << endl;

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
Function Name: printSubGrid
Author Name: Jeffrey Fishman
Creation Date: 08/12/2019
Modification Date: 08/12/2019
Purpose: display a completed subgrid. Cells not part of the subgrid are marked as X.
*/

void Sudoku::printSubGrid(int rID, int cID) const
{ 
	if (!enableSubGridPrinting)
		return;

	cout << "Subgrid completed: " << endl;
	*outs << "Subgrid completed: " << endl;

	for (int rowID = 1; rowID <= 9; ++rowID)
	{
		for (int colID = 1; colID <= 9; ++colID)
		{
			//unsigned location marked as X
			if (rowID > rID + 3 || colID > cID + 3 || rowID <= rID || colID <= cID )
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
	pair<int, int>* cell = selectUnassignedLocation();
	if (cell == nullptr)
		return true;    //success

	int rowID = cell->first, colID = cell->second;
	int from = 1, to = 9, increment = 1;

	if (solveBackwards)
	{
		from = 9;
		to = 1;
		increment = -1;
	}

	for (int value = from; solveBackwards ? value >= to : value <= to; value += increment)
	{
		if (sudoku.validToPlace(rowID, colID, value))
		{
			sudoku.grid[rowID][colID] = value;
			sudoku.creator.incNumHints();

			printCompletedSubGrid(rowID, colID);

			if (solveSudoku(solveBackwards))
				return true;

			sudoku.grid[rowID][colID] = NULL;   //failure, clear location and continue
			sudoku.creator.decNumHints();
		}
	}

	Sudoku::Solver::numBacktracks++;
	return false;   //failure, start backtracking
}

/*
Function Name: printCompletedSubGrid
Author Name: Jeffrey Fishman
Creation Date: 08/12/2019
Modification Date: 08/12/2019
Purpose: return if given cell's subgrid is completely populated or not
*/

bool Sudoku::Solver::printCompletedSubGrid(int rowID, int colID) const
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

	sudoku.printSubGrid(subGridStartRowID, subGridStartColID);
	return true;
}

void Sudoku::Solver::printSolveMetrics()
{
	cout << "Solver stats: " << endl;
	*sudoku.outs << "Solver stats: " << endl;

	cout << "\tComparisons made: " << numComparisons << endl;
	*sudoku.outs << "\tComparisons made: " << numComparisons << endl;

	cout << "\tBacktracks completed: " << numBacktracks << endl;
	*sudoku.outs << "\tBacktracks completed: " << numBacktracks << endl;

	cout << "\tAverage solve time:" << 0 << endl;
	*sudoku.outs << "\tAverage solve time:" << 0 << endl;

	cout << endl;
	*sudoku.outs << endl;
}
#pragma endregion
