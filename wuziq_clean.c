#include <windows.h>
#include <stdio.h>
#include <string.h>

#define SIZE 15
#define CELL_SIZE 40
#define OFFSET 20

int board[SIZE][SIZE];
int currentPlayer = 1;
int gameOver = 0;
int history[225][3];
int historyCount = 0;

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
}

int checkWin(int row, int col, int player) {
    int dirs[4][2] = {{1,0},{0,1},{1,1},{1,-1}};
    int d, step, count, dx, dy, r, c;
    for (d = 0; d < 4; d++) {
        count = 1;
        dx = dirs[d][0];
        dy = dirs[d][1];
        for (step = 1; step < 5; step++) {
            r = row + dx * step;
            c = col + dy * step;
            if (r < 0 || r >= SIZE || c < 0 || c >= SIZE || board[r][c] != player) break;
            count++;
        }
        for (step = 1; step < 5; step++) {
            r = row - dx * step;
            c = col - dy * step;
            if (r < 0 || r >= SIZE || c < 0 || c >= SIZE || board[r][c] != player) break;
            count++;
        }
        if (count >= 5) return 1;
    }
    return 0;
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
    currentPlayer = (currentPlayer == 1) ? 2 : 1;
    return 1;
}

void undoMove() {
    int r, c;
    if (historyCount == 0) return;
    historyCount--;
    r = history[historyCount][0];
    c = history[historyCount][1];
    board[r][c] = 0;
    currentPlayer = history[historyCount][2];
    gameOver = 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            int i, j;
            for (i = 0; i <= SIZE; i++) {
                int pos = OFFSET + i * CELL_SIZE;
                MoveToEx(hdc, pos, OFFSET, NULL);
                LineTo(hdc, pos, OFFSET + (SIZE - 1) * CELL_SIZE);
                MoveToEx(hdc, OFFSET, pos, NULL);
                LineTo(hdc, OFFSET + (SIZE - 1) * CELL_SIZE, pos);
            }
            for (i = 0; i < SIZE; i++) {
                for (j = 0; j < SIZE; j++) {
                    if (board[i][j] == 0) continue;
                    int x = OFFSET + i * CELL_SIZE;
                    int y = OFFSET + j * CELL_SIZE;
                    HBRUSH hBrush;
                    if (board[i][j] == 1) {
                        hBrush = CreateSolidBrush(RGB(0, 0, 0));
                    } else {
                        hBrush = CreateSolidBrush(RGB(255, 255, 255));
                    }
                    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
                    Ellipse(hdc, x - 14, y - 14, x + 14, y + 14);
                    SelectObject(hdc, oldBrush);
                    DeleteObject(hBrush);
                }
            }
            char buf[100];
            if (gameOver) {
                sprintf(buf, "Game Over! %s wins", currentPlayer == 1 ? "Black" : "White");
            } else {
                sprintf(buf, "Current: %s", currentPlayer == 1 ? "Black" : "White");
            }
            SetBkMode(hdc, TRANSPARENT);
            TextOut(hdc, 10, OFFSET + (SIZE - 1) * CELL_SIZE + 10, buf, strlen(buf));
            TextOut(hdc, 250, OFFSET + (SIZE - 1) * CELL_SIZE + 10, "R:Restart U:Undo", 18);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_LBUTTONDOWN: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            int col = (x - OFFSET + CELL_SIZE / 2) / CELL_SIZE;
            int row = (y - OFFSET + CELL_SIZE / 2) / CELL_SIZE;
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
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    initGame();
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "WuZiQi";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);
    HWND hwnd = CreateWindow("WuZiQi", "WuZiQi", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        100, 100, 660, 720, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}