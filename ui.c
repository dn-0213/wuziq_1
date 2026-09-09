#include <stdio.h>
#include "ui.h"
#include "game.h"

void drawBoard(void) {
    int i, j, cell;
    printf("   ");
    for (j = 0; j < BOARD_SIZE; j++) {
        printf("%c ", 'A' + j);
    }
    printf("\n");

    for (i = 0; i < BOARD_SIZE; i++) {
        printf("%2d ", i + 1);
        for (j = 0; j < BOARD_SIZE; j++) {
            cell = getCell(i, j);
            if (cell == EMPTY) {
                printf("· ");
            } else if (cell == BLACK) {
                printf("● ");
            } else {
                printf("○ ");
            }
        }
        printf("\n");
    }
}

void showTurn(void) {
    int player = getCurrentPlayer();
    if (player == BLACK) {
        printf("黑方 落子\n");
    } else {
        printf("白方 落子\n");
    }
}

void showWinner(void) {
    int w = getWinner();
    if (w == BLACK) {
        printf("黑方 获胜\n");
    } else {
        printf("白方 获胜\n");
    }
}