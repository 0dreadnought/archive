#include <stdio.h>
#include <time.h>
#include <stdlib.h>

const int size_rows = 30;
const int size_cols = 30;
const int neighbourSize = 1;

int steepestHillClimbing(int A[size_rows][size_cols], int maxIter);
void ReadData(int B[size_rows][size_cols]);

int main(void)
{
    int matrix[size_rows][size_cols];

    ReadData(matrix);

    printf("Highest number found: %d\n", steepestHillClimbing(matrix, 100));

    return 0;
}

void ReadData(int B[size_rows][size_cols])
{
    FILE* myfile;
    myfile = fopen("matrix_30x30.txt", "r");
    for (int i = 0; i < size_rows; i++)
    {
        for (int j = 0; j < size_cols; j++)
        {
            fscanf(myfile, "%d", &B[i][j]);
        }
    }
    fclose(myfile);
}

int steepestHillClimbing(int A[size_rows][size_cols], int maxIter)
{
    srand(time(NULL));
    int row = rand() % size_rows, col = rand() % size_cols; // limit the boundary 
    int solution = A[row][col]; // generated random location for the solution 
    int flag = 1, iter = 0, best = -1, best_row, best_col;

    printf("Starting coord is [%d, %d] with value %d.\n", row + 1, col + 1, solution);

    while (iter < maxIter && flag)
    {
        flag = 0;

        for (int rows = -neighbourSize; rows <= neighbourSize; rows++)
        {
            for (int cols = -neighbourSize; cols <= neighbourSize; cols++)
            {
                // be aware of the boundaries
                if ((row + rows < size_rows) && (col + cols < size_cols) &&
                    (row + rows > -1) && (col + cols > -1))
                {
                    if (A[row + rows][col + cols] > solution)
                    {
                        if (A[row + rows][col + cols] > best)
                        {
                            best_row = row + rows; // update with new location
                            best_col = col + cols; // update with new location
                            best = A[best_row][best_col];
                            flag = 1;
                        }
                    }
                }
            }
        }
        if (flag)
        {
            solution = A[best_row][best_col];
            row = best_row;
            col = best_col;
        }
        iter++;
    }
    printf("We visited %d cells to find that [%d, %d] with value %d was the best.\n", iter, row + 1, col + 1, A[row][col]);

    return solution;
}

/*

# Steepest Ascent Hill climbing algorithm

SteepestAscentHillClimbing(searchSpace, maxIter) {
    solution = random state from searchSpace
    flag = 1 // flag is raised if next solution was found
    while maxIter not reached and flag {
        flag = 0
        best = 0
        for each neighbour of solution {
            if neighbour is better than solution and better than best {
                best = neighbour
                flag = 1
            }
        }
        if flag // a new solution was identified
        {
            solution = best
        }
    }
    return solution
}
*/