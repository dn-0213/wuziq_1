/*
 * ui.h —— 五子棋（15×15）终端界面模块接口
 *
 * 职责划分：ui 模块只负责"展示"（读 game 模块暴露的状态并绘制），
 *           不修改任何游戏状态，不做输入处理。
 */
#ifndef UI_H
#define UI_H

/* 绘制棋盘（含列标 A-O、行号 1-15、空位与星位） */
void drawBoard(void);

/* 显示当前行棋方 */
void showTurn(void);

/* 显示对局结果（胜负或平局）；仅应在 gameOver 为真时调用 */
void showWinner(void);

#endif /* UI_H */
