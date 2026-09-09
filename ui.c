#include <stdio.h>
#include "ui.h"

extern int board[15][15];
extern int currentPlayer;

void drawBoard() {
    printf("  ");
    for (int x = 0; x < 15; x++) {
        printf(" %c", 'A' + x);
    }
    printf("\n");

    for (int y = 0; y < 15; y++) {
        printf("%2d", y + 1);
        for (int x = 0; x < 15; x++) {
            const char *piece;
            if (board[x][y] == 1) {
                piece = "X";
            } else if (board[x][y] == 2) {
                piece = "O";
            } else {
                piece = "+";
            }
            printf(" %s", piece);
        }
        printf("\n");
    }
}

void showTurn() {
    if (currentPlayer == 1) {
        printf("Black's turn\n");
    } else {
        printf("White's turn\n");
    }
}

void showWinner(int player) {
    if (player == 1) {
        printf("Black wins!\n");
    } else {
        printf("White wins!\n");
    }
}

void showHelp() {
    printf("Usage:\n");
    printf("  A 1 to place a piece (column + row)\n");
    printf("  U to undo\n");
    printf("  R to restart\n");
}
