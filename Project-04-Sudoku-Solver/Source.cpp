#include "Sudoku.h"
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

void ValidateFileStreams(string inFileName, ifstream& ins, ofstream& outs);
void PreparePuzzle(string inFileName, Sudoku& puzzle, ifstream& ins, ofstream& outs);
bool IsValidData(Sudoku& puzzle, int rowID, int colID, int value, ofstream& outs);
void ValidateNumHints(int& numHints, ofstream& outs);
void CreateInputFile(Sudoku puzzle, ofstream& outs);

int main()
{
	ifstream ins;
	ofstream outs;
	string inFileName, outFileName, outStatistic;

	Sudoku puzzle = Sudoku(&outs);
	int numHints = 0;

	//display welcome message
	cout << "-----------Welcome to Sudoku Program-----------" << endl;
	outs << "-----------Welcome to Sudoku Program-----------" << endl;

	//ask for input file path 
	cout << "Please enter input file name: ";
	cin >> inFileName;

	//ask for output statistic file path
	cout << "Please enter output statistic file name: ";
	cin >> outStatistic;

	//open the input and output file streams
	ins.open(inFileName);
	outs.open(outStatistic);

	//validate the file streams are opened successfully and contain data
	ValidateFileStreams(inFileName, ins, outs);

	//read input file, and populate the puzzle accordingly
	PreparePuzzle(inFileName, puzzle, ins, outs);

	//close input file
	ins.close();

	//print initial data
	cout << "Initial data" << endl;
	outs << "Initial data" << endl;
	puzzle.printGrid();


	//ask for the number of hints the user wants (17 to 40) 
	cout << "Please enter the number of hints (17 to 40): ";
	cin >> numHints;

	//check if the number of hints is valid
	ValidateNumHints(numHints, outs);



	//set srand
	srand(time(0));

	//create puzzle 
	int i = 1;
	while (i <= 81 - numHints)
	{
		//generate a rand number between 0 to 80
		int popID = rand() % 81;

		//get location ID
		int rowID = popID / 9 + 1;
		int colID = popID % 9 + 1;

		//get the value in location (rowID, colID)
		int value = puzzle.getValue(rowID, colID);

		//display the data about the selected location
		cout << "(" << rowID << ", " << colID << ") " << value << endl;
		outs << "(" << rowID << ", " << colID << ") " << value << endl;

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

			//solve the second copy of the created Sudoku start from location (1, 1) and value in decreasing order (9 to 1)
			copyPuzzle2.solver.solveSudoku(true);

			//compare the two above solutions to check if there are multiple solutions
			if (copyPuzzle1 != copyPuzzle2)
			{
				puzzle.setValue(rowID, colID, value);		//failure, restore the selected location and continue
			}
			else
			{
				//decrease the number of hints by one
				puzzle.creator.decNumHints();

				//display each iteration
				cout << "Step " << i << ": " << endl;
				outs << "Step " << i << ": " << endl;
				puzzle.printGrid();

				//go to next iteration
				++i;
			}
		}
	}

	//close output statistic file
	outs.close();



	//ask for output file name
	cout << "Please enter output file name: ";
	cin >> outFileName;

	//open output file
	outs.open(outFileName);

	//create input file for Sudoku solver
	CreateInputFile(puzzle, outs);

	//close output file
	outs.close();


	system("pause");
	return 0;
}



/*
Function Name: ValidateFileStreams
Author Name: Chong Zhang, Jeffrey Fishman
Creation Date: 08/12/2019
Modification Date: 08/13/2019
Purpose: check if the input file is valid, if not, stop program
*/

void ValidateFileStreams(string inFileName, ifstream& ins, ofstream& outs)
{
	//check if input file exists and opened properly
	if (!ins.is_open() || !ins.good())
	{
		cout << "Error - Provided input file '" << inFileName << "' does not exist. Exiting program." << endl;
		outs << "Error - Provided input file '" << inFileName << "' does not exist. Exiting program." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	//check if input file is empty
	if (ins.peek() == EOF)
	{
		cout << "Error - Input file '" << inFileName << "' exists but contains no data. Exiting program." << endl;
		outs << "Error - Input file '" << inFileName << "' exists but contains no data. Exiting program." << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
}



/*
Function Name: PreparePuzzle
Author Name: Chong Zhang, Jeffrey Fishman
Creation Date: 08/12/2019
Modification Date: 08/13/2019
Purpose: read input file, store into an array and return the number of hints, if file contains invalid data, stop program
*/

void PreparePuzzle(string inFileName, Sudoku& puzzle, ifstream& ins, ofstream& outs)
{
	int rowID, colID, value;
	puzzle.creator.setNumHints(0);

	//display input file name
	cout << endl << "Input file name: " << inFileName << endl;
	outs << "Input file name: " << inFileName << endl;

	//read input file line by line
	ins >> rowID >> colID >> value;
	while (!ins.eof())
	{
		//check if data is valid
		if (IsValidData(puzzle, rowID, colID, value, outs))
		{
			cout << rowID << " " << colID << " " << value << endl;
			outs << rowID << " " << colID << " " << value << endl;

			puzzle.setValue(rowID, colID, value);
			puzzle.creator.incNumHints();
		}
		else
		{
			system("pause");
			exit(EXIT_FAILURE);            //invalid input data, stop program
		}
		ins >> rowID >> colID >> value;
	}

	//check if there is any empty location
	if (puzzle.creator.getNumHints() != 81)
	{
		cerr << "Error - The grid has an empty cell. Exiting program." << endl;
		outs << "Error - The grid has an empty cell. Exiting program." << endl;

		system("pause");
		exit(EXIT_FAILURE);                //the grid has empty location, stop program
	}

	cout << endl;
	outs << endl;
}



/*
Function Name: IsValidData
Author Name: Chong Zhang, Jeffrey Fishman
Creation Date: 08/07/2019
Modification Date: 08/132019
Purpose: check if input file contains invalid value or duplicate value
*/

bool IsValidData(Sudoku& puzzle, int rowID, int colID, int value, ofstream& outs)
{
	//check if input file contains duplicate data
	if (puzzle.getValue(rowID, colID) != NULL)
	{
		cout << rowID << " " << colID << " " << value;
		outs << rowID << " " << colID << " " << value;

		cerr << "\t\t Error - Duplicate value - Program ended" << endl;
		outs << "\t\t Error - Duplicate value - Program ended" << endl;
		return false;
	}

	//check if input file contains invalid value 
	if (value < 1 || value > 9)
	{
		cout << rowID << " " << colID << " " << value;
		outs << rowID << " " << colID << " " << value;

		cerr << "\t\t Error - Invalid value - Program ended" << endl;
		outs << "\t\t Error - Invalid value - Program ended" << endl;
		return false;
	}

	return true;                //valid data, return 1
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

	cout << "Number of hints: " << numHints << endl;
	outs << "Number of hints: " << numHints << endl;
}




/*
Function Name: CreateInputFile
Author Name: Chong Zhang
Creation Date: 08/05/2019
Modification Date: 08/12/2019
Purpose: create input file for Sudoku solver
*/

void CreateInputFile(Sudoku puzzle, ofstream& outs)
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