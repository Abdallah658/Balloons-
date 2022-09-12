//Author: Trevor Konieczny & Abdallah Mohammad
//Date: 10/21/2021
//Description:
//  Functions that allow the user to do various things in the game.
//  We can display our board, pop elements from the board, and
//  various other functions.

/** #include statements... **/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "bpgame.h"

//Stores current game state and score/rows/columns.
typedef struct {
	char** gameBoard;
	int userScore;
	int numRows;
	int numCols;

} gameState;

//Using a stack for the undo function.
typedef struct {
	gameState* moves;
	int top;
	int size;

} gameStack;

//Struct with row and column.
typedef struct {
	int r;
	int c;
} Position;

//Current game state.
struct bpgame {
	gameState* currentGameState;
	gameStack* gameHistory;

    char** gameBoard;
    int userScore;
    int numRows;
    int numCols;
    int balloonsPop;
};

/*** IMPLEMENTATION OF bp_XXXX FUNCTIONS HERE  ****/
BPGame* bp_create(int nrows, int ncols) {

    srand(time(NULL));

    //Error checking bounds.
    if (1 > nrows || nrows > 40) {
        printf("Error: %dx%d is out of bounds!\n", nrows, ncols);
        return NULL;

    }
    if (1 > ncols || ncols > 40) {
        printf("Error: %dx%d is out of bounds!\n", nrows, ncols);
        return NULL;
    }

    BPGame* m = (BPGame*)malloc(sizeof(BPGame));

    m->numCols = ncols;
    m->numRows = nrows;
    m->userScore = 0;
    m->balloonsPop = 0;

    m->gameBoard = (char**)malloc(sizeof(char*) * nrows);

    char a[4] = { '^', '=', 'o', '+' };

    for (int i = 0; i < nrows; i++) {
        m->gameBoard[i] = (char*)malloc(sizeof(char) * ncols);
        for (int j = 0; j < ncols; j++) {
            m->gameBoard[i][j] = a[rand() % 4];
        }
    }
    return m;
}

//Create a matrix from a given board.
BPGame* bp_create_from_mtx(char mtx[][MAX_COLS], int nrows, int ncols) {
    srand(time(NULL));

    //Error checking bounds.
    if (1 > nrows || nrows > 40) {
        printf("Error: %dx%d is out of bounds!\n", nrows, ncols);
        return NULL;

    }
    if (1 > ncols || ncols > 40) {
        printf("Error: %dx%d is out of bounds!\n", nrows, ncols);
        return NULL;
    }

    BPGame* m = (BPGame*)malloc(sizeof(BPGame));

    m->numCols = ncols;
    m->numRows = nrows;
    m->userScore = 0;
    m->gameBoard = (char**)malloc(sizeof(char*) * nrows);

    char a[4] = { '^', '=', 'o', '+' };

    for (int i = 0; i < nrows; i++) {
        m->gameBoard[i] = (char*)malloc(sizeof(char) * ncols);

        for (int j = 0; j < ncols; j++) {
            m->gameBoard[i][j] = mtx[i][j];
        }
    }
    return m;
}

//Free our 2d array.
void bp_destroy(BPGame* b) {
    for (int i = 0; i < b->numRows; i++) {
        free(b->gameBoard[i]);
    }
    free(b);
}

//Displays our gameboard with correct format.
void bp_display(BPGame* b){
    printf("   +"); for (int i = 0; i < b->numCols * 2.01; i++) {
        printf("-");
    }printf("+");
    printf("\n");
    for (int i = 0; i < b->numRows; i++) {
        printf("%2d", i);  printf(" |");
        for (int j = 0; j < b->numCols; j++) {
            printf(" %c", b->gameBoard[i][j]);
        }
        printf(" |");
        printf("\n");
    }
    printf("   +"); for (int j = 0; j < b->numCols * 2.01; j++) { 
         printf("-");
         
    }printf("+\n");


    printf("     ");
    for (int i = 0; i < b->numCols; i++){
        if (i >= 10){
            int second = (i / 10) % 10;
            printf("%d ", second);
        }
        else{
            printf("%d ", 0);
        }
    }
    printf("\n");
    printf("     ");
    for (int i = 0; i < b->numCols; i++){
        if (i >= 10){
            int ans = i % 10;
            printf("%d ", ans);
        }
        else {
            printf("%d ", i);
        } 
    }
    printf("\nscore: %d\n\n", b->userScore);
}

//Helper function to use recursion to find matches.
int help(BPGame* b, int r, int c){
    char temp = b->gameBoard[r][c];

    b->gameBoard[r][c] = '.';
    b->balloonsPop++;

    int result = 0;
   
    //North
    if(r != 0){
        if (temp == b->gameBoard[r - 1][c]){
            b->gameBoard[r][c] = '.';

            result += help(b, r - 1, c);

        }
    }
    //East
    if(c != b->numCols-1){
        if (temp == b->gameBoard[r][c + 1]){
            b->gameBoard[r][c] = '.';

            result += help(b, r, c + 1);
        }
    }
    //South
    if(r != b->numRows-1){
        if (temp == b->gameBoard[r + 1][c]){
            b->gameBoard[r][c] = '.';

            result += help(b, r + 1, c);
        }
    }
    //West
    if(c != 0){
        if (temp == b->gameBoard[r][c - 1]){
            b->gameBoard[r][c] = '.';

            result += help(b, r, c - 1);
        }
    }
    return result;
}

//Pop the baloon at the given coordinate given there's a cluster.
int bp_pop(BPGame* b, int r, int c) {
    b->balloonsPop = 0;
    int result = 0;
    int count = 0;

    //Assign a temp value to user's coordinate.
    if(b->gameBoard[r][c] != '.'){
    char temp = b->gameBoard[r][c];

    //North
    if(r != 0){
        if (temp == b->gameBoard[r - 1][c]){
            b->gameBoard[r][c] = '.';
            if(count == 0){
            b->balloonsPop++;
            }
            count++;

            result += help(b, r - 1, c);
        }
    }
    //East
    if(c != b->numCols-1){
        if (temp == b->gameBoard[r][c + 1]){
            b->gameBoard[r][c] = '.';
            if(count == 0){
            b->balloonsPop++;
            }
            count++;

            result += help(b, r, c + 1);
        }
    }
    //South
    if(r != b->numRows-1){
        if (temp == b->gameBoard[r + 1][c]){
            b->gameBoard[r][c] = '.';
            if(count == 0){
            b->balloonsPop++;
            }
            count++;

            result += help(b, r + 1, c);
        }
    }
    //West
    if(c != 0){
        if (temp == b->gameBoard[r][c - 1]){
            b->gameBoard[r][c] = '.';
            if(count == 0){
            b->balloonsPop++;
            }
            count++;

            result += help(b, r, c - 1);
        }
    }
    }

    //Add to our score, and then reset the number of balloons popped.
    b->userScore += b->balloonsPop * (b->balloonsPop - 1);

    return b->balloonsPop;
}

//Checks to see if there is air above any balloons.
int bp_is_compact(BPGame* b) {
    //If the balloons have at least one air value above it, return zero.
    for (int r = 0; r < b->numRows-1; r++){
        for (int c = 0; c < b->numCols; c++){
            if (b->gameBoard[r][c] == '.' && b->gameBoard[r+1][c] != '.'){
                return 0;
            }
        }
    }
    //Otherwise, return 1.
    return 1;
}

//Moves all balloons up one above air.
void bp_float_one_step(BPGame* b) {
    for(int r = 0; r < b->numRows-1; r++){
        for(int c = 0; c < b->numCols; c++){
            if(b->gameBoard[r][c] == '.' && b->gameBoard[r+1][c] != '.'){
                char temp = b->gameBoard[r][c];
                b->gameBoard[r][c] = b->gameBoard[r+1][c];
                b->gameBoard[r+1][c] = temp;
            }
        }
    }
}

//Return the score.
int bp_score(BPGame* b){
    return b->userScore;
}

//Return the color of the balloon at the given coordinate.
int bp_get_balloon(BPGame* b, int r, int c) {
    //Make sure r or c isn't out of our bounds.
    if (r > b->numRows || r < 0) {
        return -1;
    }
    if (r > b->numCols || r < 0) {
        return -1;
    }

    return b->gameBoard[r][c];
    //return 0;
}

//Check to see if there are any remaining clusters.
int bp_can_pop(BPGame* b){
    //If at LEAST one match is found, return 1.
    for (int r = 0; r < b->numRows; r++){
        for (int c = 0; c < b->numCols; c++){
            if(b->gameBoard[r][c] != '.'){
                //North
                if (r != 0){
                    if (b->gameBoard[r][c] == b->gameBoard[r - 1][c]){
                        return 1;
                    }
                }
                //East
                if (c != b->numCols){
                    if (b->gameBoard[r][c] == b->gameBoard[r][c + 1]){
                        return 1;
                    }
                }
                //South
                if (r != b->numRows){
                    if (b->gameBoard[r][c] == b->gameBoard[r + 1][c]){
                        return 1;
                    }
                }
                //West
                if (c != 0){
                    if (b->gameBoard[r][c] == b->gameBoard[r][c - 1]){
                        return 1;
                    }
                }
            }
        }
    }

    //Otherwise, return zero.
    return 0;
}

//Function to undo our last operation in the game.
int bp_undo(BPGame* b) {


    return 0;
}

void bp_display_STD(BPGame *b) {
    for(int i = 0; i < b->numRows; i++){
        for (int j = 0; j < b->numCols; j++) {
            printf("%c", b->gameBoard[i][j]);
        }
        printf("\n");
    }
}