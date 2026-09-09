#include <windows.h>
#include <stdio.h>

#define SIZE 15
#define CELL_SIZE 40
#define BOARD_SIZE (CELL_SIZE * SIZE)
#define OFFSET 20
#define WINDOW_WIDTH (BOARD_SIZE + 2 * OFFSET)
#define WINDOW_HEIGHT (BOARD_SIZE + 2 * OFFSET + 60)

int board[SIZE][SIZE];
int currentPlayer = 1;
int gameOver = 0;
int history[225][3];
int historyCount = 0;
int winLine[5][2];
int winCount = 0;

void initGame() {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            board[i][j] = 0;
    currentPlayer = 1;
    gameOver = 0;
    historyCount = 0;
    winCount = 0;
}

int checkWin(int row, int col, int player) {
    int dirs[4][2] = {{1,0},{0,1},{1,1},{1,-1}};
    for (int d = 0; d < 4; d++) {
        int count = 1;
        int cells[5][2];
        cells[0][0] = row;
        cells[0][1] = col;
        int dx = dirs[d][0];
        int dy = dirs[d][1];
        for (int step = 1; step < 5; step++) {
            int r = row + dx * step;
            int c = col + dy * step;
            if (r < 0 || r >= SIZE || c < 0 || c >= SIZE || board[r][c] != player) break;
            cells[count][0] = r;
            cells[count][1] = c;
            count++;
        }
        for (int step = 1; step < 5; step++) {
            int r = row - dx * step;
            int c = col - dy * step;
            if (r < 0 || r >= SIZE || c < 0 || c >= SIZE || board[r][c] != player) break;
            cells[count][0] = r;
            cells[count][1] = c;
            count++;
        }
        if (count >= 5) {
            winCount = (count > 5) ? 5 : count;
            for (int i = 0; i < winCount; i++) {
                winLine[i][0] = cells[i][0];
                winLine[i][1] = cells[i][1];
            }
            return 1;
        }
    }
    return 0;
}

int checkDraw() {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (board[i][j] == 0) return 0;
    return 1;
}

int placePiece(int row, int col) {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || board[row][col] != 0 || gameOver) return 0;
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
    if (historyCount == 0 || gameOver == 0) return;
    historyCount--;
    int r = history[historyCount][0];
    int c = history[historyCount][1];
    board[r][c] = 0;
    currentPlayer = history[historyCount][2];
    gameOver = 0;
    winCount = 0;
}

void drawBoard(HDC hdc) {
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    SelectObject(hdc, hPen);

    for (int i = 0; i < SIZE; i++) {
        MoveToEx(hdc, OFFSET + i * CELL_SIZE, OFFSET, NULL);
        LineTo(hdc, OFFSET + i * CELL_SIZE, OFFSET + BOARD_SIZE);
        MoveToEx(hdc, OFFSET, OFFSET + i * CELL_SIZE, NULL);
        LineTo(hdc, OFFSET + BOARD_SIZE, OFFSET + i * CELL_SIZE);
    }

    int stars[5][2] = {{7,7},{3,3},{11,3},{3,11},{11,11}};
    for (int i = 0; i < 5; i++) {
        Ellipse(hdc, OFFSET + stars[i][0] * CELL_SIZE - 4,
                OFFSET + stars[i][1] * CELL_SIZE - 4,
                OFFSET + stars[i][0] * CELL_SIZE + 4,
                OFFSET + stars[i][1] * CELL_SIZE + 4);
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) continue;
            int x = OFFSET + i * CELL_SIZE + CELL_SIZE / 2;
            int y = OFFSET + j * CELL_SIZE + CELL_SIZE / 2;
            HBRUSH hBrush;
            if (board[i][j] == 1)
                hBrush = CreateSolidBrush(RGB(0, 0, 0));
            else
                hBrush = CreateSolidBrush(RGB(255, 255, 255));
            SelectObject(hdc, hBrush);
            Ellipse(hdc, x - 14, y - 14, x + 14, y + 14);
            DeleteObject(hBrush);
        }
    }

    if (winCount > 0) {
        HPEN redPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
        SelectObject(hdc, redPen);
        for (int i = 0; i < winCount - 1; i++) {
            MoveToEx(hdc, OFFSET + winLine[i][0] * CELL_SIZE + CELL_SIZE / 2,
                     OFFSET + winLine[i][1] * CELL_SIZE + CELL_SIZE / 2, NULL);
            LineTo(hdc, OFFSET + winLine[i + 1][0] * CELL_SIZE + CELL_SIZE / 2,
                   OFFSET + winLine[i + 1][1] * CELL_SIZE + CELL_SIZE / 2);
        }
        DeleteObject(redPen);
    }
    DeleteObject(hPen);
}

void drawStatus(HDC hdc) {
    char buf[256];
    if (gameOver) {
        if (winCount > 0)
            sprintf(buf, "Player %s wins!", currentPlayer == 1 ? "Black" : "White");
        else
            sprintf(buf, "Draw!");
    } else {
        sprintf(buf, "Current: %s", currentPlayer == 1 ? "Black" : "White");
    }
    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, 10, OFFSET + BOARD_SIZE + 10, buf, strlen(buf));
    TextOut(hdc, 250, OFFSET + BOARD_SIZE + 10, "R: Restart  U: Undo", 20);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            drawBoard(hdc);
            drawStatus(hdc);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_LBUTTONDOWN: {
            int x = LOWORD(lParam) - OFFSET;
            int y = HIWORD(lParam) - OFFSET;
            if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
                int col = (x + CELL_SIZE / 2) / CELL_SIZE;
                int row = (y + CELL_SIZE / 2) / CELL_SIZE;
                if (col >= 0 && col < SIZE && row >= 0 && row < SIZE) {
                    placePiece(row, col);
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
                             100, 100, WINDOW_WIDTH + 16, WINDOW_HEIGHT + 40,
                             NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}