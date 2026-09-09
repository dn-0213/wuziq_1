/*
 * main_improved.c —— 五子棋（15x15）终端版：改进后的主程序
 *
 * 相比原 main.c 的主要改进：
 *   1. 修复超长输入导致缓冲区残留、被下一次 fgets 误判为指令的问题；
 *   2. 统一裁剪首尾空白：既支持 " U" / "U " 这类写法，坐标也兼容 "A1" / "a 1" / "A,1"；
 *   3. 严格拒绝尾随垃圾字符（如 "A1x"），不再出现 "A1" 被拒、"A 1 xyz" 却被接受
 *      这种严格性前后矛盾的情况；
 *   4. 把三种失败原因分开提示：格式错误 / 坐标越界 / 该位置已有棋子；
 *   5. 新增 Q 指令随时退出；游戏结束后对 Y/N 做输入校验（回车或 EOF 视为不继续）；
 *   6. 提示语前 fflush(stdout)，避免 stdout 被重定向/管道时提示文字不刷新；
 *   7. 棋盘尺寸宏定义，消除魔法数字 14。
 *
 * 依赖约定（与原 main.c 一致，来自 game.h / ui.h）：
 *   void initGame(void);
 *   int  placePiece(int row, int col);   // 0 基坐标，返回 0 表示无法落子
 *   void undoMove(void);
 *   void restartGame(void);
 *   extern int gameOver;                 // 0/1
 *   void drawBoard(void);
 *   void showTurn(void);
 *   void showWinner(void);
 *
 * 若你的 game.h 提供了 isValidMove(row,col) / isGameOver() / BOARD_SIZE
 * 等接口，可把下方 parseMove 后的范围校验和 gameOver 判断替换成更内聚的写法
 * （详见评审报告中的"与 game.h/ui.h 的对齐建议"）。
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "game.h"
#include "ui.h"

#define BOARD_ROWS 15
#define BOARD_COLS 15

/* ---------- 输入工具 ---------- */

/* 丢弃 stdin 中当前行剩余字符（超长输入或非法输入后调用） */
static void discardRestOfLine(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/* 读取一整行，去掉末尾换行；若行长超过 buf 容量则清掉残余字符。
 * 返回 0 表示读到 EOF（如 Ctrl+Z / Ctrl+D）。 */
static int readLine(char *buf, size_t size)
{
    if (fgets(buf, (int)size, stdin) == NULL)
        return 0;

    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n')
        buf[len - 1] = '\0';      /* 正常行：去掉换行 */
    else
        discardRestOfLine();      /* 超长行：清掉残余，防止污染下一次读取 */

    return 1;
}

/* 去除字符串首尾空白，返回字符串起始位置 */
static char *trim(char *s)
{
    while (isspace((unsigned char)*s))
        ++s;

    char *end = s + strlen(s);
    while (end > s && isspace((unsigned char)end[-1]))
        --end;
    *end = '\0';

    return s;
}

/* 解析落子坐标，支持 "A1"、"A 1"、"a1"、"A, 1" 等写法。
 *
 * 返回：
 *    1 = 成功，*row / *col 为 0 基坐标；
 *    0 = 格式错误（首字符不是字母、缺行号、数字后还有多余字符等）；
 *   -1 = 语法正确但越界（列不在 A-O 或行不在 1-15）。
 */
static int parseMove(const char *s, int *row, int *col)
{
    unsigned char first = (unsigned char)s[0];

    if (!isalpha(first))               /* 首字符必须是字母 */
        return 0;

    first = (unsigned char)toupper(first);
    *col = first - 'A';
    if (*col < 0 || *col >= BOARD_COLS)
        return -1;                     /* 列超出 A-O */

    /* 字母后允许若干空白或逗号，再跟行号 */
    const char *p = s + 1;
    while (isspace((unsigned char)*p) || *p == ',')
        ++p;

    char *endp = NULL;
    long n = strtol(p, &endp, 10);
    if (endp == p)                     /* 只有字母、没有数字 */
        return 0;

    while (isspace((unsigned char)*endp))
        ++endp;
    if (*endp != '\0')                 /* 数字后还有内容，如 "A1x" */
        return 0;

    if (n < 1 || n > BOARD_ROWS)
        return -1;                     /* 行超出 1-15 */

    *row = (int)n - 1;
    return 1;
}

int main(void)
{
    char input[64];

    initGame();

    while (1) {
        drawBoard();
        showTurn();

        printf("落子（如 A1 或 A 1）；U 撤销，R 重开，Q 退出：");
        fflush(stdout);

        if (!readLine(input, sizeof(input)))
            break;                     /* EOF，正常结束 */

        char *cmd = trim(input);
        if (*cmd == '\0')              /* 直接回车：重新提示 */
            continue;

        if (*cmd == 'U' || *cmd == 'u') {
            undoMove();
            continue;
        }
        if (*cmd == 'R' || *cmd == 'r') {
            restartGame();
            continue;
        }
        if (*cmd == 'Q' || *cmd == 'q')
            break;                     /* 中途退出 */

        int row = 0, col = 0;
        int st = parseMove(cmd, &row, &col);
        if (st == 0) {
            printf("输入格式错误，请使用类似 A1 或 A 1 的格式\n");
            continue;
        }
        if (st < 0) {
            printf("位置无效：列应为 A-%c，行应为 1-%d\n",
                   'A' + BOARD_COLS - 1, BOARD_ROWS);
            continue;
        }

        /* 语法与范围均已校验通过，placePiece 返回 0 即代表该格已有棋子 */
        if (placePiece(row, col) == 0) {
            printf("该位置已有棋子，请重新落子\n");
            continue;
        }

        /* 落子后游戏结束（有人连成五子，或平局由 game.c 置 gameOver） */
        if (gameOver) {
            drawBoard();
            showWinner();

            /* 校验 Y/N：回车视为 N，EOF 视为直接退出 */
            int quit = 0;
            for (;;) {
                printf("再来一局？(Y/N)：");
                fflush(stdout);

                if (!readLine(input, sizeof(input))) {
                    quit = 1;
                    break;
                }
                cmd = trim(input);
                if (*cmd == 'Y' || *cmd == 'y') {
                    restartGame();
                    break;
                }
                if (*cmd == 'N' || *cmd == 'n' || *cmd == '\0') {
                    quit = 1;
                    break;
                }
                printf("请输入 Y 或 N\n");
            }
            if (quit)
                break;
        }
    }

    return 0;
}
