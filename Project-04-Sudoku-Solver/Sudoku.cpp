
#include "Sudoku.h"

// Initialize our static external 
// symbols from our header
int Sudoku::grid[10][10];
int Creator::numHints;

/*
Function Name: construcor
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

void Sudoku::setGrid(int** array)
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

void Sudoku::printGrid(ofstream& outs)
{
	Creator::getNumHints();

	//display current grid
	for (int rowID = 1; rowID <= 9; ++rowID)
	{
		for (int colID = 1; colID <= 9; ++colID)
		{
			//unsigned location marked as X
			if (grid[rowID][colID] == -1)
			{
				cout << "X ";
				outs << "X ";
			}
			else
			{
				cout << grid[rowID][colID] << " ";
				outs << grid[rowID][colID] << " ";
			}
		}
		cout << endl;
		outs << endl;
	}
	cout << endl;
	outs << endl;
}

/*
Function Name: setNumHints
Author Name: Chong Zhang
Creation Date: 08/07/2019
Modification Date: 08/07/2019
Purpose: set numHints
*/

void Creator::setNumHints(int hints)
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

int Creator::getNumHints()
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

void Creator::incNumHints()
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

void Creator::decNumHints()
{
	numHints--;
}

/*
Function Name: getValue
Author Name: Chong Zhang
Creation Date: 08/07/2019
Modification Date: 08/07/2019
Purpose: return the value of the given location (rowID, colID)
*/

int Sudoku::getValue(int rowID, int colID)
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
Function Name: selectUnassignedLocation
Author Name: Chong Zhang
Creation Date: 08/07/2019
Modification Date: 08/07/2019
Purpose: seach an unassigned location, if the grid has no empty location, return 0
*/

bool Solver::selectUnassignedLocation(int& rowID, int& colID)
{
	while (rowID <= 9)
	{
		while (colID <= 9)
		{
			if (grid[rowID][colID] == NULL)
			{
				return 1;
			}
			colID++;
		}
		colID = 1;
		rowID++;
	}

	return 0;    //no empty location
}



///*
//Function Name: usedInRow
//Author Name: Chong Zhang
//Creation Date: 08/07/2019
//Modification Date: 08/07/2019
//Purpose: return if the given value (value) has been used in row
//*/
//
//bool Sudoku::usedInRow(int rowID, int value) const
//{
//	for (int col = 1; col <= sizeGrid; ++col)
//	{
//		if (grid[rowID][col] == value)
//		{
//			return 1;    //value has been used in row 
//		}
//	}
//	return 0;
//}
//
//
//
///*
//Function Name: usedInColumn
//Author Name: Chong Zhang
//Creation Date: 08/07/2019
//Modification Date: 08/07/2019
//Purpose: return if the given value (value) has been used in column
//*/
//
//bool Sudoku::usedInColumn(int colID, int value) const
//{
//	for (int row = 1; row <= sizeGrid; ++row)
//	{
//		if (grid[row][colID] == value)
//		{
//			return 1;    //value has been used in column 
//		}
//	}
//	return 0;
//}
//
//
//
//
///*
//Function Name: usedInSubGrid
//Author Name: Chong Zhang
//Creation Date: 08/07/2019
//Modification Date: 08/07/2019
//Purpose: return if the given value (value) has been used in the sub-grid
//*/
//
//bool Sudoku::usedInSubGrid(int rowID, int colID, int value) const
//{
//	int subGridStartRowID = ((rowID - 1) / 3) * 3;
//	int subGridStartColID = ((colID - 1) / 3) * 3;
//
//	for (int row = 1; row < 4; ++row)
//	{
//		for (int col = 1; col < 4; ++col)
//		{
//			if (grid[subGridStartRowID + row][subGridStartColID + col] == value)
//			{
//				return 1;    //value has been used in sub-grid 
//			}
//		}
//	}
//	return 0;
//}
//
//
//
//
///*
//Function Name: validToPlace
//Author Name: Chong Zhang
//Creation Date: 08/07/2019
//Modification Date: 08/07/2019
//Purpose: return if the given value is valid to place in the given location (rowID, colID)
//*/
//
//bool Sudoku::validToPlace(int rowID, int colID, int value) const
//{
//	return !usedInRow(rowID, value) && !usedInColumn(colID, value) && !usedInSubGrid(rowID, colID, value);
//}
//
//
//
//
///*
//Function Name: solveSudoku
//Author Name: Chong Zhang
//Creation Date: 08/07/2019
//Modification Date: 08/07/2019
//Purpose: solve sudoku (start from value 1), if no solution, return 0
//*/
//
//bool Sudoku::solveSudoku(int rowID, int colID)
//{
//	if (!selectUnassignedLocation(rowID, colID))
//	{
//		return 1;    //success
//	}
//
//	for (int value = 1; value <= sizeGrid; ++value)
//	{
//		if (validToPlace(rowID, colID, value))
//		{
//			grid[rowID][colID] = value;
//			numHints++;
//			if (solveSudoku(rowID, colID))
//			{
//				return 1;
//			}
//			grid[rowID][colID] = -1;   //failure, clear location and continue
//			numHints--;
//		}
//	}
//	return false;   //failure, start backtracking
//}
//
//
//
//
///*
//Function Name: solveSudokuReverse
//Author Name: Chong Zhang
//Creation Date: 08/07/2019
//Modification Date: 08/07/2019
//Purpose: solve sudoku (start from value 9), if no solution, return 0
//*/
//
//bool Sudoku::solveSudokuReverse(int rowID, int colID)
//{
//	if (!selectUnassignedLocation(rowID, colID))
//	{
//		return 1;    //success
//	}
//
//	for (int value = sizeGrid; value >= 1; --value)
//	{
//		if (validToPlace(rowID, colID, value))
//		{
//			grid[rowID][colID] = value;
//			numHints++;
//			if (solveSudokuReverse(rowID, colID))
//			{
//				return 1;
//			}
//			grid[rowID][colID] = -1;   //failure, clear location and continue
//			numHints--;
//		}
//	}
//	return false;   //failure, start backtracking
//}