// N-QUEEN PUZZLE SOLUTIONS
// by John Hutchinson
// Aug. 10, 2012

#include <iostream>   // For cin / cout
#include <vector>     // Using a vector as a stack (they are easier to iterate)

// These three used for file output
#include <fstream>    // For file output stream
#include <string>     // used for constructing the filename
#include <sstream>    // used to convert n to a string (as part of the filename)

using namespace std;

/*
Upper limit of N = 14 because the output for all solutions when N > 14
will overflow the file stream, resulting in incomplete solution sets saved
to the file.
The 4-Queen Puzzle has only 2 solutions.
The 8-Queen Puzzle has 92 solutions.
The 14-Queen Puzzle has 365596 solutions (over 600mb of text file data)!
*/
#define MAX_QUEEN 14

// Global Chess Board.  This is not used for computation, only to draw the
// board to the screen
bool board[MAX_QUEEN][MAX_QUEEN];

// These three functions are used only to build the chess board from
// the stack and then output to a file
void clearBoard(int n);
void placePiecesOnBoard(vector<int> &rows);
void drawBoard(int n, unsigned long int solutionNum, ofstream &file);

// Finds the ALL solution to the N-Queen Puzzle.  I used a non-recursive
// algorithm to speed up computation and avoid reaking havoc on the internal
// stack. This also allows the puzzles to be solved when n is large, since
// system stack overflow is not a concern.
void findSolutions(int n);

// Used by FindSolution(). Returns false if last queen on the stack is on the
// same row or diagonal as any other queen.  We don't need to check columns
// because no more than one queen is ever added to each column.
bool lastPieceIsValid(int n, vector<int> &rows);

int main()
{
    int n;
    char c;

    do
    {
        cout << "-= Finds the ALL solutions to an N-Queen Puzzle =-\n";
        cout << "-= WARNING: N > 12 will take a long time to process! =-\n";
        cout << "-= N = 14 took about 4 minutes on my computer =-\n\n";

        do
        {
            cout << "\tEnter an integer N such that 3 < N < " << MAX_QUEEN + 1;
            cout << "\nN = ";
            cin >> n;
            if (n < 4 || n > MAX_QUEEN)
                cout << "INVALID!\n";
        } while (n < 4 || n > MAX_QUEEN);

        findSolutions(n);

        cout << "Solve another N-Queen Puzzle (Y/N) ? ";
        cin >> c;

        cout << "\n\n";
    } while (c != 'N' && c != 'n');

    system("PAUSE");
    return 0;
}

void outputBoard(int n, unsigned long int solutionNum, ofstream &out)
{       
    int i, j, q;
    out << "\nSolution #" << solutionNum;
    out << " to the " << n << "-Queen Puzzle:\n";

    // Draw Top of Board
    for (q = 0; q < n; q++) 
        out << "|---";
    out << "|\n";

    // Draw Rest of Board
    for (i = 0; i < n; i++)
    {
        // Draw Middle Board, Including Any Queens
        for (j = 0; j < n; j++)
        {
            out << "|";
            if (board[j][i])
                out << " Q ";
            else
                out << "   ";
        }

        // Draw Row Dividers, Including Bottom of Board
        out << "|\n";
        for (q = 0; q < n; q++)
            out << "|---";
        out << "|\n";
    }
    out << endl;
}

// Set entire chess board to false values
void clearBoard(int n)
{
    int i, j;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            board[i][j] = false;
}

// Loop through the stack of Queens and set their positions
// on the chess board
void placePiecesOnBoard(vector<int> &rows)
{
	for(size_t i = 0; i < rows.size(); i++)
        board[i][rows[i]] = true;
}

bool lastPieceIsValid(int n, vector<int> &rows)
{
    int row = rows.back();
    int col = rows.size() - 1;

    // Check if last queen is beyond the chess board
    if (row >= n)
		return false;

    // Otherwise, check position validity against the other queens
    for(size_t i = 0; i < rows.size() - 1; i++)
    {
        // Check if on same row
        if (row == rows[i])
			return false;

        // check diagonals
        if (abs(row - rows[i]) == abs(col - int(i)))
			return false;
    }
    return true;
}

void findSolutions(int n)
{
    // using vector with push_back() / pop_back() as a stack
    vector<int> rows;
    bool done = false;
    unsigned long int solutions = 0;
    int row;

    // Open output file
    ofstream outfile;
    string filename;
    stringstream IntToStr;
    IntToStr << n;
    filename = IntToStr.str() + "-Queen.txt";;
    outfile.open(filename.c_str());

    // Place First Piece
    rows.push_back(0);

    cout << "\nWorking";

    // Loop until a all solutions are found
    while (!done)
    {
        // See if last queen is ok where it is
        if (!lastPieceIsValid(n, rows))
        {
            // Last position is bad, find last valid piece

            row = rows.back();  // Record last row tried

            // If it's out of bounds, backtrack to last vaild piece
            if (row >= n)
            {
                do
                { 
                    // make sure we haven't found all solutions already                  
                    if (rows.size() > 1)
                    {
                        rows.pop_back();        // Remove invalid piece
                        row = rows.back();      // Record row of last good piece
                        rows.pop_back();        // Remove that piece...
                        rows.push_back(row + 1);    // And now try the next row.
                        row = rows.back();      // See if that row is in bounds
                    }
                    else    // all solutions have been found
                    {
                        done = true;
                        break;  // exit loop
                    }
                    // continue backtracking as needed
                } while (rows.back() >= n);
            }
            else    // It was within bounds but still bad.  Try the next row.
            {
                rows.pop_back();
                rows.push_back(row + 1);
            }
        }
        else    // last queen is ok where it is
        {
            // See if we found a solution
            if (rows.size() == n)
            {
                solutions++;
                clearBoard(n);
                placePiecesOnBoard(rows);
                outputBoard(n, solutions, outfile);

                // Continue finding more soltions (if any)
                row = rows.back();
                rows.pop_back();
                rows.push_back(row + 1);

                // Output a tick-mark once in a while so the user knows
                // that the program is "Working..."
                if (solutions % (n * n) == 0)
					cout << ".";
            }
            // If not, proceed to the next column
            else rows.push_back(0);
        }
    } 

    outfile.close();

    cout << "\n\n\n===== ALL " << solutions << " SOLUTIONS FOUND! =====\n";
    cout << "(See " << n << "-Queen.txt for results)\n\n";
}