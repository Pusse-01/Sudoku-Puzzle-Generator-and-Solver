/*
Name: P.M.N.R.Pussadeniya
Index No: 19/ENG/082
Registration number: EN 95142
Course: Programming Quest
Course Code: CO2210
Assignment 01
*/

#include <iostream>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>

#define UNASSIGNED 0

using namespace std;

class Sudoku {
private:
  
  int solnGrid[9][9];
  int guessNum[9];
  int gridPos[81];
  int difficultyLevel;
  bool grid_status;

public:
 bool fillRemaining(int i, int j);
  bool availOfBox(int row, int col, int number);
  bool availofRow(int i, int number);
  bool availOfCols(int j, int number);
  bool availabilityCheck(int i, int j, int number);
  void boxFilling(int row, int col);
  bool solveSUdoku(int arr[9][9], int row, int col);
  void printSudoku(int arr[9][9]);
  void nextEmpty(int arr[9][9], int row, int col, int& rowNext, int& colNext);
  void copyArray(int arr[9][9], int arrCpy[9][9]);
  vector<int> findPlaceables(int arr[9][9], int row, int col);
  bool placebale(int arr[9][9], int row, int col, int n); 
  
  int grid[9][9];
  Sudoku ();
  Sudoku (string, bool row_major=true);
  void createSeed();
  void printGrid();
  bool solveGrid();
  string getGrid();
  void countSoln(int &number);
  void genPuzzle();
  bool verifyGridStatus();
  void printSVG(string);
  void calculateDifficulty();
  int  branchDifficultyScore();
  
};

/*Here  have wrote functions for each generating and solving respectively 
and added the two codes to one programm and made a single class. */

///////////////////////////////////////////////////////////////////////////////////////////////
/*Functions to generate the sudoku*/
// Defining the function to generate random number
int genRandNum(int maxLimit)
{
  return rand()%maxLimit;
}

//Create seed grid
void Sudoku::createSeed()
{ 
  this->solveGrid();
  
  // Saving the solution grid
  for(int i=0;i<9;i++)
  {
    for(int j=0;j<9;j++)
    {
      this->solnGrid[i][j] = this->grid[i][j];
    }
  }
}



//Intialising
Sudoku::Sudoku()
{



  // Randomly shuffling the array of removing grid positions
  for(int i=0;i<81;i++)
  {
    this->gridPos[i] = i;
  }

  random_shuffle(this->gridPos, (this->gridPos) + 81, genRandNum);

  // Randomly shuffling the guessing number array
  for(int i=0;i<9;i++)
  {
    this->guessNum[i]=i+1;
  }

  random_shuffle(this->guessNum, (this->guessNum) + 9, genRandNum);

  // Initialising the grid
  for(int i=0;i<9;i++)
  {
    for(int j=0;j<9;j++)
    {
      this->grid[i][j]=0;
    }
  }

  grid_status = true;
}



//Custom Initialising with grid passed as argument
Sudoku::Sudoku(string grid_str, bool row_major)
{
  if(grid_str.length() != 81)
  {
    grid_status=false;
    return;
  }

  //Check if all cells are valid
  for(int i=0; i<81; ++i)
  {
    int curr_num = grid_str[i]-'0';
    if(!((curr_num == UNASSIGNED) || (curr_num > 0 && curr_num < 10)))
    {
      grid_status=false;
      return;
    }

    if(row_major) grid[i/9][i%9] = curr_num;
    else          grid[i%9][i/9] = curr_num;
  }

  //Check if all columns are valid
  for (int col_num=0; col_num<9; ++col_num)
  {
    bool nums[10]={false};
    for (int row_num=0; row_num<9; ++row_num)
    {
      int curr_num = grid[row_num][col_num];
      if(curr_num!=UNASSIGNED && nums[curr_num]==true)
      {
        grid_status=false;
        return;
      }
      nums[curr_num] = true;
    }
  }

  //Check if all rows are valid
  for (int row_num=0; row_num<9; ++row_num)
  {
    bool nums[10]={false};
    for (int col_num=0; col_num<9; ++col_num)
    {
      int curr_num = grid[row_num][col_num];
      if(curr_num!=UNASSIGNED && nums[curr_num]==true)
      {
        grid_status=false;
        return;
      }
      nums[curr_num] = true;
    }
  }

  //Check if all blocks are valid
  for (int block_num=0; block_num<9; ++block_num)
  {
    bool nums[10]={false};
    for (int cell_num=0; cell_num<9; ++cell_num)
    {
      int curr_num = grid[((int)(block_num/3))*3 + (cell_num/3)][((int)(block_num%3))*3 + (cell_num%3)];
      if(curr_num!=UNASSIGNED && nums[curr_num]==true)
      {
        grid_status=false;
        return;
      }
      nums[curr_num] = true;
    }
  }

  // Randomly shuffling the guessing number array
  for(int i=0;i<9;i++)
  {
    this->guessNum[i]=i+1;
  }

  random_shuffle(this->guessNum, (this->guessNum) + 9, genRandNum);

  grid_status = true;
}



// Verification status of the custom grid passed
bool Sudoku::verifyGridStatus()
{
  return grid_status;
}

//Printing the grid
void Sudoku::printGrid()
{
  for(int i=0;i<9;i++)
  {
    for(int j=0;j<9;j++)
    {
      if(grid[i][j] == 0)
	cout<<"_";
      else
	cout<<grid[i][j];
      cout<<"|";
    }
    cout<<endl;
  }
}

//Helper functions for solving grid
bool FindUnassignedLocation(int grid[9][9], int &row, int &col)
{
    for (row = 0; row < 9; row++)
    {
        for (col = 0; col < 9; col++)
        {
            if (grid[row][col] == UNASSIGNED)
                return true;
        }
    }

    return false;
}

bool UsedInRow(int grid[9][9], int row, int num)
{
    for (int col = 0; col < 9; col++)
    {
        if (grid[row][col] == num)
            return true;
    }

    return false;
}

bool UsedInCol(int grid[9][9], int col, int num)
{
    for (int row = 0; row < 9; row++)
    {
        if (grid[row][col] == num)
            return true;
    }

    return false;
}

bool UsedInBox(int grid[9][9], int boxStartRow, int boxStartCol, int num)
{
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            if (grid[row+boxStartRow][col+boxStartCol] == num)
                return true;
        }
    }

    return false;
}

bool isSafe(int grid[9][9], int row, int col, int num)
{
    return !UsedInRow(grid, row, num) && !UsedInCol(grid, col, num) && !UsedInBox(grid, row - row%3 , col - col%3, num);
}


//Modified Sudoku solver
bool Sudoku::solveGrid()
{
    int row, col;

    
    if (!FindUnassignedLocation(this->grid, row, col))
       return true; 

    // Consider digits 1 to 9
    for (int num = 0; num < 9; num++)
    {
        
        if (isSafe(this->grid, row, col, this->guessNum[num]))
        {
            
            this->grid[row][col] = this->guessNum[num];

            
            if (solveGrid())
                return true;

            
            this->grid[row][col] = UNASSIGNED;
        }
    }

    return false; // this triggers backtracking

}



//Check if the grid is uniquely solvable
void Sudoku::countSoln(int &number)
{
  int row, col;

  if(!FindUnassignedLocation(this->grid, row, col))
  {
    number++;
    return ;
  }


  for(int i=0;i<9 && number<2;i++)
  {
      if( isSafe(this->grid, row, col, this->guessNum[i]) )
      {
        this->grid[row][col] = this->guessNum[i];
        countSoln(number);
      }

      this->grid[row][col] = UNASSIGNED;
  }

}

//Gneerate puzzle
void Sudoku::genPuzzle()
{
  for(int i=0;i<81;i++)
  {
    int x = (this->gridPos[i])/9;
    int y = (this->gridPos[i])%9;
    int temp = this->grid[x][y];
    this->grid[x][y] = UNASSIGNED;

     int check=0;
    countSoln(check);
    if(check!=1)
    {
      this->grid[x][y] = temp;
    }
  }
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Fuctions to solve the puzzle*/


//Checking whether placeble
bool Sudoku::placebale(int arr[9][9], int row, int col, int n)
{
	if (arr[row][col] != 0) return false;
	bool status = true;
	int gridx = (col / 3) * 3;
	int gridy = (row / 3) * 3;
	for (int i = 0; i < 9; i++) {
		if (arr[row][i] == n) { status = false; break; }
		if (arr[i][col] == n) { status = false; break; }
		if (arr[gridy + i / 3][gridx + i % 3] == n) { status = false; break; }
	}
	return status;
}


void Sudoku::nextEmpty(int arr[9][9], int row, int col, int& rowNext, int& colNext)
{

	int indexNext = 9 * 9 + 1;
	for (int i = row * 9 + col + 1; i < 9 * 9; i++) {
		if (arr[i / 9][i % 9] == 0) {

			indexNext = i;
			break;
		}
	}
	rowNext = indexNext / 9;
	colNext = indexNext % 9;
	//cout << row << "," << col << "|" << rowNext << "," << colNext << endl;
}

//Coppying two arrays
void Sudoku::copyArray(int arr[9][9], int arrCpy[9][9]) {
	for (int y = 0; y < 9; y++)
		for (int x = 0; x < 9; x++)
			arrCpy[y][x] = arr[y][x];
}
vector<int> Sudoku::findPlaceables(int arr[9][9], int row, int col) {
	vector<int> placebles = {};
	for (int n = 1; n <= 9; n++)
		if (placebale(arr, row, col, n)) placebles.push_back(n);
	return placebles;
}

//Checking the availability of 3x3 grids
bool Sudoku::availOfBox(int row, int col, int number)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[row + i][col + j] == number)
                return false;

    return true;
}

//Checking the availability of rows
bool Sudoku::availofRow(int i, int number)
{
    for (int j = 0; j < 9; j++)
        if (grid[i][j] == number)
            return false;
    return true;
}

//Checking the availability of columns
bool Sudoku::availOfCols(int j, int number)
{
    for (int i = 0; i < 9; i++)
        if (grid[i][j] == number)
            return false;
    return true;
}

//Checking availability
bool Sudoku::availabilityCheck(int i, int j, int number)
{
    return (availofRow(i, number) && availOfCols(j, number) && availOfBox(i - i % 3, j - j % 3, number));
}


//Filling the box
void Sudoku::boxFilling(int row, int col)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int numbers[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
            random_device rd;
            mt19937 g(rd());

            int rndm = rand() % 20 + 5;
            for (int c = 0; c <= rndm; c++)
                shuffle(numbers, numbers + 9, g);

            for (int k = 0; k < 9; k++)
            {
                if (availOfBox(row, col, numbers[k] + 1))
                {
                    grid[(row + i)][(col + j)] = numbers[k] + 1;
                    break;
                }
            }
        }
    }
}

//Filling the rest
bool Sudoku::fillRemaining(int i, int j)
{
    if (j >= 9 && i < 8)
    {
        ++i;
        j = 0;
    }

    if (i >= 9 && j >= 9)
        return true;

    if (i < 3 && j < 3)
        j = 3;

    else if (i < 6)
    {
        if (j == (i / 3) * 3)
            j = j + 3;
    }
    else if (j == 6)
    {
        i = i + 1;
        j = 0;
        if (i >= 9)
            return true;
    }

    for (int number = 1; number <= 9; number++)
    {
        if (availabilityCheck(i, j, number))
        {
            grid[i][j] = number;
            if (fillRemaining(i, j + 1))
                return true;

            grid[i][j] = 0;
        }
    }
    return false;
}

//Solving the puzzle
bool Sudoku::solveSUdoku(int arr[9][9], int row, int col)
{
	

	if (row > 8) return true;
	if (arr[row][col] != 0) {
		int rowNext, colNext;
		nextEmpty(arr, row, col, rowNext, colNext);
		return solveSUdoku(arr, rowNext, colNext);
	}

	std::vector<int> placebles = findPlaceables(arr, row, col);

	if (placebles.size() == 0) {
		
		return false; 
	
	};

	bool status = false;
	for (int i = 0; i < placebles.size(); i++) {
		int n = placebles[i];
		int arrCpy[9][9];
		copyArray(arr, arrCpy);
		//cout << "(" << row << "," << col << ") =>" << n << endl;
		arrCpy[row][col] = n;
		int rowNext = row;
		int colNext = col;
		nextEmpty(arrCpy, row, col, rowNext, colNext);
		if (solveSUdoku(arrCpy, rowNext, colNext)) {
			copyArray(arrCpy, arr);
			status = true;
			break;
		}
	}
	
	/*Following two lines of codes are to print the each step to show the process of solving the puzzle.
	But it takes much time to give the result.If you want the result quickly, I reccomend to comment out those two lines.*/
	//system("cls");
	//printSudoku(arr);
	return status;
}

//Printing the puzzle which shows each step
void Sudoku::printSudoku(int arr[9][9]) {
	cout << "-------------------------" << endl;
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++)
			cout << arr[y][x] << " ";
		cout << endl;
	}
	cout << "-------------------------" << endl;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






//main function
int main(int argc, char const *argv[])
{
	cout<<"------------------------Sudoku------------------------------"<<endl;
	cout<<endl;
	cout<<endl;
	cout<<endl;
	cout<<"__________________"<<endl;
	int x;
  // Initialising seed for random number generation
  srand(time(NULL));

  // Creating an instance of Sudoku
  Sudoku *puzzle = new Sudoku();

  // Creating a seed for puzzle generation
  puzzle->createSeed();

  // Generating the puzzle
  puzzle->genPuzzle();

  // testing by printing the grid
  puzzle->printGrid();


	//puzzle->printSVG();
	cout<<"\n\nSudoku Puzzle has been generated succefully. \nPress 0 to solve the Puzzle:";
	cin>>x;

  if (x==0){
  	if (puzzle->solveSUdoku(puzzle->grid, 0, 0)) cout << "\nPuzzle has been solved succesfully!\n\n" << std::endl;
	puzzle->printGrid();
	cout<<"__________________"<<endl;
  }else return 0;

//Freeing memory    
  delete puzzle;	
  return 0;
}


