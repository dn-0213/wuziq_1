#include <stdio.h>
#include "ui.h"

/*
 * 外部依赖：由游戏主模块（如 game.c）定义并维护。
 *   board[x][y]：棋盘状态，x 为列(0-14 对应 A-O)，y 为行(0-14 对应 1-15)
 *                 0 = 空位，1 = 黑棋，2 = 白棋
 *   currentPlayer：当前玩家，1 = 黑棋，2 = 白棋
 */
extern int board[15][15];
extern int currentPlayer;

/* 绘制 15x15 棋盘 */
void drawBoard() {
    /* 顶部横坐标 A-O，前导两空格与行号(%2d)对齐 */
    printf("  ");
    for (int x = 0; x < 15; x++) {
        printf(" %c", 'A' + x);
    }
    printf("\n");

    /* 逐行绘制：行号 + 15 个格子 */
    for (int y = 0; y < 15; y++) {
        printf("%2d", y + 1);
        for (int x = 0; x < 15; x++) {
            const char *piece;
            if (board[x][y] == 1) {
                piece = "●";   /* 黑棋 */
            } else if (board[x][y] == 2) {
                piece = "○";   /* 白棋 */
            } else {
                piece = "+";   /* 空位 */
            }
            printf(" %s", piece);
        }
        printf("\n");
    }
}

/* 显示当前轮到谁 */
void showTurn() {
    if (currentPlayer == 1) {
        printf("当前轮到黑棋\n");
    } else {
        printf("当前轮到白棋\n");
    }
}

/* 显示胜利信息 */
void showWinner(int player) {
    if (player == 1) {
        printf("黑棋获胜！\n");
    } else {
        printf("白棋获胜！\n");
    }
}

/* 显示操作提示 */
void showHelp() {
    printf("操作提示：\n");
    printf("  输入格式：A 1 表示落子（列 + 行）\n");
    printf("  U 悔棋\n");
    printf("  R 重新开始\n");
}