#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>

#define SIZE 15
#define CELL 38
#define OFF 35

int board[SIZE][SIZE];
int currentPlayer = 1;
int gameOver = 0;
int history[225][3];
int historyCount = 0;
int winLine[5][2];
int winCount = 0;

void initGame() {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            board[i][j] = 0;
        }
    }
    currentPlayer = 1;
    gameOver = 0;
    historyCount = 0;
    winCount = 0;
}

int checkWin(int row, int col, int player) {
    int dirs[4][2] = {{1,0},{0,1},{1,1},{1,-1}};
    int d, step, count, dr, dc, nr, nc;
    for (d = 0; d < 4; d++) {
        count = 1;
        dr = dirs[d][0];
        dc = dirs[d][1];
        for (step = 1; step < 5; step++) {
            nr = row + dr * step;
            nc = col + dc * step;
            if (nr < 0 || nr >= SIZE || nc < 0 || nc >= SIZE) break;
            if (board[nr][nc] != player) break;
            count++;
        }
        for (step = 1; step < 5; step++) {
            nr = row - dr * step;
            nc = col - dc * step;
            if (nr < 0 || nr >= SIZE || nc < 0 || nc >= SIZE) break;
            if (board[nr][nc] != player) break;
            count++;
        }
        if (count >= 5) {
            winCount = (count > 5) ? 5 : count;
            for (int i = 0; i < winCount; i++) {
                winLine[i][0] = row;
                winLine[i][1] = col;
            }
            return 1;
        }
    }
    return 0;
}

int checkDraw() {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) return 0;
        }
    }
    return 1;
}

int placePiece(int row, int col) {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) return 0;
    if (board[row][col] != 0) return 0;
    if (gameOver) return 0;
    board[row][col] = currentPlayer;
    history[historyCount][0] = row;
    history[historyCount][1] = col;
    history[historyCount][2] = currentPlayer;
    historyCount++;
    if (checkWin(row, col, currentPlayer)) {
        gameOver = 1;
        return 1;
    }
    if (checkDraw()) {
        gameOver = 1;
        return 1;
    }
    currentPlayer = (currentPlayer == 1) ? 2 : 1;
    return 1;
}

void undoMove() {
    int row, col;
    if (historyCount == 0) return;
    historyCount--;
    row = history[historyCount][0];
    col = history[historyCount][1];
    board[row][col] = 0;
    currentPlayer = history[historyCount][2];
    gameOver = 0;
    winCount = 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            int i, j;

            // 深色外框背景
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            HBRUSH outerBg = CreateSolidBrush(RGB(60, 40, 20));
            FillRect(hdc, &clientRect, outerBg);
            DeleteObject(outerBg);

            // 棋盘木色背景
            int boardSize = (SIZE - 1) * CELL + 2 * OFF;
            HBRUSH boardBg = CreateSolidBrush(RGB(215, 175, 125));
            RECT boardRect = {OFF - 15, OFF - 15, OFF + boardSize + 15, OFF + boardSize + 15};
            FillRect(hdc, &boardRect, boardBg);
            DeleteObject(boardBg);

            // 棋盘外框
            HPEN borderPen = CreatePen(PS_SOLID, 3, RGB(120, 80, 50));
            SelectObject(hdc, borderPen);
            MoveToEx(hdc, OFF - 5, OFF - 5, NULL);
            LineTo(hdc, OFF + boardSize + 5, OFF - 5);
            LineTo(hdc, OFF + boardSize + 5, OFF + boardSize + 5);
            LineTo(hdc, OFF - 5, OFF + boardSize + 5);
            LineTo(hdc, OFF - 5, OFF - 5);
            DeleteObject(borderPen);

            // 网格线
            HPEN gridPen = CreatePen(PS_SOLID, 1, RGB(80, 50, 30));
            SelectObject(hdc, gridPen);
            for (i = 0; i < SIZE; i++) {
                int pos = OFF + i * CELL;
                MoveToEx(hdc, pos, OFF, NULL);
                LineTo(hdc, pos, OFF + (SIZE - 1) * CELL);
                MoveToEx(hdc, OFF, pos, NULL);
                LineTo(hdc, OFF + (SIZE - 1) * CELL, pos);
            }

            // 坐标标注
            HFONT labelFont = CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
            SelectObject(hdc, labelFont);
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(60, 40, 20));
            char label[3];
            for (i = 0; i < SIZE; i++) {
                label[0] = 'A' + i;
                label[1] = '\0';
                TextOut(hdc, OFF + i * CELL - 4, OFF - 25, label, 1);
                sprintf(label, "%d", i + 1);
                TextOut(hdc, OFF - 25, OFF + i * CELL - 7, label, (int)strlen(label));
            }
            DeleteObject(labelFont);

            // 星位
            int stars[5][2] = {{7,7},{3,3},{11,3},{3,11},{11,11}};
            HBRUSH starBrush = CreateSolidBrush(RGB(60, 40, 20));
            SelectObject(hdc, starBrush);
            for (i = 0; i < 5; i++) {
                int x = OFF + stars[i][0] * CELL;
                int y = OFF + stars[i][1] * CELL;
                Ellipse(hdc, x - 5, y - 5, x + 5, y + 5);
            }
            DeleteObject(starBrush);

            // 棋子
            for (i = 0; i < SIZE; i++) {
                for (j = 0; j < SIZE; j++) {
                    if (board[i][j] == 0) continue;
                    int x = OFF + i * CELL;
                    int y = OFF + j * CELL;

                    // 阴影
                    HBRUSH shadowBrush = CreateSolidBrush(RGB(140, 110, 70));
                    SelectObject(hdc, shadowBrush);
                    Ellipse(hdc, x - 15, y - 15, x + 15, y + 15);
                    DeleteObject(shadowBrush);

                    // 棋子主体
                    HBRUSH pieceBrush;
                    if (board[i][j] == 1) {
                        pieceBrush = CreateSolidBrush(RGB(10, 10, 10));
                    } else {
                        pieceBrush = CreateSolidBrush(RGB(245, 245, 245));
                    }
                    SelectObject(hdc, pieceBrush);
                    Ellipse(hdc, x - 13, y - 13, x + 13, y + 13);
                    DeleteObject(pieceBrush);

                    // 高光
                    HBRUSH highlightBrush;
                    if (board[i][j] == 1) {
                        highlightBrush = CreateSolidBrush(RGB(70, 70, 70));
                    } else {
                        highlightBrush = CreateSolidBrush(RGB(255, 255, 255));
                    }
                    SelectObject(hdc, highlightBrush);
                    Ellipse(hdc, x - 5, y - 7, x - 1, y - 3);
                    DeleteObject(highlightBrush);

                    // 最后落子红圈
                    if (historyCount > 0 && i == history[historyCount - 1][0] && j == history[historyCount - 1][1]) {
                        HPEN redPen = CreatePen(PS_SOLID, 2, RGB(255, 50, 50));
                        SelectObject(hdc, redPen);
                        HBRUSH emptyBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
                        SelectObject(hdc, emptyBrush);
                        Ellipse(hdc, x - 17, y - 17, x + 17, y + 17);
                        DeleteObject(redPen);
                    }
                }
            }
            DeleteObject(gridPen);

            // 胜利连线
            if (winCount > 0) {
                HPEN winPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
                SelectObject(hdc, winPen);
                int x1 = OFF + winLine[0][0] * CELL;
                int y1 = OFF + winLine[0][1] * CELL;
                int x2 = OFF + winLine[winCount-1][0] * CELL;
                int y2 = OFF + winLine[winCount-1][1] * CELL;
                MoveToEx(hdc, x1, y1, NULL);
                LineTo(hdc, x2, y2);
                DeleteObject(winPen);
            }

            // 状态栏
            char statusText[100分];
            if (gameOver) {
                if (winCount > 0) {
                    sprintf(statusText, "Game Over! %s wins!", currentPlayer == 1 ? "Black" : "White");
                } else {
                    sprintf(statusText, "Draw!");
                }
            } else {
                sprintf(statusText, "Current: %s", currentPlayer == 1 ? "Black" : "White");
            }

            HBRUSH statusBg = CreateSolidBrush(RGB(170, 130, 90));
            RECT statusRect = {OFF, OFF + boardSize + 20, OFF + boardSize, OFF + boardSize + 55};
            FillRect(hdc, &statusRect, statusBg);
            DeleteObject(statusBg);

            HPEN statusBorder = CreatePen(PS_SOLID, 1, RGB(100, 70, 40));
            SelectObject(hdc, statusBorder);
            MoveToEx(hdc, OFF, OFF + boardSize + 20, NULL);
            LineTo(hdc, OFF + boardSize, OFF + boardSize + 20);
            LineTo(hdc, OFF + boardSize, OFF + boardSize + 55);
            LineTo(hdc, OFF, OFF + boardSize + 55);
            LineTo(hdc, OFF, OFF + boardSize + 20);
            DeleteObject(statusBorder);

            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(255, 255, 240));
            HFONT statusFont = CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
            SelectObject(hdc, statusFont);
            TextOut(hdc, OFF + 10, OFF + boardSize + 26, statusText, (int)strlen(statusText));
            TextOut(hdc, OFF + 320, OFF + boardSize + 26, "R:Restart U:Undo", 18);
            DeleteObject(statusFont);

            EndPaint(hwnd, &ps);
            break;
        }
        case WM_LBUTTONDOWN: {
            int mouseX = LOWORD(lParam);
            int mouseY = HIWORD(lParam);
            int col = (mouseX - OFF + CELL / 2) / CELL;
            int row = (mouseY - OFF + CELL / 2) / CELL;
            if (col >= 0 && col < SIZE && row >= 0 && row < SIZE) {
                if (placePiece(row, col)) {
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            }
            break;
        }
        case WM_KEYDOWN: {
            if (wParam == 'R' || wParam == 'r') {
                initGame();
                InvalidateRect(hwnd, NULL, TRUE);
            }
            if (wParam == 'U' || wParam == 'u') {
                undoMove();
                InvalidateRect(hwnd, NULL, TRUE);
            }
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        default: {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    initGame();

    WNDCLASS windowClass = {0};
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = hInstance;
    windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    windowClass.lpszClassName = "FiveInARow";
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&windowClass);

    int boardPixels = (SIZE - 1) * CELL + 2 * OFF;
    HWND hwnd = CreateWindow(
        "FiveInARow",
        "Five In A Row - Optimized",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        100, 100, boardPixels + 40, boardPixels + 140,
        NULL, NULL, hInstance, NULL
    );

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
