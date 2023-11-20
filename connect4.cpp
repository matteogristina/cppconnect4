#include <iostream>
#include <stdlib.h>
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

/*  datatype made specifically for checkDirection method
    dir - direction relative to source tile that was checked for a matching char
    row - the row of the specific tile that was checked
    col - the column of the specific tile that was checked */
struct boolInt {
    int dir;
    int row;    //row that was checked
    int col;    //col that was checked
    bool b;
};

/*  clears console so that I don't spam 40 new boards
    source: https://stackoverflow.com/a/32008479 */
void clearscreen() {
    cout << "\033[2J\033[1;1H";
}

//prints out initial grid when program is started
void printgrid() {

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

//overloaded printgrid method, takes universal array as input
void printgrid(char arr[6][7]) {

    clearscreen();

    cout << "__ 1 ___ 2 ___ 3 ___ 4 ___ 5 ___ 6 ___ 7 __" << endl;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            printf("|  %c  ", arr[i][j]);
        }

        printf("|\n");
        cout << "|_____|_____|_____|_____|_____|_____|_____|" << endl;

    }

}

//simple method to take user input, to choose which channel to "drop" their piece into
int userTurn() {
    cout << "Connect 4, choose a slot to play your piece:" << endl;
    int input;

    cin >> input;

    return input - 1;
}

int userTurn(string s) {

    int input;

    cin >> input;

    return input - 1;
}

//checks if position is valid (does [row] [col] exist on the grid?)
bool isPosValid(int row, int col) {

    if ((row >= 0 && row < 6) && (col >= 0 && col < 7)) {
        return true;
    } else {
        return false;
    }

}

/* check adjacent tiles in certain directions (POI = point of interest)
                 7    0    1
                   ↖  ↑  ↗
                6 <- POI ->  2
                   ↙  ↓  ↘
                  5   4    3

REMEMBER, TOP left is (row,col) = (0,0), to move right, col + 1, to move down, row + 1

from tile at [ROW] and [COL], see if [c] is in [direction] direction
 returns true, and a direction int if the tile in that direction
matches with the given tile

returns false and a direction if the tile in that direction exists,
but does not match

returns false and -1 if the tile in that direction DOES NOT EXIST

last 2 values returned are the row, col that was checked. -1, -1 if invalid 

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

//reverses the direction passed as parameter (180 degree turn)
int reverseDirection(int dir) {
    if (dir < 4) {
        return dir + 4;
    } else {
        return dir - 4;
    }
}
//searches in [direction]'s direction, and finds how many consecutive c there are
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

//return if the game has been won by putting char c in position row and col
bool gameWon(char c, int row, int col) {

    //start checking all directions (excluding up, ofr obv reasons)
        //if you get a hit, count = 2, keep moving in that direction
            //if you get another hit, count = 3, keep moving in that direction
                //if you get another hit, count = 4, game has been won
            //if you dont get a hit, count still = 2, check polar opposite direction
    //if there are no hits, the game is not won


    for (int dirs = 0; dirs < 8; dirs++) {

        int count = 0;
        count = count + innaRow(row, col, c, dirs); //check in initial direction
//        cout << count << endl;

        count += innaRow(row, col, c, reverseDirection(dirs)); //check in opposite direction (if placed piece is in middle)
       
//            cout << "count: " << count << "in dir: " << dirs << endl;

        if (count >= 5) { //self is counted twice
            return true;
        }
    }

    return false;

}

bool isColumnFull(int col) {

    for (int i = 0; i <= 6; i++) {

        if (board[i][col] == ' ') {
            return false;
        }

    }

    return true;

}

/*  Given a character to place, and a starting row and column, 
    "gravity" will place the piece in the lowest available open position
    uses recursion to search the spot below current slot to check if
        a) it exists
        b) is it open or taken? */
void determinePos(char c, int row, int col) {

    if(row == 0 && isColumnFull(col)) {
        cout << "That column is full, pick another one: " << endl;
        determinePos(c, 0, userTurn("full"));
        return;
    }

    if (row == 5) { //base case, if piece is put into empty column

        board[row][col] = c;
        if (gameWon(c, row, col)) {
            isGameOver = true;
        }

    } else if (board[row + 1][col] != ' ') {
        board[row][col] = c;
        if (gameWon(c, row, col)) {
            isGameOver = true;
        }
    } else {
        determinePos(c, row + 1, col);
    }

}

/* START OF MINIMAX ALGORITHM/DECISION MAKING 

    1. Are there moves to make?
    2. To find the best move, we use the minimax algorithm to weigh our options, and to choose the best one
    3. How do we measure how "good" each poossible move is?



*/

int main()
{

    printgrid();

    while(!isGameOver) {
    determinePos('x', 0, userTurn());
    printgrid(board);

    if (isGameOver) {
        break;
    }

    determinePos('o', 0, userTurn());
    printgrid(board);
    }


    return 0;
}
