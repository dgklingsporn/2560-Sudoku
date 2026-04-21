/*
Sudoku Solver - Part A
EECE 2560
4/13/26
Group ID + #: ROBKLISYGRE 10
board.cpp
   This file implements the board class for 
   a Sudoku puzzle. It handles reading the board 
   , tracking conflicts, printing the board and
   conflicts, clearing, and checking if the board is solved.

*/


#include <iostream>
#include <limits.h>
#include "d_ matrix.h"
#include "d_except.h"
#include <list>
#include <fstream>

using namespace std;

typedef int ValueType; // value in each cell
const int Blank = -1;  // blank cell marker

const int SquareSize = 3;
const int BoardSize = SquareSize * SquareSize;

const int MinValue = 1;
const int MaxValue = 9;

int numSolutions = 0;

// return which 3x3 square cell (i,j) belongs to
int squareNumber(int i, int j)
{
   return SquareSize * ((i-1)/SquareSize) + (j-1)/SquareSize + 1;
}

class board
{
   public:
      board(int);
      void clear();
      void initialize(ifstream &fin);
      void print();
      void printConflicts();
      bool isBlank(int, int);
      ValueType getCell(int, int);
      void setCell(int, int, ValueType);
      void clearCell(int, int);
      bool isSolved();
      bool findBlank(int &row, int &col);

   private:
      matrix<ValueType> value; // actual board values

      // track if a value already exists in row/col/square
      matrix<bool> rowUsed;
      matrix<bool> colUsed;
      matrix<bool> squareUsed;

      matrix<int> conflicts; // number of conflicts per cell

      void recomputeConflicts(); // rebuild all conflict info
};

board::board(int sqSize)
   : value(BoardSize+1, BoardSize+1),
     rowUsed(BoardSize+1, BoardSize+1),
     colUsed(BoardSize+1, BoardSize+1),
     squareUsed(BoardSize+1, BoardSize+1),
     conflicts(BoardSize+1, BoardSize+1)
{
   clear();
}

void board::clear()
// reset board and all tracking arrays
{
   for (int i = 1; i <= BoardSize; i++)
      for (int j = 1; j <= BoardSize; j++)
      {
         value[i][j] = Blank;
         rowUsed[i][j] = false;
         colUsed[i][j] = false;
         squareUsed[i][j] = false;
         conflicts[i][j] = 0;
      }
}

void board::recomputeConflicts()
// rebuild all conflict info from current board
{
   // reset tracking
   for (int i = 1; i <= BoardSize; i++)
      for (int j = 1; j <= BoardSize; j++)
      {
         rowUsed[i][j] = false;
         colUsed[i][j] = false;
         squareUsed[i][j] = false;
         conflicts[i][j] = 0;
      }

   // mark which values are present in each row/col/square
   for (int i = 1; i <= BoardSize; i++)
      for (int j = 1; j <= BoardSize; j++)
         if (value[i][j] != Blank)
         {
            int val = value[i][j];
            int s = squareNumber(i,j);

            rowUsed[i][val] = true;
            colUsed[j][val] = true;
            squareUsed[s][val] = true;
         }

   // compute conflicts for each filled cell
   for (int i = 1; i <= BoardSize; i++)
      for (int j = 1; j <= BoardSize; j++)
         if (value[i][j] != Blank)
         {
            int val = value[i][j];
            int count = 0;

            // check row for duplicates
            for (int c = 1; c <= BoardSize; c++)
               if (c != j && value[i][c] == val)
               {
                  count++;
                  break;
               }

            // check column for duplicates
            for (int r = 1; r <= BoardSize; r++)
               if (r != i && value[r][j] == val)
               {
                  count++;
                  break;
               }

            // check 3x3 square for duplicates
            int rStart = ((i-1)/SquareSize)*SquareSize + 1;
            int cStart = ((j-1)/SquareSize)*SquareSize + 1;

            for (int r = rStart; r < rStart + SquareSize; r++)
               for (int c = cStart; c < cStart + SquareSize; c++)
                  if (!(r == i && c == j) && value[r][c] == val)
                  {
                     count++;
                     r = rStart + SquareSize; // break outer loop
                     break;
                  }

            conflicts[i][j] = count; // store total conflicts
         }
}

void board::setCell(int i, int j, ValueType val)
// set value and recompute conflicts
{
   if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
      throw rangeError("bad value in setCell");

   if (val < MinValue || val > MaxValue)
      throw rangeError("bad cell value in setCell");

   value[i][j] = val;

   // easiest approach for part A: rebuild everything
   recomputeConflicts();
}

void board::clearCell(int i, int j)
// clear a cell and update conflicts
{
   if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
      throw rangeError("bad value in clearCell");

   value[i][j] = Blank;
   recomputeConflicts();
}

void board::initialize(ifstream &fin)
// read one board from file ('.' = blank)
{
   char ch;

   clear();

   for (int i = 1; i <= BoardSize; i++)
      for (int j = 1; j <= BoardSize; j++)
      {
         fin >> ch;

         if (ch != '.')
            setCell(i,j,ch-'0');
      }
}

ValueType board::getCell(int i, int j)
// return value at (i,j)
{
   if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize)
      return value[i][j];
   else
      throw rangeError("bad value in getCell");
}

bool board::isBlank(int i, int j)
// check if cell is blank
{
   if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
      throw rangeError("bad value in isBlank");

   return (getCell(i,j) == Blank);
}

void board::print()
// print board nicely formatted
{
   for (int i = 1; i <= BoardSize; i++)
   {
      if ((i-1) % SquareSize == 0)
      {
         cout << " -";
         for (int j = 1; j <= BoardSize; j++)
            cout << "---";
         cout << "-\n";
      }

      for (int j = 1; j <= BoardSize; j++)
      {
         if ((j-1) % SquareSize == 0)
            cout << "|";

         if (!isBlank(i,j))
            cout << " " << getCell(i,j) << " ";
         else
            cout << "   ";
      }

      cout << "|\n";
   }

   cout << " -";
   for (int j = 1; j <= BoardSize; j++)
      cout << "---";
   cout << "-\n";
}

void board::printConflicts()
// print conflict counts in same layout as board
{
   cout << "\nConflicts:\n";

   for (int i = 1; i <= BoardSize; i++)
   {
      if ((i-1) % SquareSize == 0)
      {
         cout << " -";
         for (int j = 1; j <= BoardSize; j++)
            cout << "---";
         cout << "-\n";
      }

      for (int j = 1; j <= BoardSize; j++)
      {
         if ((j-1) % SquareSize == 0)
            cout << "|";

         if (!isBlank(i,j))
            cout << " " << conflicts[i][j] << " ";
         else
            cout << "   ";
      }

      cout << "|\n";
   }

   cout << " -";
   for (int j = 1; j <= BoardSize; j++)
      cout << "---";
   cout << "-\n";
}
bool board::valid_Num(int value, int i, int j){
   if (rowUsed[i][value] || colUsed[j][value] || squareUsed[squareNumber(i,j)][value]){
      return false;
   }
   else{
      return true;
   }
}
bool board::isSolved()
// solved = no blanks AND no conflicts
{
   for (int i = 1; i <= BoardSize; i++)
      for (int j = 1; j <= BoardSize; j++)
         if (value[i][j] == Blank || conflicts[i][j] > 0)
            return false;

   return true;
}
bool board::findBlank(int &row, int &col)
{
   for (int i = 1; i <= BoardSize; i++)
   {
      for (int j = 1; j <= BoardSize; j++)
      {
         if (value[i][j] == Blank)
         {
            row = i;
            col = j;
            return true;
         }
      }
   }
   return false; // no blanks left
}

int main()
{
   ifstream fin;

   string fileName = "sudoku1.txt";

   fin.open(fileName.c_str());
   if (!fin)
   {
      cerr << "Cannot open " << fileName << endl;
      exit(1);
   }

   try
   {
      board b1(SquareSize);

      while (fin && fin.peek() != 'Z')
      {
         b1.initialize(fin);
         b1.print();
         b1.printConflicts();

         if (b1.isSolved())
            cout << "Board is solved.\n";
         else
            cout << "Board is not solved.\n";
      }
   }
   catch (indexRangeError &ex)
   {
      cout << ex.what() << endl;
      exit(1);
   }
   catch (rangeError &ex)
   {
      cout << ex.what() << endl;
      exit(1);
   }
}
