/*
 * game.h —— 五子棋（15×15）游戏逻辑模块接口
 *
 * 职责划分：
 *   game 模块只负责规则与状态，不做任何输入输出；
 *   棋盘、当前玩家、结束状态以 extern 变量暴露，供 ui 模块读取。
 */
#ifndef GAME_H
#define GAME_H

#define BOARD_SIZE 15          /* 棋盘尺寸：15×15 */

/* 棋子取值 */
#define EMPTY 0                /* 空位 */
#define BLACK 1                /* 黑方，符号 X，先手 */
#define WHITE 2                /* 白方，符号 O */

/* ---- 游戏状态（由 game.c 定义，供 ui.c / main.c 读取） ---- */
extern int board[BOARD_SIZE][BOARD_SIZE]; /* 棋盘：EMPTY / BLACK / WHITE */
extern int currentPlayer;                  /* 当前轮到谁：BLACK 或 WHITE */
extern int gameOver;                       /* 0 进行中，1 已结束（胜或平） */
extern int winner;                         /* 结束时：BLACK/WHITE，0 为平局 */

/* ---- 游戏逻辑接口 ---- */

/* 初始化一局新棋（等价于 restartGame） */
void initGame(void);

/*
 * 在 (row, col) 落子（0 基坐标）。
 * 成功返回 1；失败返回 0（越界、该格已有棋子、或棋局已结束）。
 * 落子后自动判定胜负/平局并切换行棋方。
 */
int placePiece(int row, int col);

/* 撤销上一步。成功返回 1；无子可撤返回 0。 */
int undoMove(void);

/* 重新开始一局 */
void restartGame(void);

#endif /* GAME_H */
