/*
 * game.c —— 五子棋（15×15）游戏逻辑实现
 *
 * 功能：棋盘状态管理、落子、五连判定、平局判定、撤销、重新开始。
 * 简化说明：未实现黑棋"禁手"规则（双三、四四、长连禁手），
 *           如需竞技规则可在 placePiece 中自行补充。
 */
#include "game.h"

/* ---- 游戏状态定义 ---- */
int board[BOARD_SIZE][BOARD_SIZE];
int currentPlayer = BLACK;
int gameOver = 0;
int winner = 0;

/* ---- 落子历史栈（用于撤销），最多 BOARD_SIZE×BOARD_SIZE 步 ---- */
static int histRow[BOARD_SIZE * BOARD_SIZE];
static int histCol[BOARD_SIZE * BOARD_SIZE];
static int histCount = 0;

/* 将棋盘与全部状态复位为一局新棋 */
static void resetBoard(void)
{
    int r, c;
    for (r = 0; r < BOARD_SIZE; r++)
        for (c = 0; c < BOARD_SIZE; c++)
            board[r][c] = EMPTY;

    currentPlayer = BLACK;
    gameOver = 0;
    winner = 0;
    histCount = 0;
}

void initGame(void)
{
    resetBoard();
}

void restartGame(void)
{
    resetBoard();
}

/* 坐标是否在棋盘内 */
static int inBoard(int row, int col)
{
    return row >= 0 && row < BOARD_SIZE &&
           col >= 0 && col < BOARD_SIZE;
}

/* 从 (row, col) 出发沿 (dr, dc) 方向，统计连续同色棋子的个数（含起点） */
static int countLine(int row, int col, int dr, int dc, int color)
{
    int cnt = 0;
    int r = row, c = col;

    while (inBoard(r, c) && board[r][c] == color) {
        cnt++;
        r += dr;
        c += dc;
    }
    return cnt;
}

/* 以 (row, col) 为中心，检查四个方向（横/竖/两斜）是否连成至少五子 */
static int isFive(int row, int col, int color)
{
    static const int dir[4][2] = {
        { 0,  1},   /* 水平   */
        { 1,  0},   /* 垂直   */
        { 1,  1},   /* 主对角线 */
        { 1, -1}    /* 副对角线 */
    };
    int d;

    for (d = 0; d < 4; d++) {
        int dr = dir[d][0];
        int dc = dir[d][1];
        /* 正向 + 反向 - 1（起点被数了两次）即该方向总连续数 */
        int total = countLine(row, col,  dr,  dc, color)
                  + countLine(row, col, -dr, -dc, color)
                  - 1;
        if (total >= 5)
            return 1;
    }
    return 0;
}

/* 棋盘是否已经下满（用于平局判定） */
static int boardFull(void)
{
    int r, c;
    for (r = 0; r < BOARD_SIZE; r++)
        for (c = 0; c < BOARD_SIZE; c++)
            if (board[r][c] == EMPTY)
                return 0;
    return 1;
}

int placePiece(int row, int col)
{
    int color;

    if (gameOver)                    /* 棋局已结束 */
        return 0;
    if (!inBoard(row, col))          /* 越界 */
        return 0;
    if (board[row][col] != EMPTY)    /* 已有棋子 */
        return 0;

    color = currentPlayer;
    board[row][col] = color;

    /* 记录历史，供撤销使用 */
    histRow[histCount] = row;
    histCol[histCount] = col;
    histCount++;

    if (isFive(row, col, color)) {       /* 连成五子 */
        winner = color;
        gameOver = 1;
    } else if (boardFull()) {            /* 棋盘下满且无五连：平局 */
        winner = 0;
        gameOver = 1;
    } else {                             /* 正常换手 */
        currentPlayer = (color == BLACK) ? WHITE : BLACK;
    }
    return 1;
}

int undoMove(void)
{
    int r, c, color;

    if (histCount == 0)              /* 无子可撤 */
        return 0;

    histCount--;
    r = histRow[histCount];
    c = histCol[histCount];
    color = board[r][c];             /* 被撤下的棋子颜色 */

    board[r][c] = EMPTY;
    currentPlayer = color;           /* 回到该子落下前的行棋方 */
    gameOver = 0;                    /* 撤销必然回到"进行中" */
    winner = 0;
    return 1;
}
