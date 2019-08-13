#pragma once
/*
 * Purpose: Sukoku - header file
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
	static void setGrid(int** array);
	static void printGrid(ofstream& outs);									  //display the number of hints and the grid
	static void setValue(int rowID, int colID, int value);							 //set the value in the given lacation (rowID, colID)
	static int getValue(int rowID, int colID);								  //return the value in the given lacation (rowID, colID)

protected:
	static int grid[10][10];		                                                   //use index 1 to 9
};

/*
 * Purpose: Solver child class
 * Author Name: Jeffrey Fishman
 * Creation Date: 08/12/2019
 * Modification Date: 08/12/2019
 */
class Solver : public Sudoku
{
public:
	static bool usedInRow(int rowID, int value);								  //check if the given value has been used in row, if not, return 0
	static bool usedInColumn(int colID, int value);								//check if the given value has been used in column, if not, return 0
	static bool usedInSubGrid(int rowID, int colID, int value);				  //check if the given value has been used in sub-grid, if not, return 0

	static bool printCompletedSubGrid(int rowID, int colID);                       //check if the sub-grid is completed, and print it out if it is complete at some point in time

	static bool selectUnassignedLocation(int& rowID, int& colID);					   //find an unassigned location (rowID, colID), if grid has no empty location, return 0
	static bool validToPlace(int rowID, int colID, int value);				  //check if the given value is valid to place in location (rowID, colID), if not, return 0

	static bool solveSudoku(int rowID, int colID);                                    //solve sudoku, start from value 1 to 9 in increasing order, if no solution, return 0
	static bool solveSudokuReverse(int rowID, int colID);							 //solve sudoku, start from value 9 to 1 in decreasing order, if no solution, return 0
};

/*
 * Purpose: Creator child class
 * Author Name: Jeffrey Fishman
 * Creation Date: 08/12/2019
 * Modification Date: 08/12/2019
 */
class Creator : public Sudoku
{
public:
	static void setNumHints(int hints);
	static int getNumHints();
	static void incNumHints();                                                        //increase the number of hints by one
	static void decNumHints();                                                        //decrease the number of hints by one

private:
	static int numHints;                                                              //require a minimum of 17 numbers
};