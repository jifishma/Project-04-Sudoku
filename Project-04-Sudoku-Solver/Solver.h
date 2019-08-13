#pragma once
#include <vector>

using namespace std;

class Solver
{
public:
	Solver();
	~Solver();
	void assignCell(int& rowID, int& colID, int& value);
	bool checkRow(int& rowID, int& value) const;
	bool checkColumn(int& colID, int& value) const;
	bool checkSubGrid(int& rowID, int& colID, int& value) const;
	bool isValidValue(int& rowID, int& colID, int& value) const;
	void findEmptyCell(int& rowID, int& colID) const;
	bool solveGrid(int** grid);                        //recursive backtracking

	int solvedGrid[10][10] = { 0 };	                   //use index 1 to 9

private:
	int originalGrid[10][10] = { 0 };      //copy of input grid
	int sizeGrid = 9;	                   //9x9 grid
};