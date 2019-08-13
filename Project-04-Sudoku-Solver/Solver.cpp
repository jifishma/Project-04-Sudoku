#include "Solver.h"
#include <vector>

Solver::Solver()
{ }

Solver::~Solver()
{ }

void Solver::assignCell(int& rowID, int& colID, int& value)
{
	return;
}

bool Solver::checkRow(int& rowID, int& value) const
{
	return false;
}

bool Solver::checkColumn(int& colID, int& value) const
{
	return false;
}

bool Solver::checkSubGrid(int& rowID, int& colID, int& value) const
{
	return false;
}

bool Solver::isValidValue(int& rowID, int& colID, int& value) const
{
	return false;
}

void Solver::findEmptyCell(int& rowID, int& colID) const
{ 
	return;
}

bool Solver::solveGrid(int** grid)
{

	return false;
}
