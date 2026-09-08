#ifndef GAME_H
#define GAME_H

#define SIZE 15              /* 棋盘大小 15x15 */
#define MAX_HISTORY 225      /* 最多步数 = 15*15 */

/* 0=空, 1=黑棋, 2=白棋 */
extern int board[SIZE][SIZE];

/* 1=黑棋先走, 2=白棋 */
extern int currentPlayer;

/* 0=进行中, 1=已结束 */
extern int gameOver;

/* 存储每一步 {row, col, player} */
extern int history[MAX_HISTORY][3];

/* 历史步数 */
extern int historyCount;

/* 初始化游戏 */
void initGame();

/* 落子，返回1成功，0失败 */
int placePiece(int row, int col);

/* 检查胜利，返回1胜利，0未胜利 */
int checkWin(int row, int col, int player);

/* 悔棋 */
void undoMove();

/* 重新开始 */
void restartGame();

/* 检查平局，返回1平局，0未平局 */
int checkDraw();

#endif /* GAME_H */