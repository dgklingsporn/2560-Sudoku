# 2560-Sudoku
Some of the declarations and definitions for the board class are given to you. (Optional.
You can choose to write your own codes.) Add functions to the class that:
1. initialize the board, and update conflicts based on the given board,
2. print the board and the conflicts to the screen,
3. add a value to a cell, and update conflicts,
4. clear a cell, and update conflicts, and
5. check to see if the board has been solved (return true or false, and print the result to
the screen)
Choose one of the following approaches for maintaining the conflicts information: (1) Conflict
Counts Approach - store a vector of conflict counts for each cell; and (2) Improved Conflict
Counts Approach - for each row i and digit j, keep track of whether each digit j has been
placed in row i. Do the same for each column and each square. We will use this information
in part b of the project to write the Sudoku solver.
The code you submit should read each Sudoku board from the file one-by-one, print the
board and conflicts to the screen, and check to see if the board has been solved (all boards
will not be solved at this point).
