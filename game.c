#include "game.h"

int board[SIZE][SIZE];
int currentPlayer;
int gameOver;
int history[MAX_HISTORY][3];
int historyCount;

void initGame()
{
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            board[i][j] = 0;
        }
    }
    currentPlayer = 1;
    gameOver = 0;
    historyCount = 0;
}

int placePiece(int row, int col)
{
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
        return 0;
    }
    if (gameOver) {
        return 0;
    }
    if (board[row][col] != 0) {
        return 0;
    }

    if (historyCount >= MAX_HISTORY) {
        return 0;
    }

    board[row][col] = currentPlayer;

    history[historyCount][0] = row;
    history[historyCount][1] = col;
    history[historyCount][2] = currentPlayer;
    historyCount++;

    if (checkWin(row, col, currentPlayer)) {
        gameOver = 1;
        return 1;
    }

    if (checkDraw()) {
        gameOver = 1;
        return 1;
    }

    currentPlayer = (currentPlayer == 1) ? 2 : 1;

    return 1;
}

int checkWin(int row, int col, int player)
{
    int dir[4][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };
    int d, count, r, c;

    for (d = 0; d < 4; d++) {
        count = 1;

        r = row + dir[d][0];
        c = col + dir[d][1];
        while (r >= 0 && r < SIZE && c >= 0 && c < SIZE && board[r][c] == player) {
            count++;
            r += dir[d][0];
            c += dir[d][1];
        }

        r = row - dir[d][0];
        c = col - dir[d][1];
        while (r >= 0 && r < SIZE && c >= 0 && c < SIZE && board[r][c] == player) {
            count++;
            r -= dir[d][0];
            c -= dir[d][1];
        }

        if (count >= 5) {
            return 1;
        }
    }

    return 0;
}

void undoMove()
{
    if (historyCount <= 0) {
        return;
    }

    historyCount--;
    int row = history[historyCount][0];
    int col = history[historyCount][1];
    int player = history[historyCount][2];

    board[row][col] = 0;

    currentPlayer = player;

    gameOver = 0;
}

void restartGame()
{
    initGame();
}

int checkDraw()
{
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                return 0;
            }
        }
    }
    return 1;
}
