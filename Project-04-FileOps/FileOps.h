#pragma once
#include "../Project-04-Sudoku/Sudoku.h"

/*
Function Name: IsValidData
Author Name: Chong Zhang, Jeffrey Fishman
Creation Date: 08/07/2019
Modification Date: 08/13/2019
Purpose: check if input file contains invalid value or duplicate value
*/

bool IsValidData(Sudoku& puzzle, int rowID, int colID, int value, ofstream& outs)
{
	//check if input file contains invalid value 
	if (value < 1 || value > 9)
	{
		cout << rowID << " " << colID << " " << value;
		outs << rowID << " " << colID << " " << value;

		cerr << "\t\t Error - Invalid value - Program ended" << endl;
		outs << "\t\t Error - Invalid value - Program ended" << endl;
		return false;
	}

	//check if input file contains duplicate data
	if (puzzle.getValue(rowID, colID) != NULL)
	{
		cout << rowID << " " << colID << " " << value;
		outs << rowID << " " << colID << " " << value;

		cerr << "\t\t Error - Duplicate value - Program ended" << endl;
		outs << "\t\t Error - Duplicate value - Program ended" << endl;
		return false;
	}

	return true;                //valid data, return 1
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
		cout << "Error - Provided input file '" << inFileName << "' does not exist or could not be read. Exiting program." << endl;
		outs << "Error - Provided input file '" << inFileName << "' does not exist or could not be read. Exiting program." << endl;
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
	while (ins)
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

	cout << endl;
	outs << endl;
}

/*
Function Name: OpenFileStreams
Author Name: Chong Zhang, Jeffrey Fishman
Creation Date: 08/07/2019
Modification Date: 08/13/2019
Purpose: ask for and open the given input and output file streams
*/

void OpenFileStreams(ifstream& ins, string& inFileName, ofstream& outs, string& outStatistic)
{
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
}

/*
Function Name: CloseFileStreams
Author Name: Jeffrey Fishman
Creation Date: 08/07/2019
Modification Date: 08/13/2019
Purpose: close the provided file streams if they're open
*/

void CloseFileStreams(ifstream& ins, ofstream& outs)
{
	if (ins.is_open())
		ins.close();

	if (outs.is_open())
		outs.close();
}