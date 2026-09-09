#include <stdio.h>
#include "game.h"
#include "ui.h"

int main() {
    initGame();

    while (1) {
        drawBoard();
        showTurn();

        printf("Enter move (e.g. A 1), U to undo, R to restart: ");
        char input[32];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        if (input[0] == 'U' || input[0] == 'u') {
            undoMove();
            continue;
        }

        if (input[0] == 'R' || input[0] == 'r') {
            restartGame();
            continue;
        }

        char colChar;
        int rowNum;
        if (sscanf(input, " %c %d", &colChar, &rowNum) == 2) {
            if (colChar >= 'a' && colChar <= 'z') {
                colChar = colChar - 'a' + 'A';
            }

            int col = colChar - 'A';
            int row = rowNum - 1;

            if (col < 0 || col > 14 || row < 0 || row > 14) {
                printf("Invalid position\n");
                continue;
            }

            if (placePiece(row, col) == 0) {
                printf("Invalid position\n");
                continue;
            }
        } else {
            printf("Invalid input format\n");
            continue;
        }

        if (gameOver) {
            drawBoard();
            showWinner(currentPlayer);

            printf("Restart? (Y/N): ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                break;
            }
            char choice = input[0];
            if (choice == 'Y' || choice == 'y') {
                restartGame();
            } else {
                break;
            }
        }
    }

    return 0;
}
