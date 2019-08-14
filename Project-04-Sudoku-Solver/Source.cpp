#include "Sudoku.h"
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>

/*
 * Purpose: Sudoku creator
 * Author Name: Chong Zhang
 * Creation Date: 08/05/2019
 * Modification Date: 08/12/2019
 */

void FileHandling(string inFileName, ifstream& ins, ofstream& outs);
void ReadInputFile(string inFileName, int array[10][10], ifstream& ins, ofstream& outs);
bool IsValidData(int array[10][10], int rowID, int colID, int value, ofstream& outs);
void HintsHandling(int& numHints, ofstream& outs);
void CreateInputFile(Sudoku puzzle, ofstream& outs);
bool MultiSolutions(Sudoku puzzle1, Sudoku puzzle2);

int main()
{

	ifstream ins;
	ofstream outs;
	string inFileName, outFileName, outStatistic;
	int array[10][10];
	Sudoku puzzle = Sudoku(&outs);
	int numHints = 0;

	//display welcome message
	cout << "-----------Welcome to Sudoku Creator Program-----------" << endl;
	outs << "-----------Welcome to Sudoku Creator Program-----------" << endl;

	//ask for input file name 
	cout << "Please enter input file name: ";
	cin >> inFileName;

	//ask for output statistic file name
	cout << "Please enter output statistic file name: ";
	cin >> outStatistic;

	//open above two files
	ins.open(inFileName);
	outs.open(outStatistic);

	//input file handling
	FileHandling(inFileName, ins, outs);

	//read input file into the given array
	ReadInputFile(inFileName, array, ins, outs);

	//close input file
	ins.close();



	//set the grid
	puzzle.setGrid(array);

	//set the number of hints
	puzzle.creator.setNumHints(81);

	//print initial data
	cout << "Initial data" << endl;
	outs << "Initial data" << endl;
	puzzle.printGrid();



	//ask for the number of hints the user wants (17 to 40) 
	cout << "Please enter the number of hints (17 to 40): ";
	cin >> numHints;

	//check if the number of hints is valid
	HintsHandling(numHints, outs);



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
		if (value != -1)
		{
			//unassign location
			puzzle.setValue(rowID, colID, -1);

			//copy the created Sudoku twice
			Sudoku copyPuzzle1 = puzzle;
			Sudoku copyPuzzle2 = puzzle;

			//solve the first copy of the created Sudoku start from location (1, 1) and value in increasing order (1 to 9)
			copyPuzzle1.solver.solveSudoku();

			//solve the second copy of the created Sudoku start from location (1, 1) and value in decreasing order (9 to 1)
			copyPuzzle2.solver.solveSudoku(true);

			//compare the two above solutions to check if there are multiple solutions
			if (MultiSolutions(copyPuzzle1, copyPuzzle2))
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
Function Name: FileHandling
Author Name: Chong Zhang
Creation Date: 08/12/2019
Modification Date: 08/12/2019
Purpose: check if the input file is valid, if not, stop program
*/

void FileHandling(string inFileName, ifstream& ins, ofstream& outs)
{
	//check if input file exists
	if (!ins.is_open())
	{
		cout << "Error message - Input file " << inFileName << " provided does not exist - Program ended" << endl;
		outs << "Error message - Input file " << inFileName << " provided does not exist - Program ended" << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	//check if input file is empty
	if (ins.peek() == EOF)
	{
		cout << "Error message - Input file " << inFileName << " exists but contains no data - Program ended" << endl;
		outs << "Error message - Input file " << inFileName << " exists but contains no data - Program ended" << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
}



/*
Function Name: ReadInputFile
Author Name: Chong Zhang
Creation Date: 08/12/2019
Modification Date: 08/12/2019
Purpose: read input file, store into an array and return the number of hints, if file contains invalid data, stop program
*/

void ReadInputFile(string inFileName, int array[10][10], ifstream& ins, ofstream& outs)
{
	int rowID, colID, value;
	int numHints = 0;

	//display input file name
	cout << endl << "Input file name: " << inFileName << endl;
	outs << "Input file name: " << inFileName << endl;

	//initialize the array with -1
	for (int i = 1; i <= 9; ++i)
	{
		for (int j = 1; j <= 9; ++j)
		{
			array[i][j] = -1;
		}
	}

	//read input file line by line
	ins >> rowID >> colID >> value;
	while (ins)
	{
		//check if data is valid
		if (IsValidData(array, rowID, colID, value, outs))
		{
			cout << rowID << " " << colID << " " << value << endl;
			outs << rowID << " " << colID << " " << value << endl;
			array[rowID][colID] = value;
			++numHints;
		}
		else
		{
			system("pause");
			exit(EXIT_FAILURE);            //invalid input data, stop program
		}
		ins >> rowID >> colID >> value;
	}

	//check if there is any empty location
	if (numHints != 81)
	{
		cout << "Error message - The grid has empty location - Program ended!!" << endl;
		outs << "Error message - The grid has empty location - Program ended!!" << endl;

		system("pause");
		exit(EXIT_FAILURE);                //the grid has empty location, stop program
	}

	cout << endl;
	outs << endl;
}



/*
Function Name: IsValidData
Author Name: Chong Zhang
Creation Date: 08/07/2019
Modification Date: 08/12/2019
Purpose: check if input file contains invalid value or duplicate value
*/

bool IsValidData(int array[10][10], int rowID, int colID, int value, ofstream& outs)
{
	//check if input file contains duplicate data
	if (array[rowID][colID] != -1)
	{
		cout << rowID << " " << colID << " " << value;
		outs << rowID << " " << colID << " " << value;
		cout << "\t\t Error message - Duplicate value - Program ended" << endl;
		outs << "\t\t Error message - Duplicate value - Program ended" << endl;
		return 0;
	}

	//check if input file contains invalid value 
	if (value < 1 || value > 9)
	{
		cout << rowID << " " << colID << " " << value;
		outs << rowID << " " << colID << " " << value;
		cout << "\t\t Error message - Invalid value - Program ended" << endl;
		outs << "\t\t Error message - Invalid value - Program ended" << endl;
		return 0;
	}

	return 1;                //valid data, return 1
}



/*
Function Name: IsValidHints
Author Name: Chong Zhang
Creation Date: 08/12/2019
Modification Date: 08/12/2019
Purpose: check if the number of hints is valid, if not, ask the user to enter again
*/

void HintsHandling(int& numHints, ofstream& outs)
{
	while (numHints < 17 || numHints > 40)
	{
		cout << "Error message - Invalid number of hints" << endl;

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
			if (puzzle.getValue(rowID, colID) != -1)
			{
				cout << rowID << " " << colID << " " << puzzle.getValue(rowID, colID) << endl;
				outs << rowID << " " << colID << " " << puzzle.getValue(rowID, colID) << endl;
			}
		}
	}
}





/*
Function Name: MultiSolutions
Author Name: Chong Zhang
Creation Date: 08/05/2019
Modification Date: 08/12/2019
Purpose: check if there are multiple solutions, if not, return 0
*/

bool MultiSolutions(Sudoku puzzle1, Sudoku puzzle2)
{
	for (int row = 1; row <= 9; ++row)
	{
		for (int col = 1; col <= 9; ++col)
		{
			if (puzzle1.getValue(row, col) != puzzle2.getValue(row, col))
			{
				return 1;      //multiple solutions
			}
		}
	}
	return 0;				//unique solution
}