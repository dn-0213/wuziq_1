#include "game.h"

/* 全局变量定义 */
int board[SIZE][SIZE];
int currentPlayer;
int gameOver;
int history[MAX_HISTORY][3];
int historyCount;

/* 初始化游戏：棋盘清空，黑棋先行，游戏进行中，历史清零 */
void initGame()
{
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            board[i][j] = 0;
        }
    }
    currentPlayer = 1;   /* 黑棋先走 */
    gameOver = 0;        /* 进行中 */
    historyCount = 0;    /* 无历史 */
}

/* 落子：检查边界、空位、游戏状态，落子后记录历史并判断胜负，最后切换玩家 */
int placePiece(int row, int col)
{
    /* 边界检查 */
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
        return 0;
    }
    /* 游戏已结束 */
    if (gameOver) {
        return 0;
    }
    /* 位置已被占用 */
    if (board[row][col] != 0) {
        return 0;
    }

    /* 历史步数上限防御性校验，防止数组越界 */
    if (historyCount >= MAX_HISTORY) {
        return 0;
    }

    /* 落子 */
    board[row][col] = currentPlayer;

    /* 记录历史 */
    history[historyCount][0] = row;
    history[historyCount][1] = col;
    history[historyCount][2] = currentPlayer;
    historyCount++;

    /* 判断胜利 */
    if (checkWin(row, col, currentPlayer)) {
        gameOver = 1;
        return 1;
    }

    /* 判断平局 */
    if (checkDraw()) {
        gameOver = 1;
        return 1;
    }

    /* 切换玩家 */
    currentPlayer = (currentPlayer == 1) ? 2 : 1;

    return 1;
}

/* 检查胜利：以 (row,col) 为中心，检查水平、垂直、两条对角线四个方向
   任一方向连续同色棋子数达到5即胜利 */
int checkWin(int row, int col, int player)
{
    /* 四个方向的增量：水平、垂直、主对角线(左上-右下)、副对角线(右上-左下) */
    int dir[4][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };
    int d, count, r, c;

    for (d = 0; d < 4; d++) {
        count = 1;  /* 包含当前落子 */

        /* 正方向延伸 */
        r = row + dir[d][0];
        c = col + dir[d][1];
        while (r >= 0 && r < SIZE && c >= 0 && c < SIZE && board[r][c] == player) {
            count++;
            r += dir[d][0];
            c += dir[d][1];
        }

        /* 反方向延伸 */
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

/* 悔棋：从历史弹出最后一步，棋盘置空，恢复玩家，游戏恢复进行中 */
void undoMove()
{
    if (historyCount <= 0) {
        return;
    }

    historyCount--;
    int row = history[historyCount][0];
    int col = history[historyCount][1];
    int player = history[historyCount][2];

    /* 棋盘对应位置置空 */
    board[row][col] = 0;

    /* 恢复当前玩家为刚悔棋的玩家 */
    currentPlayer = player;

    /* 游戏恢复进行中 */
    gameOver = 0;
}

/* 重新开始：等价于初始化 */
void restartGame()
{
    initGame();
}

/* 检查平局：棋盘全满则平局 */
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