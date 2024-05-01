#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

void swap(int& x1, int& y1, int& x2, int& y2) {
    int tmp = x1;
    x1 = x2;
    x2 = tmp;
    tmp = y1;
    y1 = y2;
    y2 = tmp;
}

void drawLine(int x1, int y1, int x2, int y2, HDC hdc, COLORREF color) {
    int dx = x2 - x1, dy = y2 - y1;
    double m = (double)dy / dx;

    if (abs(dx) >= abs(dy)) {   // Case Slope is lees than 1.
        if (x2 < x1) swap(x1, y1, x2, y2);
        int x = x1;
        double y = y1;
        SetPixel(hdc, x, (int) y, color);
        for (; x <= x2; x++) {
            y += m;
            SetPixel(hdc, x, (int) round(y), color);
        }
    }
    else {  // Case Slope is more than 1.
        if (y1 > y2) swap(x1, y1, x2, y2);
        double mi = 1 / m;
        int y = y1;
        double x = x1;
        SetPixel(hdc, (int) x, y, color);
        for (; y <= y2; y++) {
            x += mi;
            SetPixel(hdc, (int) round (x), y, color);
        }
    }
}

// x1 y1 for top left corner and x2 y2 for bottom right corner.
void drawRectangle(int x1, int y1, int x2, int y2, HDC hdc, COLORREF color){
    drawLine(x1, y1, x2, y1, hdc, color);   // Upper Line
    drawLine(x2, y1, x2, y2, hdc, color);   // Right Line
    drawLine(x2, y2, x1, y2, hdc, color);   // Lower Line
    drawLine(x1, y2, x1, y1, hdc, color);   // Left Line
}

//----------------------------------------------------------------
POINT topLeft = { -1, -1 }; // Initializing to (-1, -1) indicates no click yet.

LRESULT WINAPI WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp)
{
    HDC hdc;
    int x, y;
    switch (m)
    {
        case WM_LBUTTONDOWN:
            if (topLeft.x == -1 && topLeft.y == -1) {
                // First click, store the coordinates as top-left corner.
                topLeft.x = LOWORD(lp);
                topLeft.y = HIWORD(lp);
            } else {
                // Second click, calculate bottom-right corner and draw rectangle.
                hdc = GetDC(hwnd);
                x = LOWORD(lp);
                y = HIWORD(lp);
                drawRectangle(topLeft.x, topLeft.y, x, y, hdc, RGB(255, 0, 0)); // Drawing in red color for example
                ReleaseDC(hwnd, hdc);
                topLeft.x = -1; // Resetting for next rectangle drawing
                topLeft.y = -1;
            }
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default: return DefWindowProc(hwnd, m, wp, lp);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE hi, HINSTANCE pi, LPSTR c, int ns)
{
    WNDCLASS wc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
    wc.hInstance = hi;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = "MyClass";
    wc.lpszMenuName = nullptr;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);
    HWND hwnd = CreateWindow("MyClass", "Hello World", WS_OVERLAPPEDWINDOW, 0, 0, 600, 400, nullptr, nullptr, hi, nullptr);
    ShowWindow(hwnd, ns);
    UpdateWindow(hwnd);
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}