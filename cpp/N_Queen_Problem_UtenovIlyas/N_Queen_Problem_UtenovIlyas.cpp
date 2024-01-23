/**
  * File:    N_Queen_Problem_UtenovIlyas.cpp
  *
  * Author:  Utenov Ilyas (ilyas2.utenov@live.uwe.ac.uk)
  * Date:     04/04/2023
  * Partner:  I worked alone
  * Course:   UWEBIC IY496 Artificial Intelligence
  *
  * Summary of File:
  *
  * This code is a C++ algorythm, that solves the n-queen problem using DFS. It asks user for N and prints out first solution found.
  *
  */

  //I confirm that this assignment is my own work. Where I have referred to academic sources, I have provided in-text citations and included the sources in the final reference list.

#include <iostream>
#include <cstdlib>

int board[10000];

/*
 bool isSafe(int row, int N)

 Summary of the isSafe function:
    The isSafe function checks verticaly and diagonally if there are any other queens threatening this position.

 Parameters: row: current row
             col: current column
 
 Return value: true or false

 Description:
    This function checks if current position is safe for queen to place.
*/

bool isSafe(int row, int col) {
    for (int i = 0; i < row; i++) {
        if (board[i] == col || abs(i - row) == abs(board[i] - col)) return false;
    }
    return true;
}

/*
 bool solveNQueens(int row, int N)

 Summary of the solveNQueens function:
    The solveNQueens is a recursive function that goes through chessboard row by row and tries to put a queen in every column.
 
 Parameters: row: current row
             N: dimensions of the board and number of queens
 
 Return value: true of false

 Description:
    This function tries to put a queen in every row with help of isSafe function, hence solves the problem.
*/

bool solveNQueens(int row, int N) {
    if (row == N) return true;
    for (int col = 0; col < N; col++) {
        if (isSafe(row, col)) {
            board[row] = col;
            if (solveNQueens(row + 1, N)) return true;
        }
    }
    return false;
}

/*
 void printBoard(int N)

 Summary of printBoard function:
    The printBoard function displays final solution.

 Parameters: N: dimensions of the board and number of queens

 Return value: nothing

 Description:
    This function prints out solution to the problem
*/

void printBoard(int N) {
    std::cout << "\033[2J\033[H" << "Solution found:" << std::endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i] == j) std::cout << "Q ";
            else std::cout << ". ";
        }
        std::cout << std::endl;
    }
}

/*
 int main()
 Summmary of main funnction:
    The main function asks user to input a dimensions of the board and checks if the input is correct and calls other functions.

 Parameters: none

 Return value: nothing

 Description:
    This function gets from user dimensions of the board. The process will be repeated after solution is found.
*/

int main() {
    int N;
    do {
        std::cout << "Enter the size of the board:(min 4) ";
        std::cin >> N;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\033[2J\033[H" << "Wrong input." << std::endl;
        }
        else {
            if (N < 4) N = 4;
            std::cout << "\033[2J\033[H" << "Work in progress";
            if (solveNQueens(0, N))  printBoard(N);
        }
    } while (true);
    return 0;
}

// Replit link: https://replit.com/@1ukrainech1/NQueenproblem