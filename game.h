#ifndef GAME_H
#define GAME_H

#define BOARD_SIZE 15

#define EMPTY 0
#define BLACK 1
#define WHITE 2

extern int gameOver;

void initGame(void);

int placePiece(int row, int col);

void undoMove(void);

void restartGame(void);

int getCell(int row, int col);

int getCurrentPlayer(void);

int getWinner(void);

#endif