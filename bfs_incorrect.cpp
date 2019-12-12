#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
using namespace std;

/********************************************************************************
 Definitions & typedefs
 ********************************************************************************/
typedef char Cell;

const char
    EMPTY_CELL       = '.',
    PENGUIN_CELL     = 'p',
    PENGUIN_RESCUED  = 'P',
    FLAMINGO_CELL    = 'f',
    FLAMINGO_RESCUED = 'F',
    RESCUE_LOCATION  = 'x';

/********************************************************************************
 Structures
 ********************************************************************************/
/*
 * Part 1: structures
 */
struct Puzzle
{
    vector<Cell> board;
    int height;
    int width;
    int pos;
};
struct Candidate
{
    Puzzle  candidate;
    int     parent;
};

/********************************************************************************
 Functions
 ********************************************************************************/
/*
 * Part 1: displaying
 */
void display_puzzle(Puzzle puzzle)
{
    // Precondition:
    assert(puzzle.height > 0 && puzzle.width > 0);
    // Postcondition: will display given configuration in console.
    for(int r = 0, i = 0; r < puzzle.height; r++)
    {
        for(int c = 0; c < puzzle.width; c++)
        {
            cout << puzzle.board[i];
            i++;
        }
        cout << endl;
    }
}

/*
 * Files
 */
bool read_challenge_file(string filename, Puzzle& puzzle)
{
    // Precondition:
    assert(filename.length() > 0);
    // Postcondition: will read file and store it in given variable. Returns true only if file was opened successfully.
    ifstream handle;
    handle.open(filename);
    if(!handle.fail())
    {
        int
            row = 0,
            col = 0;
        string
            line;

        while(getline(handle, line))
        {
            col = 0;
            while(col < line.length())
            {
                puzzle.board.push_back(line[col]);
                col++;
            }
            row++;
        }
        puzzle.width    = col;
        puzzle.height   = row;

        handle.close();
        return true;
    }
    return false;
}

/*
 * Part 2: solving puzzle (breadth-first)
 */

bool is_valid_cell(Puzzle puzzle, int row, int col)
{
    return (row < puzzle.height && row >= 0 && col < puzzle.width && col >= 0);
}
bool operator==(const Candidate& c1, const Candidate& c2)
{
    if(c1.candidate.pos == c2.candidate.pos) return true;
    return false;
}

bool is_candidate_present(vector<Candidate> cands, Candidate current, bool printThis = false)
{
    for(int i = 0; i < cands.size(); i++)
    {
        if(cands[i] == current)
        {
            return true;
        }
    }
    return false;
}

bool is_candidate_successfull(Candidate c)
{
    if(c.candidate.board[c.candidate.pos] == RESCUE_LOCATION) return true;
    return false;
}

void solve_puzzle(Puzzle init)
{
    // Find the start node
    for(int b = 0; b < init.height*init.width; b++)
    {
        #if 0
        if(init.board[b] == FLAMINGO_CELL)
        {
            init.pos = b;
            cout << "Init flamingo position: " << b << endl;
            cout << "Height: " << init.height << endl;
            cout << "Width: " << init.width << endl;
        }
        if(init.board[b] == RESCUE_LOCATION)
        {
            cout << "Rescue position: " << b << endl;
        }
        #endif
    }

    // Variables
    vector<Candidate> cands = {{init, -1}}; // Create a vector of candidates (attempts-so-far). Add element with {init board, -1 as parent}.
    int i = 0; // Keep track of current attempt-candidate index.

    // All the possible directions
    int moves[4][2] = {
        {-1,0}, // north
        {1, 0}, // south
        {0, 1}, // east
        {0,-1}  // west
    };

    while(i < cands.size() && !is_candidate_successfull(cands[i]))
    {
        int
            // Convert current position to row & col numbers.
            row = cands[i].candidate.pos / cands[i].candidate.width,
            col = cands[i].candidate.pos - (row * cands[i].candidate.width);

        #if 0
        cout << "Current position: [" << row << "], [" << col << "]" << "(" << cands[i].candidate.pos << ")" << endl;
        #endif

        for(int l = 0; l < 4; l++)
        {
            // Try to move in all possible directions and see if the move is valid.
            int
                new_row = row + moves[l][0],
                new_col = col + moves[l][1];


            if(is_valid_cell(cands[i].candidate, new_row, new_col))
            {
                // Copy the current candidate and set the new position we just checked.
                Candidate current       = cands[i];
                current.parent          = i;
                int pos = current.candidate.pos   = new_col + new_row*current.candidate.width;

                // Because the move is valid, add it as a new candidate. We will check it later.
                if(!is_candidate_present(cands, current))
                {
                    cands.push_back(current);
                }
            }
        }

        i++; // Increase i, move to the next candidate if there is one
    }
    if(i < cands.size())
    {
        // display it
        cout << "Finished, path iterations: " << i << endl;
    }
}


/********************************************************************************
 Main procedure
 ********************************************************************************/
int main()
{
    Puzzle puzzle;
    #if 1
    if(read_challenge_file("challenge.5.5steps.txt", puzzle))
    {
        #if 1
        cout << "Starting with:" << endl;
        display_puzzle(puzzle);
        #endif

        #if 1
        solve_puzzle(puzzle);
        #endif
    }
    #endif
    return 0;
}
