#include <Windows.h>
#include <bits/stdc++.h>
using namespace std;

POINT startPoint = { 0 }; // Global variable to store the starting point of the line

void swap(int& x1, int& x2, int& y1, int& y2) {
    int tmp = x1;
    x1 = x2;
    x2 = tmp;
    tmp = y1;
    y1 = y2;
    y2 = tmp;
}

void simpleDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {
    int dx = x2 - x1, dy = y2 - y1;
    if (dx == 0 && dy == 0) {
        SetPixel(hdc, x1, y1, color);
        return;
    }
    double m = (double)dy / dx;
    if (abs(dy) <= abs(dx)) {
        if (x2 < x1) {
            swap(x1, x2, y1, y2);
        }

        int x = x1;
        double y = y1;
        SetPixel(hdc, x, (int)y, color);

        while (x < x2) {
            x++;
            y += m;
            SetPixel(hdc, x, (int) round (y), color);
        }
    }
    else {
        if (y2 > y1) {
            swap(x1, x2, y1, y2);
        }

        int y = y1;
        double x = x1;
        m = 1 / m;
        SetPixel(hdc, (int)x, y, color);

        while (y < y2) {
            y++;
            x += m;
            SetPixel(hdc, (int) round (x), y, color);
        }
    }
}

void integerDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {
    int dx = x2 - x1, dy = y2 - y1;
    if (dx == 0 && dy == 0) {
        SetPixel(hdc, x1, y1, color);
        return;
    }
    double m = (double)dy / dx;
    if (m < 1) {
        if (x2 < x1) {
            swap(x1, x2, y1, y2);
        }

        int x = x1, y = y1;
        int d = dx - 2 * dy;
        int d1 = 2 * (dx - dy);
        int d2 = -2 * dy;
        SetPixel(hdc, x, y, color);

        while (x < x2) {
            if (d < 0) {
                d += d1;
                y++;
            }
            else {
                d += d2;
            }
            x++;
            SetPixel(hdc, x, y, color);
        }
    }
    else {
        if (y2 > y1) {
            swap(x1, x2, y1, y2);
        }

        int y = y1, x = x1;
        int d = 2 * dx - dy;
        int d1 = 2 * (dx - dy);
        int d2 = 2 * dx;
        SetPixel(hdc, x, y, color);

        while (y < y2) {
            if (d > 0) {
                d += d1;
                x++;
            }
            else {
                d += d2;
            }
            y++;
            SetPixel(hdc, x, y, color);
        }
    }
}

LRESULT WINAPI WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp) {
    HDC hdc;
    int x, y;
    switch (m) {

        case WM_LBUTTONDOWN:
            startPoint.x = LOWORD(lp); // Store the starting point when mouse is pressed
            startPoint.y = HIWORD(lp);
            break;
        case WM_LBUTTONUP:
            hdc = GetDC(hwnd);
            x = LOWORD(lp); // Get the current position of the mouse when released
            y = HIWORD(lp);
            integerDDA(hdc, startPoint.x, startPoint.y, x, y, RGB(0, 0, 0)); // Draw line using DDA algorithm
            ReleaseDC(hwnd, hdc);
            InvalidateRect(hwnd, NULL, TRUE); // Trigger redraw of the window
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, m, wp, lp);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE hi, HINSTANCE pi, LPSTR c, int ns) {
    WNDCLASS wc = { 0 };
    wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
    wc.hInstance = hi;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = "MyClass";
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);

    HWND hwnd = CreateWindow("MyClass", "Line Drawing", WS_OVERLAPPEDWINDOW, 0, 0, 600, 400, nullptr, nullptr, hi, nullptr);
    if (!hwnd) {
        MessageBox(nullptr, "Window creation failed!", "Error", MB_ICONERROR);
        return -1;
    }

    ShowWindow(hwnd, ns);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
