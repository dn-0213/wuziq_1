/*
 * ui.c —— 五子棋（15×15）终端界面实现
 *
 * 职责：只负责“展示”，读取 game 模块暴露的状态并绘制，
 *       不修改任何游戏状态，也不处理用户输入。
 *
 * 画面：带坐标（A–O / 1–15）与网格线的文本棋盘，
 *       黑方用 X、白方用 O 表示，空位用 · 表示，星位用 * 表示。
 */
#include <stdio.h>
#include "game.h"
#include "ui.h"

/* Windows 下把控制台输出代码页切到 UTF-8，避免中文/制表符乱码 */
#ifdef _WIN32
#include <windows.h>
static int g_consoleReady = 0;
static void ensureUtf8Console(void)
{
    if (!g_consoleReady) {
        SetConsoleOutputCP(CP_UTF8);
        g_consoleReady = 1;
    }
}
#else
static void ensureUtf8Console(void) { /* 非 Windows 无需处理 */ }
#endif

/* 棋盘第 c 列交点在整行文本中的横向偏移（每格交点槽宽 4 字符） */
static int colOffset(int c)
{
    return c * 4;
}

/* 连续输出 n 个空格 */
static void printSpaces(int n)
{
    int i;
    for (i = 0; i < n; i++)
        putchar(' ');
}

/* 该位置是否 15×15 棋盘的传统星位 */
static int isStar(int row, int col)
{
    /* 天元 (7,7) 与四角星位 (3,3)(3,11)(11,3)(11,11) */
    static const int star[5][2] = {
        {3, 3}, {3, 11}, {7, 7}, {11, 3}, {11, 11}
    };
    int i;
    for (i = 0; i < 5; i++) {
        if (star[i][0] == row && star[i][1] == col)
            return 1;
    }
    return 0;
}

/* 取 (row, col) 处应显示的字符 */
static int cellChar(int row, int col)
{
    if (board[row][col] == BLACK)
        return 'X';                                  /* 黑方 */
    if (board[row][col] == WHITE)
        return 'O';                                  /* 白方 */
    return isStar(row, col) ? '*' : '·';             /* 空位 */
}

void drawBoard(void)
{
    int r, c;

    ensureUtf8Console();

    /* 顶部标题与图例 */
    printf("\n============== 五 子 棋 Gomoku 15×15 ==============\n");
    printf("  黑方 X（先手）    白方 O    空位 ·    星位 *\n\n");

    /* 列标行 A–O，字母与下方各列交点对齐。
     * 主行格式：行号区(3 字符) + 15 个交点，交点位于 3+4c 列。 */
    printf("   ");
    {
        int cursor = 3;              /* 当前已写到的绝对列位置 */
        for (c = 0; c < BOARD_SIZE; c++) {
            int target = 3 + colOffset(c);
            while (cursor < target) {
                putchar(' ');
                cursor++;
            }
            putchar('A' + c);
            cursor++;
        }
    }
    printf("\n");

    /* 逐行绘制：主行（交点 + 横向连接线），行间补竖线 */
    for (r = 0; r < BOARD_SIZE; r++) {
        printf("%2d ", r + 1);
        for (c = 0; c < BOARD_SIZE; c++) {
            if (c > 0)
                printf("───");
            putchar(cellChar(r, c));
        }
        printf("\n");

        if (r < BOARD_SIZE - 1) {
            printf("   ");
            for (c = 0; c < BOARD_SIZE; c++) {
                if (c > 0)
                    printf("   ");
                putchar('│');
            }
            printf("\n");
        }
    }
    printf("\n");
}

void showTurn(void)
{
    if (gameOver)
        return;

    if (currentPlayer == BLACK)
        printf(">>> 轮到 黑方（X）落子\n");
    else
        printf(">>> 轮到 白方（O）落子\n");
}

void showWinner(void)
{
    if (winner == BLACK)
        printf("🎉 黑方（X）获胜！\n");
    else if (winner == WHITE)
        printf("🎉 白方（O）获胜！\n");
    else
        printf("🤝 平局：棋盘已下满，双方未分胜负。\n");
}
