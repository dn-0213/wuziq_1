#include "game.h"

typedef struct {
    int row;
    int col;
} Move;

static int board[BOARD_SIZE][BOARD_SIZE];
static int currentPlayer;
static int winner;
static Move undoStack[BOARD_SIZE * BOARD_SIZE];
static int undoTop;

int gameOver = 0;

void initGame(void) {
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = EMPTY;
        }
    }
    currentPlayer = BLACK;
    undoTop = -1;
    gameOver = 0;
    winner = 0;
}

void restartGame(void) {
    initGame();
}

static int checkWin(int row, int col) {
    int directions[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};
    int d, count, r, c;
    int piece = board[row][col];

    for (d = 0; d < 4; d++) {
        count = 1;

        r = row + directions[d][0];
        c = col + directions[d][1];
        while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && board[r][c] == piece) {
            count++;
            r += directions[d][0];
            c += directions[d][1];
        }

        r = row - directions[d][0];
        c = col - directions[d][1];
        while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && board[r][c] == piece) {
            count++;
            r -= directions[d][0];
            c -= directions[d][1];
        }

        if (count >= 5) {
            return 1;
        }
    }
    return 0;
}

int placePiece(int row, int col) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return 0;
    }
    if (board[row][col] != EMPTY) {
        return 0;
    }

    board[row][col] = currentPlayer;
    undoTop++;
    undoStack[undoTop].row = row;
    undoStack[undoTop].col = col;

    if (checkWin(row, col)) {
        gameOver = 1;
        winner = currentPlayer;
    }

    currentPlayer = (currentPlayer == BLACK) ? WHITE : BLACK;
    return 1;
}

void undoMove(void) {
    Move last;
    if (undoTop < 0) {
        return;
    }
    last = undoStack[undoTop];
    board[last.row][last.col] = EMPTY;
    currentPlayer = (currentPlayer == BLACK) ? WHITE : BLACK;
    gameOver = 0;
    winner = 0;
    undoTop--;
}

int getCell(int row, int col) {
    return board[row][col];
}

int getCurrentPlayer(void) {
    return currentPlayer;
}

int getWinner(void) {
    return winner;
}