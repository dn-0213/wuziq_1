#ifndef GAME_H
#define GAME_H

#define SIZE 15
#define MAX_HISTORY 225

extern int board[SIZE][SIZE];

extern int currentPlayer;

extern int gameOver;

extern int history[MAX_HISTORY][3];

extern int historyCount;

void initGame();

int placePiece(int row, int col);

int checkWin(int row, int col, int player);

void undoMove();

void restartGame();

int checkDraw();

#endif
