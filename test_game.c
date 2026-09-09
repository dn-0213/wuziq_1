#include <stdio.h>
#include "game.h"

static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define ASSERT_TRUE(cond) do { \
    tests_run++; \
    if (cond) { tests_passed++; } \
    else { tests_failed++; printf("  [FAIL] %s:%d ASSERT_TRUE(%s)\n", __FILE__, __LINE__, #cond); } \
} while (0)

#define ASSERT_EQ(actual, expected) do { \
    tests_run++; \
    if ((actual) == (expected)) { tests_passed++; } \
    else { tests_failed++; printf("  [FAIL] %s:%d ASSERT_EQ(%s, %s): actual=%d expected=%d\n", \
        __FILE__, __LINE__, #actual, #expected, (int)(actual), (int)(expected)); } \
} while (0)

static void fillBoardAlternating(int emptyRow, int emptyCol)
{
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (i == emptyRow && j == emptyCol) {
                board[i][j] = 0;
            } else {
                board[i][j] = ((i + j) % 2 == 0) ? 1 : 2;
            }
        }
    }
}

static void test_initGame(void)
{
    int i, j;

    printf("[TEST] test_initGame\n");
    initGame();

    ASSERT_EQ(currentPlayer, 1);
    ASSERT_EQ(gameOver, 0);
    ASSERT_EQ(historyCount, 0);

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            ASSERT_EQ(board[i][j], 0);
        }
    }
}

static void test_placePiece_bounds(void)
{
    printf("[TEST] test_placePiece_bounds\n");
    initGame();

    ASSERT_EQ(placePiece(-1, 0), 0);
    ASSERT_EQ(placePiece(0, -1), 0);
    ASSERT_EQ(placePiece(SIZE, 0), 0);
    ASSERT_EQ(placePiece(0, SIZE), 0);
    ASSERT_EQ(placePiece(SIZE, SIZE), 0);

    ASSERT_EQ(currentPlayer, 1);
    ASSERT_EQ(gameOver, 0);
    ASSERT_EQ(historyCount, 0);
}

static void test_placePiece_occupied(void)
{
    printf("[TEST] test_placePiece_occupied\n");
    initGame();

    ASSERT_EQ(placePiece(7, 7), 1);
    ASSERT_EQ(placePiece(7, 7), 0);
    ASSERT_EQ(board[7][7], 1);
}

static void test_placePiece_gameOver(void)
{
    printf("[TEST] test_placePiece_gameOver\n");
    initGame();

    gameOver = 1;
    ASSERT_EQ(placePiece(0, 0), 0);
    ASSERT_EQ(board[0][0], 0);
}

static void test_placePiece_success(void)
{
    printf("[TEST] test_placePiece_success\n");
    initGame();

    ASSERT_EQ(placePiece(7, 7), 1);
    ASSERT_EQ(board[7][7], 1);
    ASSERT_EQ(historyCount, 1);
    ASSERT_EQ(history[0][0], 7);
    ASSERT_EQ(history[0][1], 7);
    ASSERT_EQ(history[0][2], 1);
    ASSERT_EQ(currentPlayer, 2);
}

static void test_checkWin_horizontal(void)
{
    printf("[TEST] test_checkWin_horizontal\n");
    initGame();

    board[7][5] = 1; board[7][6] = 1; board[7][7] = 1; board[7][8] = 1; board[7][9] = 1;
    ASSERT_EQ(checkWin(7, 7, 1), 1);

    initGame();
    board[7][6] = 1; board[7][7] = 1; board[7][8] = 1; board[7][9] = 1;
    ASSERT_EQ(checkWin(7, 7, 1), 0);
}

static void test_checkWin_vertical(void)
{
    printf("[TEST] test_checkWin_vertical\n");
    initGame();

    board[5][7] = 1; board[6][7] = 1; board[7][7] = 1; board[8][7] = 1; board[9][7] = 1;
    ASSERT_EQ(checkWin(7, 7, 1), 1);
}

static void test_checkWin_diagonalMain(void)
{
    printf("[TEST] test_checkWin_diagonalMain\n");
    initGame();

    board[5][5] = 1; board[6][6] = 1; board[7][7] = 1; board[8][8] = 1; board[9][9] = 1;
    ASSERT_EQ(checkWin(7, 7, 1), 1);
}

static void test_checkWin_diagonalAnti(void)
{
    printf("[TEST] test_checkWin_diagonalAnti\n");
    initGame();

    board[5][9] = 1; board[6][8] = 1; board[7][7] = 1; board[8][6] = 1; board[9][5] = 1;
    ASSERT_EQ(checkWin(7, 7, 1), 1);
}

static void test_checkWin_edge(void)
{
    printf("[TEST] test_checkWin_edge\n");
    initGame();

    board[0][0] = 1; board[0][1] = 1; board[0][2] = 1; board[0][3] = 1; board[0][4] = 1;
    ASSERT_EQ(checkWin(0, 0, 1), 1);

    initGame();
    board[10][14] = 1; board[11][14] = 1; board[12][14] = 1; board[13][14] = 1; board[14][14] = 1;
    ASSERT_EQ(checkWin(14, 14, 1), 1);
}

static void test_undoMove_empty(void)
{
    printf("[TEST] test_undoMove_empty\n");
    initGame();

    undoMove();
    ASSERT_EQ(historyCount, 0);
    ASSERT_EQ(currentPlayer, 1);
    ASSERT_EQ(gameOver, 0);
}

static void test_undoMove_normal(void)
{
    printf("[TEST] test_undoMove_normal\n");
    initGame();

    placePiece(7, 7);
    placePiece(8, 8);
    ASSERT_EQ(historyCount, 2);

    undoMove();
    ASSERT_EQ(historyCount, 1);
    ASSERT_EQ(board[8][8], 0);
    ASSERT_EQ(currentPlayer, 2);

    undoMove();
    ASSERT_EQ(historyCount, 0);
    ASSERT_EQ(board[7][7], 0);
    ASSERT_EQ(currentPlayer, 1);
}

static void test_undoMove_afterWin(void)
{
    printf("[TEST] test_undoMove_afterWin\n");
    initGame();

    placePiece(7, 5);
    placePiece(0, 0);
    placePiece(7, 6);
    placePiece(0, 1);
    placePiece(7, 7);
    placePiece(0, 2);
    placePiece(7, 8);
    placePiece(0, 3);
    placePiece(7, 9);
    ASSERT_EQ(gameOver, 1);

    undoMove();
    ASSERT_EQ(gameOver, 0);
    ASSERT_EQ(board[7][9], 0);
    ASSERT_EQ(currentPlayer, 1);
}

static void test_checkDraw_hasEmpty(void)
{
    printf("[TEST] test_checkDraw_hasEmpty\n");
    initGame();
    ASSERT_EQ(checkDraw(), 0);

    board[0][0] = 1;
    ASSERT_EQ(checkDraw(), 0);
}

static void test_checkDraw_full(void)
{
    printf("[TEST] test_checkDraw_full\n");
    initGame();

    fillBoardAlternating(-1, -1);
    ASSERT_EQ(checkDraw(), 1);
}

static void integration_blackWin(void)
{
    printf("[TEST] integration_blackWin\n");
    initGame();

    placePiece(7, 7);
    placePiece(0, 0);
    placePiece(7, 8);
    placePiece(0, 1);
    placePiece(7, 9);
    placePiece(0, 2);
    placePiece(7, 10);
    placePiece(0, 3);
    placePiece(7, 11);

    ASSERT_EQ(gameOver, 1);
    ASSERT_EQ(currentPlayer, 1);
    ASSERT_EQ(historyCount, 9);
}

static void integration_whiteWin(void)
{
    printf("[TEST] integration_whiteWin\n");
    initGame();

    placePiece(0, 0);
    placePiece(7, 7);
    placePiece(0, 1);
    placePiece(7, 8);
    placePiece(0, 2);
    placePiece(7, 9);
    placePiece(0, 3);
    placePiece(7, 10);
    placePiece(1, 1);
    placePiece(7, 11);

    ASSERT_EQ(gameOver, 1);
    ASSERT_EQ(currentPlayer, 2);
    ASSERT_EQ(historyCount, 10);
}

static void integration_draw(void)
{
    int i, j;
    printf("[TEST] integration_draw\n");
    initGame();

    fillBoardAlternating(0, 0);
    historyCount = 0;
    currentPlayer = 1;
    gameOver = 0;

    ASSERT_EQ(placePiece(0, 0), 1);
    ASSERT_EQ(gameOver, 1);

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            ASSERT_TRUE(board[i][j] != 0);
        }
    }
}

static void e2e_undoAndContinue(void)
{
    printf("[TEST] e2e_undoAndContinue\n");
    initGame();

    placePiece(7, 7);
    placePiece(8, 8);
    placePiece(7, 8);

    undoMove();
    ASSERT_EQ(board[7][8], 0);
    ASSERT_EQ(currentPlayer, 1);

    placePiece(7, 8);
    placePiece(0, 0);
    placePiece(7, 9);
    placePiece(0, 1);
    placePiece(7, 10);
    placePiece(0, 2);
    placePiece(7, 11);

    ASSERT_EQ(gameOver, 1);
    ASSERT_EQ(currentPlayer, 1);
}

static void e2e_boundary(void)
{
    printf("[TEST] e2e_boundary\n");
    initGame();

    board[0][0] = 1; board[0][1] = 1; board[0][2] = 1; board[0][3] = 1; board[0][4] = 1;
    ASSERT_EQ(checkWin(0, 0, 1), 1);
    ASSERT_EQ(checkWin(0, 4, 1), 1);

    initGame();
    board[0][14] = 1; board[1][14] = 1; board[2][14] = 1; board[3][14] = 1; board[4][14] = 1;
    ASSERT_EQ(checkWin(0, 14, 1), 1);

    initGame();
    board[10][0] = 1; board[11][1] = 1; board[12][2] = 1; board[13][3] = 1; board[14][4] = 1;
    ASSERT_EQ(checkWin(14, 4, 1), 1);

    initGame();
    board[10][14] = 1; board[11][13] = 1; board[12][12] = 1; board[13][11] = 1; board[14][10] = 1;
    ASSERT_EQ(checkWin(14, 10, 1), 1);
}

static void e2e_restart(void)
{
    printf("[TEST] e2e_restart\n");
    initGame();

    placePiece(7, 7);
    placePiece(8, 8);
    gameOver = 1;

    restartGame();
    ASSERT_EQ(currentPlayer, 1);
    ASSERT_EQ(gameOver, 0);
    ASSERT_EQ(historyCount, 0);
    ASSERT_EQ(board[7][7], 0);
    ASSERT_EQ(board[8][8], 0);
}

int main(void)
{
    printf("========================================\n");
    printf("五子棋核心逻辑测试\n");
    printf("========================================\n\n");

    printf("--- 单元测试 ---\n");
    test_initGame();
    test_placePiece_bounds();
    test_placePiece_occupied();
    test_placePiece_gameOver();
    test_placePiece_success();
    test_checkWin_horizontal();
    test_checkWin_vertical();
    test_checkWin_diagonalMain();
    test_checkWin_diagonalAnti();
    test_checkWin_edge();
    test_undoMove_empty();
    test_undoMove_normal();
    test_undoMove_afterWin();
    test_checkDraw_hasEmpty();
    test_checkDraw_full();

    printf("\n--- 集成与端到端测试 ---\n");
    integration_blackWin();
    integration_whiteWin();
    integration_draw();
    e2e_undoAndContinue();
    e2e_boundary();
    e2e_restart();

    printf("\n========================================\n");
    printf("测试结果汇总：\n");
    printf("  总数: %d\n", tests_run);
    printf("  通过: %d\n", tests_passed);
    printf("  失败: %d\n", tests_failed);
    printf("========================================\n");

    return tests_failed == 0 ? 0 : 1;
}
