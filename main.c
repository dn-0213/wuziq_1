#include <stdio.h>
#include "game.h"
#include "ui.h"

int main() {
    /* 初始化游戏 */
    initGame();

    /* 主循环 */
    while (1) {
        /* 绘制棋盘并显示当前回合 */
        drawBoard();
        showTurn();

        /* 提示并读取用户输入 */
        printf("请输入落子坐标（如 A 1），U 撤销，R 重新开始：");
        char input[32];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        /* 处理撤销操作 */
        if (input[0] == 'U' || input[0] == 'u') {
            undoMove();
            continue;
        }

        /* 处理重新开始 */
        if (input[0] == 'R' || input[0] == 'r') {
            restartGame();
            continue;
        }

        /* 解析坐标输入，格式如 "A 1" */
        char colChar;
        int rowNum;
        if (sscanf(input, " %c %d", &colChar, &rowNum) == 2) {
            /* 统一转换为大写字母 */
            if (colChar >= 'a' && colChar <= 'z') {
                colChar = colChar - 'a' + 'A';
            }

            /* A-O 对应列 0-14，1-15 对应行 0-14 */
            int col = colChar - 'A';
            int row = rowNum - 1;

            /* 范围校验 */
            if (col < 0 || col > 14 || row < 0 || row > 14) {
                printf("位置无效\n");
                continue;
            }

            /* 落子，返回 0 表示位置无效 */
            if (placePiece(row, col) == 0) {
                printf("位置无效\n");
                continue;
            }
        } else {
            printf("输入格式错误\n");
            continue;
        }

        /* 落子后检查游戏是否结束 */
        if (gameOver) {
            /* 最后绘制一次棋盘并显示胜利者 */
            drawBoard();
            showWinner();

            /* 询问是否重新开始 */
            printf("是否重新开始？(Y/N)：");
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