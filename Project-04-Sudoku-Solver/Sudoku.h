#pragma once
/*
 * Purpose: Sukoku superclass - header file
 * Author Name: Chong Zhang, Jeffrey Fishman
 * Creation Date: 08/05/2019
 * Modification Date: 08/12/2019
 */

#include <iostream>
#include <fstream>

using namespace std;

class Sudoku
{
public:
	Sudoku();
	Sudoku(ofstream* outStream);
	static void setGrid(int** array);
	static void printGrid();						              			  //display the number of hints and the grid
	static void setValue(int rowID, int colID, int value);							 //set the value in the given lacation (rowID, colID)
	static int getValue(int rowID, int colID);								  //return the value in the given lacation (rowID, colID)
	static bool validToPlace(int rowID, int colID, int value);				  //check if the given value is valid to place in location (rowID, colID), if not, return 0

protected:
	static int grid[10][10];		                                                   //use index 1 to 9
	static ofstream* outs;

	/*
	 * Purpose: Solver sub class
	 * Author Name: Jeffrey Fishman
	 * Creation Date: 08/12/2019
	 * Modification Date: 08/12/2019
	 */
	class Solver
	{
	public:
		static bool usedInRow(int rowID, int value);								  //check if the given value has been used in row, if not, return false
		static bool usedInColumn(int colID, int value);								//check if the given value has been used in column, if not, return false
		static bool usedInSubGrid(int rowID, int colID, int value);				  //check if the given value has been used in sub-grid, if not, return false

		static pair<int, int>* selectUnassignedLocation(int rowID = 1, int colID = 1);	       //find an unassigned location (rowID, colID), if grid has no empty location, return false
		static bool solveSudoku(bool solveBackwards = false);                //solve sudoku in the specified direction (1-9, 9-1), if no solution, return false

		static bool validateCompletedSubGrid(int rowID, int colID);                       //check if the sub-grid is completed, and print it out if it is complete at some point in time

	private:
		static int numComparisons;
		static int numBacktracks;
		static time_t timer;
	};

	/*
	 * Purpose: Creator sub class
	 * Author Name: Jeffrey Fishman
	 * Creation Date: 08/12/2019
	 * Modification Date: 08/12/2019
	 */
	class Creator
	{
	public:
		static void setNumHints(int hints);
		static int getNumHints();
		static void incNumHints();                                                        //increase the number of hints by one
		static void decNumHints();                                                        //decrease the number of hints by one

	private:
		static int numHints;                                                              //require a minimum of 17 numbers
	};

public:
	// Class containers for access from within Sudoku class
	Solver solver;
	Creator creator;
};