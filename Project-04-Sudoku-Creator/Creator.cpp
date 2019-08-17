#include "../Project-04-Sudoku/Sudoku.h"
#include "../Project-04-FileOps/FileOps.h"

#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>

/*
 * Purpose: Sudoku creator
 * Author Name: Chong Zhang, Jeffrey Fishman
 * Creation Date: 08/05/2019
 * Modification Date: 08/13/2019
 */

void ValidatePuzzleIsFull(Sudoku& puzzle, ofstream& outs);
void ValidateNumHints(int& numHints, ofstream& outs);
void CreateCreatorInputFile(Sudoku puzzle, ofstream& outs);

int main()
{
	ifstream ins;
	ofstream outs;
	string inFileName, outFileName, outStatistic;

	Sudoku puzzle = Sudoku(outs);
	puzzle.state = State::Creating;

	int numHints = 0;
	int numRandNum = 0;
	int numComparisons = 0;
	int numBacktracks = 0;
	int numSubGridIterations = 0;

	//display welcome message
	cout << "-----------Welcome to Sudoku Creator Program-----------" << endl;

	//ask for and open the input and output file streams
	OpenFileStreams(ins, inFileName, outs, outStatistic);

	//print welcome message to file
	outs << "-----------Welcome to Sudoku Creator Program-----------" << endl;

	//read input file, and populate the puzzle accordingly
	PreparePuzzle(inFileName, puzzle, ins, outs);

	//check that the prepared puzzle has every cell populated
	ValidatePuzzleIsFull(puzzle, outs);

	//close input file
	ins.close();

	//print initial data
	cout << "Initial data" << endl;
	outs << "Initial data" << endl;
	puzzle.printGrid();


	//ask for the number of hints the user wants (17 to 40) 
	cout << "Please enter the number of hints (17 to 40): ";
	cin >> numHints;

	//check if the user-entered number of hints is valid
	ValidateNumHints(numHints, outs);

	//set srand
	srand(time(0));

	//create puzzle 
	int i = 1;
	while (i <= 81 - numHints)
	{
		//generate a rand number between 0 to 80
		int popID = rand() % 81;
		++numRandNum;

		//get location ID
		int rowID = popID / 9 + 1;
		int colID = popID % 9 + 1;

		//get the value in location (rowID, colID)
		int value = puzzle.getValue(rowID, colID);

		//check if the selected location is valid
		if (value != NULL)
		{
			//unassign location
			puzzle.setValue(rowID, colID, NULL);

			//copy the created Sudoku twice
			Sudoku copyPuzzle1 = puzzle;
			Sudoku copyPuzzle2 = puzzle;

			//solve the first copy of the created Sudoku start from location (1, 1) and value in increasing order (1 to 9)
			copyPuzzle1.solver.solveSudoku();
			numComparisons += copyPuzzle1.solver.getNumComparisons();
			numBacktracks += copyPuzzle1.solver.getNumBacktracks();

			//solve the second copy of the created Sudoku start from location (1, 1) and value in decreasing order (9 to 1)
			copyPuzzle2.solver.solveSudoku(true);
			numComparisons += copyPuzzle2.solver.getNumComparisons();
			numBacktracks += copyPuzzle2.solver.getNumBacktracks();


			//compare the two above solutions to check if there are multiple solutions
			if (copyPuzzle1 != copyPuzzle2)
			{
				puzzle.setValue(rowID, colID, value);		//failure, restore the selected location and continue
			}
			else
			{
				//decrease the number of hints by one
				puzzle.creator.decNumHints();

				//display the data about the selected pop location
				cout << "Step " << i << ": " << endl;
				outs << "Step " << i << ": " << endl;
				cout << "Popped value " << value << " at (" << rowID << ", " << colID << ")" << endl;
				outs << "Popped value " << value << " at (" << rowID << ", " << colID << ")" << endl;
				puzzle.printGrid();

				//go to next iteration
				++i;
			}
		}
	}

	// Once we get here, print out collected metrics from finding a solution
	cout << "Creator stats: " << endl;
	cout << "\tRandom number made: " << numRandNum << endl;
	cout << "\tComparisons made: " << numComparisons << endl;
	cout << "\tBacktracks completed: " << numBacktracks << endl;
	cout << "\tPuzzle Comparisons to Backtracks ratio: " << (numComparisons / (double)numBacktracks) << ":1" << endl;

	outs << "Creator stats: " << endl;
	outs << "\tRandom number made: " << numRandNum << endl;
	outs << "\tComparisons made: " << numComparisons << endl;
	outs << "\tBacktracks completed: " << numBacktracks << endl;
	outs << "\tPuzzle Comparisons to Backtracks ratio: " << (numComparisons / (double)numBacktracks) << ":1" << endl;


	//close output statistic file
	outs.close();

	//ask for output file name
	cout << "Please enter output file name: ";
	cin >> outFileName;

	//open output file
	outs.open(outFileName);

	//create input file for Sudoku solver
	CreateCreatorInputFile(puzzle, outs);

	//close output file
	outs.close();

	system("pause");
	return 0;
}

/*
Function Name: ValidatePuzzleIsFull
Author Name: Chong Zhang, Jeffrey Fishman
Creation Date: 08/05/2019
Modification Date: 08/14/2019
Purpose: validate that the puzzle is filled
*/

void ValidatePuzzleIsFull(Sudoku& puzzle, ofstream& outs)
{
	//check if there is any empty location
	if (puzzle.creator.getNumHints() != 81)
	{
		cerr << "Error - The grid has an empty cell. Exiting program." << endl;
		outs << "Error - The grid has an empty cell. Exiting program." << endl;

		system("pause");
		exit(EXIT_FAILURE);                //the grid has empty location, stop program
	}
}


/*
Function Name: IsValidHints
Author Name: Chong Zhang
Creation Date: 08/12/2019
Modification Date: 08/12/2019
Purpose: check if the number of hints is valid, if not, ask the user to enter again
*/

void ValidateNumHints(int& numHints, ofstream& outs)
{
	while (numHints < 17 || numHints > 40)
	{
		cerr << "Error - Invalid number of hints" << endl;

		cout << "Please enter the number of hints (17 to 40): ";
		cin >> numHints;
	}

	cout << "Requested number of hints: " << numHints << endl << endl;
	outs << "Requested number of hints: " << numHints << endl << endl;
}




/*
Function Name: CreateSolverInputFile
Author Name: Chong Zhang
Creation Date: 08/05/2019
Modification Date: 08/12/2019
Purpose: create input file for Sudoku solver
*/

void CreateCreatorInputFile(Sudoku puzzle, ofstream& outs)
{
	for (int rowID = 1; rowID <= 9; ++rowID)
	{
		for (int colID = 1; colID <= 9; ++colID)
		{
			if (puzzle.getValue(rowID, colID) != NULL)
			{
				cout << rowID << " " << colID << " " << puzzle.getValue(rowID, colID) << endl;
				outs << rowID << " " << colID << " " << puzzle.getValue(rowID, colID) << endl;
			}
		}
	}
}