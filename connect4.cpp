#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>
using namespace std;


/*  universal variables */
bool isGameOver = false;
char board [6][7] = { // 0,0 -->       -->         -->   0,6
         /* 0,0 */      {' ', ' ', ' ', ' ', ' ', ' ', ' '},
                        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
                        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
                        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
                        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        /* 5,0 */       {' ', ' ', ' ', ' ', ' ', ' ', ' '}
                    };

char boardWin [6][7] = { // 0,0 -->       -->         -->   0,6
         /* 0,0 */      {' ', ' ', ' ', ' ', ' ', ' ', ' '},
                        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
                        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
                        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
                        {' ', ' ', ' ', ' ', ' ', ' ', ' '},
        /* 5,0 */       {' ', ' ', ' ', ' ', ' ', ' ', ' '}
                    };

const float numerator = 10.0;

/*  datatype made specifically for checkDirection method
    dir - direction relative to source tile that was checked for a matching char
    row - the row of the specific tile that was checked
    col - the column of the specific tile that was checked 
    b   - bool that represents if the function found a match */
struct boolInt {
    int dir;
    int row;    //row that was checked
    int col;    //col that was checked
    bool b;
};

/*  FUNCTIONS */
    
/*  Modified sigmoid function that determines how far the bot will loook ahead. 
    Will output INTEGER VALUE between 0 and 10 (inclusive) */
int sigmoid(int x) {

    float denom = 1.0 + exp((-0.5) * (x - 10));

    return round(numerator/denom);

}




/*  clears console so that I don't spam hundreds of messages and new boards
    source: https://stackoverflow.com/a/32008479 */
void clearscreen() {
    cout << "\033[2J\033[1;1H";
}

/*  prints out initial grid when program is started */
void printgrid() {

    clearscreen();

    cout << "__ 1 ___ 2 ___ 3 ___ 4 ___ 5 ___ 6 ___ 7 __" << endl;
    cout << "|     |     |     |     |     |     |     |" << endl;
    cout << "|_____|_____|_____|_____|_____|_____|_____|" << endl;
    cout << "|     |     |     |     |     |     |     |" << endl;
    cout << "|_____|_____|_____|_____|_____|_____|_____|" << endl;
    cout << "|     |     |     |     |     |     |     |" << endl;
    cout << "|_____|_____|_____|_____|_____|_____|_____|" << endl;
    cout << "|     |     |     |     |     |     |     |" << endl;
    cout << "|_____|_____|_____|_____|_____|_____|_____|" << endl;
    cout << "|     |     |     |     |     |     |     |" << endl;
    cout << "|_____|_____|_____|_____|_____|_____|_____|" << endl;
    cout << "|     |     |     |     |     |     |     |" << endl;
    cout << "|_____|_____|_____|_____|_____|_____|_____|" << endl;

}

/*  overloaded printgrid method, takes universal array as input 
    clears console first, then prints contents of the array into the grid */
void printgrid(char arr[6][7], bool clear) {

    if (clear) {
        clearscreen();
    }

    cout << "__ 1 ___ 2 ___ 3 ___ 4 ___ 5 ___ 6 ___ 7 __" << endl;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            printf("|  %c  ", arr[i][j]);
        }

        printf("|\n");
        cout << "|_____|_____|_____|_____|_____|_____|_____|" << endl;

    }

}

/*  function that sums up the number of NON-EMPTY PIECES 
    to be used as the input into sigmoid() */

int numberPieces() {

    int count = 0;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            if (board[i][j] != ' ') {
                count++;
            }
        }
    }
    return count;

}

/* simple method to take user input, to choose which channel to "drop" their piece into */
int userTurn() {
    cout << "Connect 4, choose a slot to play your piece:" << endl;
    int input;

    cin >> input;

    return input - 1;
}
/*  Overloaded userTurn method to not print out a new prompt line */
int userTurn(string s) {

    int input;

    cin >> input;

    return input - 1;
}

/*  checks if position is valid (does [row] [col] exist on the grid?) */
bool isPosValid(int row, int col) {

    if ((row >= 0 && row < 6) && (col >= 0 && col < 7)) {
        return true;
    } else {
        return false;
    }

}

/*  returns true if column col is full */
bool isColumnFull(int col) {

    for (int i = 0; i <= 6; i++) {

        if (board[i][col] == ' ') {
            return false;
        }

    }

    return true;

}

/* check adjacent tiles in certain directions (POI = point of interest)
                 7    0    1
                   ↖  ↑  ↗
                6 <- POI ->  2
                   ↙  ↓  ↘
                  5   4    3

REMEMBER, top left is (row,col) = (0,0), to move right, col + 1, to move down, row + 1

from tile at [ROW] and [COL], see if character [c] is in [direction] direction
returns true, a direction int, and the row and col of the value checked if
the tile in that direction/at that index matches with the POI

returns false, a direction, and the row/col checked if the tile in that direction
exists, but does not match the POI

returns false with direction -1, and row, col -1, -1 if invalid. 
returns this case if the tile in that direction DOES NOT EXIST


This function is meant to be used to check for game wins AS PIECES ARE BEING PLAYED.
Unfortunately that does not mesh well with how the minimax function operates.

Maybe just use this for checking if the user won?
*/
boolInt checkDirection(int row, int col, char c, int direction) {

    int row1, col1 = 0;

    switch(direction) {
        case 0:     //directly up
            row1 = row - 1;
            col1 = col;
            break;
        
        case 1:     //up and to the right
            row1 = row - 1;
            col1 = col + 1;
            break;
        
        case 2:     //to the right
            row1 = row;
            col1 = col + 1;
            break;

        case 3:     //down and to the right
            row1 = row + 1;
            col1 = col + 1;
            break;

        case 4:     //down
            row1 = row + 1;
            col1 = col;
            break;

        case 5:     //down and to the left
            row1 = row + 1;
            col1 = col - 1;
            break;

        case 6:     //to the left
            row1 = row;
            col1 = col - 1;
            break;

        case 7:     //up and to the left
            row1 = row - 1;
            col1 = col - 1;
            break;
    }

    if(isPosValid(row1, col1)) {
        if (c == board[row1][col1]) {
            boolInt result = {direction, row1, col1, true};
            return result;
        } else {
            boolInt result = {direction, row1, col1, false};
            return result;
        }
    } else {
        boolInt result = {-1, -1, -1, false};
        return result;
    }

}

/*  reverses the direction passed as parameter (180 degree turn) */
int reverseDirection(int dir) {
    if (dir < 4) {
        return dir + 4;
    } else {
        return dir - 4;
    }
}
/*  searches in [direction]'s direction, and finds how many consecutive c there are */
int innaRow(int row, int col, char c, int direction) {
    int solution = 1;  
    boolInt check = checkDirection(row, col, c, direction);

    if (check.b) {
        solution++;
        solution += innaRow(check.row, check.col, c, direction);
        return solution;
    } else {
        return 0;
    }
}

/*  return if the game has been won by putting char c in position row and col

    start checking all directions (excluding up, for obv reasons)
        once one direction is fully exhausted, check the polar opposite direction
        to see if there is a possibility of more consecutive pieces */
bool gameWon(char c, int row, int col) {

    for (int dirs = 0; dirs < 8; dirs++) {

        int count = 0;
        count = count + innaRow(row, col, c, dirs); //check in initial direction

        count += innaRow(row, col, c, reverseDirection(dirs)); //check in opposite direction (if placed piece is in middle)

        if (count >= 5) { //self is counted twice
            return true;
        }
    }

    return false;

}

/*  Given a character to place, and a starting row and column, 
    "gravity" will place the piece in the lowest available open position
    uses recursion to search the spot below current slot to check if
        a) it exists
        b) is it open or taken? */
void determinePos(char arr[6][7], char c, int row, int col) {

    if(row == 0 && isColumnFull(col)) {
        cout << "That column is full, pick another one: " << endl;
        determinePos(arr, c, 0, userTurn("full"));
        return;
    }

    if (row == 5) { //base case, if piece is put into empty column

        arr[row][col] = c;


    } else if (arr[row + 1][col] != ' ') {
        arr[row][col] = c;

    } else {
        determinePos(arr, c, row + 1, col);
    }

}

/*  start of minimax stuff */

/*  Iterates through the whole board, at first sight of ' ' (space/empty),
    return true; there are still moves that can be played */
bool isMovesLeft(char arr[6][7]) {

    for (int i = 0; i < 5; i++) {

        for (int j = 0; j < 6; j++) {

            if(arr[i][j] == ' ') {
                return true;
            }

        }
    }

    return false;

}

/*  given array of the game state, find a winner.
        10 if x wins,
        -10 if o wins,
        0 if there is no winner yet.
        
    Manually checks the whole array, becuase no row/col parameters are passed.
    This results in longer search speeds, at the expense of having minimax work. */
int eval(char arr[6][7], bool print) {

    int count = 0;
    char match = 'z';
    //check for horizontal winners (row stays the same, col changes)
    for (int i = 0; i < 6; i++) {

        count = 0;

        for (int j = 0; j < 7; j++) {

            if(arr[i][j] == ' ') {
                count = 0;
            }

            if(arr[i][j] != ' ' && arr[i][j] == match) {
                count++;
            } else if (arr[i][j] != ' ') {

                count = 1;
                match = arr[i][j];

            }

            if (count == 4) {
                if (match == 'x') {
                    if (print) {
                        cout << "x horizontal win " << i << ", " << j << endl;
                    }

                    return 10;

                } else if (match == 'o') {
                    if (print) {
                        cout << "o horizontal win " << i << ", " << j << endl;
                    }

                    return -10;
                }
            }

        }

    }

    //check for vertical winners (row changes, col stays the same)
    for (int j = 0; j < 7; j++) {

        count = 0;

        for (int i = 0; i < 6; i++) {

            if(arr[i][j] == ' ') {
                count = 0;
            }

            if(arr[i][j] != ' ' && arr[i][j] == match) {
                count++;
            } else if (arr[i][j] != ' ') {

                count = 1;
                match = arr[i][j];

            }

            if (count == 4) {
                if (match == 'x') {
                    if (print) {
                        cout << "x vertical win " << i << ", " << j << endl;
                    }

                    return 10;

                } else if (match == 'o') {
                    if (print) {
                        cout << "o vertical win " << i << ", " << j << endl;
                    }

                    return -10;
                }
            }

        }

    }

    //check for diagonal (top left to bottom right)
    for (int i = 2; i > -1; i--) {

        for (int j = 0; j < 4; j++) {

            for(int k = 1; k < 4; k++) {

                match = arr[i][j];

                if(match != arr[i + k][j + k]) {

                    break;

                }

                if (k == 3) {
                    if (match == 'x') {
                        if (print) {
                            cout << "x diag ↘ win " << i << ", " << j << endl;
                        }

                        return 10;
                    } else if (match == 'o') {
                        if (print) {
                            cout << "o diag ↘ win " << i << ", " << j << endl;
                        }

                        return -10;
                    }
                }


            }


        }

    }

    //check for diagonal (top right to bottom left)
    for (int i = 2; i > -1; i--) {

        for (int j = 3; j < 7; j++) {

            for(int k = 1; k < 4; k++) {

                match = arr[i][j];

                if(match != arr[i + k][j - k]) {

                    break;

                }

                if (k == 3) {
                    if (match == 'x') {
                        if (print) {
                            cout << "x diag ↙ win " << i << ", " << j << endl;
                        }

                        return 10;
                    } else if (match == 'o') {
                        if (print) {
                            cout << "o diag ↙ win " << i << ", " << j << endl;
                        }

                        return -10;
                    }
                }


            }


        }

    }

    return 0;
}

int finaleval(char arr[6][7], bool print) {

    int count = 0;
    char match = 'z';
    //check for horizontal winners (row stays the same, col changes)
    for (int i = 0; i < 6; i++) {

        count = 0;

        for (int j = 0; j < 7; j++) {

            if(arr[i][j] == ' ') {
                count = 0;
            }

            if(arr[i][j] != ' ' && arr[i][j] == match) {
                count++;
            } else if (arr[i][j] != ' ') {

                count = 1;
                match = arr[i][j];

            }

            if (count == 4) {
                if (match == 'x') {
                    if (print) {
                        cout << "x horizontal win " << i << ", " << j << endl;
                    }

                    boardWin[i][j - 3] = 'x';
                    boardWin[i][j - 2] = 'x';
                    boardWin[i][j - 1] = 'x';
                    boardWin[i][j] = 'x';

                    return 10;

                } else if (match == 'o') {
                    if (print) {
                        cout << "o horizontal win " << i << ", " << j << endl;
                    }

                    boardWin[i][j - 3] = 'o';
                    boardWin[i][j - 2] = 'o';
                    boardWin[i][j - 1] = 'o';
                    boardWin[i][j] = 'o';

                    return -10;
                }
            }

        }

    }

    //check for vertical winners (row changes, col stays the same)
    for (int j = 0; j < 7; j++) {

        count = 0;

        for (int i = 0; i < 6; i++) {

            if(arr[i][j] == ' ') {
                count = 0;
            }

            if(arr[i][j] != ' ' && arr[i][j] == match) {
                count++;
            } else if (arr[i][j] != ' ') {

                count = 1;
                match = arr[i][j];

            }

            if (count == 4) {
                if (match == 'x') {
                    if (print) {
                        cout << "x vertical win " << i << ", " << j << endl;
                    }

                    boardWin[i - 3][j] = 'x';
                    boardWin[i - 2][j] = 'x';
                    boardWin[i - 1][j] = 'x';
                    boardWin[i][j] = 'x';

                    return 10;
                    
                } else if (match == 'o') {
                    if (print) {
                        cout << "o vertical win " << i << ", " << j << endl;
                    }

                    boardWin[i - 3][j] = 'o';
                    boardWin[i - 2][j] = 'o';
                    boardWin[i - 1][j] = 'o';
                    boardWin[i][j] = 'o';

                    return -10;
                }
            }

        }

    }

    //check for diagonal (top left to bottom right)
    for (int i = 2; i > -1; i--) {

        for (int j = 0; j < 4; j++) {

            for(int k = 1; k < 4; k++) {

                match = arr[i][j];

                if(match != arr[i + k][j + k]) {

                    break;

                }

                if (k == 3) {
                    if (match == 'x') {
                        if (print) {
                            cout << "x diag ↘ win " << i << ", " << j << endl;
                        }

                        boardWin[i + 3][j + 3] = 'x';
                        boardWin[i + 2][j + 2] = 'x';
                        boardWin[i + 1][j + 1] = 'x';
                        boardWin[i][j] = 'x';

                        return 10;
                    } else if (match == 'o') {
                        if (print) {
                            cout << "o diag ↘ win " << i << ", " << j << endl;
                        }

                        boardWin[i + 3][j + 3] = 'o';
                        boardWin[i + 2][j + 2] = 'o';
                        boardWin[i + 1][j + 1] = 'o';
                        boardWin[i][j] = 'o';

                        return -10;
                    }
                }


            }


        }

    }

    //check for diagonal (top right to bottom left)
    for (int i = 2; i > -1; i--) {

        for (int j = 3; j < 7; j++) {

            for(int k = 1; k < 4; k++) {

                match = arr[i][j];

                if(match != arr[i + k][j - k]) {

                    break;

                }

                if (k == 3) {
                    if (match == 'x') {
                        if (print) {
                            cout << "x diag ↙ win " << i << ", " << j << endl;
                        }

                        boardWin[i + 3][j - 3] = 'x';
                        boardWin[i + 2][j - 2] = 'x';
                        boardWin[i + 1][j - 1] = 'x';
                        boardWin[i][j] = 'x';

                        return 10;
                    } else if (match == 'o') {
                        if (print) {
                            cout << "o diag ↙ win " << i << ", " << j << endl;
                        }

                        boardWin[i + 3][j - 3] = 'o';
                        boardWin[i + 2][j - 2] = 'o';
                        boardWin[i + 1][j - 1] = 'o';
                        boardWin[i][j] = 'o';

                        return -10;
                    }
                }


            }


        }

    }

    return 0;
}

/*  given current game state (array of played pieces), and [ROW],[COL] of last piece played, check if it won.
    If that piece won, return a score depending on the piece.
    
    Initial version if eval, just makes use of gameWon function. Does not work with minimax function */
int eval(char arr[6][7], int row, int col) {

    char piece = arr[row][col];
    if (gameWon(piece, row, col)) {

        if (piece == 'x') {
            return 10;
        } else if (piece == 'o'){
            return -10;
        }

    }

    return 0;

}


/*  minimax function:
        parameters:
            arr     - array of game state (board with the pieces on it)
            depth   - how many times minimax has been recursively called
            isMax   - true represents maximizer, false represents minimizer
            alpha   - parameter for alpha beta pruning: the best value that the maximizer currently can guarantee at that level or above
            beta    - parameter for alpha beta pruning: the best value that the minimizer currently can guarantee at that level or below 
            
        returns:
            int     - "value" of the board 
            
        IMPORTANT:  First if statement in function determines how "far" the function will look into the future.
                    At the start of the game, there are so many possible moves and end game states that calculations
                    take far too long, so I shortened the lookahead of the "bot" */
int minimax(char arr[6][7], int depth, bool isMax, int alpha, int beta) {

//sigmoid(numberPieces())

    if (depth > 3) {

        return 0;

    }

    int score = eval(arr, false);          //  evaluate current board

    char before[6][7];
    copy(&arr[0][0], &arr[0][0]+6*7*sizeof(char), &before[0][0]);

    if (score == 10) {              //  if maximizer won, return their score (10)
        return score - depth;               //  maximizer = x
    } else if (score == -10) {      //  if minimizer wins, return their score (-10)
        return score + depth;               //  minimizer = o
    } else if (!isMovesLeft(arr)) { //  if there are no more moves left, score = 0 (tie)
        return 0;
    }

    

    if (isMax) {
        int best = -10000;
        
        /*  explore all options (you have 7 choices, you can only play the
            lowest available slots. Remember: pieces "fall" to the lowest
            available free slot.) */

        for (int i = 0; i < 7; i++) {
            if (!isColumnFull(i)) {
                determinePos(arr, 'x', 0, i);
                best = max(best, minimax(arr, depth + 1, false, alpha, beta));
                alpha = max(alpha, best);

                if (beta <= alpha) {
                    return best;
                }

                //undo the move!
                copy(&before[0][0], &before[0][0]+6*7*sizeof(char), &board[0][0]);
            }
        }
//        cout << "x best move calc'd" << endl;
        return best;
    } else { //if minimizer's move
        int best = 10000;

        for (int i = 0; i < 7; i++) {
            if (!isColumnFull(i)) {
                determinePos(arr, 'o', 0, i);
                best = min(best, minimax(arr, depth + 1, true, alpha, beta));    

                beta = min(beta, best);

                if (beta <= alpha) {
                    return best;
                }
//                cout << "depth: " << depth << endl;
                //undo the move!
                copy(&before[0][0], &before[0][0]+6*7*sizeof(char), &board[0][0]);
            }
        }

//        cout << "o best move calc'd" << endl;
        return best;

    }


}

/*  partner function to minimax:
    given the current game state (board): function will return the best possible move
    
    If there are too little pieces on the board, the "bot" will randomly generate a move
    returns index of the move the bot should make */
int findBestMove(char arr[6][7]) {

    int solution = -1;  //column of the best move
    int best = -10000;

    int thresh = sigmoid(numberPieces());

    // if (thresh < 4) {

    //     solution = rand() % 7;
    //     return solution;

    // }


    for (int i = 0; i < 7; i++) {

        if(!isColumnFull(i)) {

            char before[6][7];
            copy(&board[0][0], &board[0][0]+6*7*sizeof(char), &before[0][0]);

            determinePos(board, 'x', 0, i);

            int value = minimax(board, 0, false, -10000, 10000);

            copy(&before[0][0], &before[0][0]+6*7*sizeof(char), &board[0][0]);

            if (value > best) {

                solution = i;
                best = value;

            }


        }

    }

    return solution;

}




/*TODO:
find better way to integrate the bot difficulty ramping up, its technically possible to win */


int main()
{

    srand(time(NULL)); //seed randomness based on time

    printgrid(board, true);

    while(!isGameOver) {
        determinePos(board, 'o', 0, userTurn());
        printgrid(board, true);
        

        if (eval(board, true) != 0) {
                isGameOver = true;
                finaleval(board, false);
                printgrid(boardWin, false);
        }

        if (isGameOver) {
            break;
        }

        determinePos(board, 'x', 0, (findBestMove(board)));
        printgrid(board, true);

        if (eval(board, true) != 0) {
                isGameOver = true;
                finaleval(board, false);
                printgrid(boardWin, false);
        }
    }

    return 0;
}
