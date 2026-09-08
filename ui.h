#ifndef UI_H
#define UI_H

/*
 * 五子棋界面绘制模块
 * 仅负责显示，不管理棋盘状态。
 * 棋盘状态（board、currentPlayer）由游戏主模块定义并维护。
 */

/* 绘制 15x15 棋盘，含横坐标 A-O 与纵坐标 1-15 */
void drawBoard();

/* 显示当前轮到黑棋/白棋 */
void showTurn();

/* 显示胜利信息，player: 1=黑棋, 2=白棋 */
void showWinner(int player);

/* 显示操作提示 */
void showHelp();

#endif /* UI_H */