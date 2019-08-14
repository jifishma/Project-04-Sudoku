#pragma once
/*
 * Purpose: Sukoku superclass - header file
 * Author Name: Chong Zhang, Jeffrey Fishman
 * Creation Date: 08/05/2019
 * Modification Date: 08/12/2019
 */

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

class Sudoku
{
public:
	Sudoku();
	Sudoku(ofstream* outStream);
	void setGrid(int array[10][10]);
	void printGrid();						              			  //display the number of hints and the grid
	void printSubGrid(int rowID, int colID);						      //display the sub-grid by the defined quadrant
	void setValue(int rowID, int colID, int value);							 //set the value in the given lacation (rowID, colID)
	int getValue(int rowID, int colID);								  //return the value in the given lacation (rowID, colID)
	bool validToPlace(int rowID, int colID, int value);				  //check if the given value is valid to place in location (rowID, colID), if not, return 0

protected:
	int grid[10][10];		                                                   //use index 1 to 9
	ofstream* outs;

	/*
	 * Purpose: Solver sub class
	 * Author Name: Jeffrey Fishman
	 * Creation Date: 08/12/2019
	 * Modification Date: 08/12/2019
	 */
	class Solver
	{
	public:
		Solver(Sudoku& parent);
		bool usedInRow(int rowID, int value);								  //check if the given value has been used in row, if not, return false
		bool usedInColumn(int colID, int value);								//check if the given value has been used in column, if not, return false
		bool usedInSubGrid(int rowID, int colID, int value);				  //check if the given value has been used in sub-grid, if not, return false

		pair<int, int>* selectUnassignedLocation(int rowID = 1, int colID = 1);	       //find an unassigned location (rowID, colID), if grid has no empty location, return false
		bool solveSudoku(bool solveBackwards = false);                //solve sudoku in the specified direction (1-9, 9-1), if no solution, return false

		bool printCompletedSubGrid(int rowID, int colID);                       //check if the sub-grid is completed, and print it out if it is complete at some point in time

	private:
		int numComparisons;
		int numBacktracks;
		time_t timer;
		Sudoku& sudoku;
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
		Creator(Sudoku& parent);
		void setNumHints(int hints);
		int getNumHints();
		void incNumHints();                                                        //increase the number of hints by one
		void decNumHints();                                                        //decrease the number of hints by one

	private:
		int numHints;                                                              //require a minimum of 17 numbers
		Sudoku& sudoku;
	};

public:
	Solver solver = Solver(*this);
	Creator creator = Creator(*this);
};