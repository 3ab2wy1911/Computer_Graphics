#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

void draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF color){
    SetPixel(hdc,xc+x,yc+y,color);
    SetPixel(hdc,xc+x,yc-y,color);
    SetPixel(hdc,xc-x,yc+y,color);
    SetPixel(hdc,xc-x,yc-y,color);
    SetPixel(hdc,xc+y,yc+x,color);
    SetPixel(hdc,xc+y,yc-x,color);
    SetPixel(hdc,xc-y,yc+x,color);
    SetPixel(hdc,xc-y,yc-x,color);
}

void drawCircle(HDC hdc, int xc, int yc, int R, COLORREF color){
    int x =0, y = R;
    draw8Points(hdc, xc, yc, x, y, color);
    while(x<y){
        x++;
        y = (int) round(sqrt(R*R - x*x));
        draw8Points(hdc, xc, yc, x, y, color);
    }
}

LRESULT WINAPI WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp)
{
    HDC hdc;
    static POINT center; // Static variable to store center coordinates
    static POINT onCircle; // Static variable to store a point on the circle
    static bool isFirstClick = true; // Flag to track first click

    switch (m)
    {
        case WM_LBUTTONDOWN:
            if (isFirstClick) {
                // First click, store the coordinates as the center of the circle.
                center.x = LOWORD(lp);
                center.y = HIWORD(lp);
                isFirstClick = false;
            } else {
                // Second click, store the coordinates as a point on the circle.
                onCircle.x = LOWORD(lp);
                onCircle.y = HIWORD(lp);

                hdc = GetDC(hwnd);

                // Calculate the radius using the distance formula
                int radius = static_cast<int>(sqrt(pow(onCircle.x - center.x, 2) + pow(onCircle.y - center.y, 2)));

                // Draw the Face.
                drawCircle(hdc, center.x, center.y, radius, RGB(0, 0, 255));

                // Draw the eyes
                int eyeRadius = radius / 10;
                drawCircle(hdc, center.x - radius / 4, center.y - radius / 4, eyeRadius, RGB(0, 0, 255));
                drawCircle(hdc, center.x + radius / 4, center.y - radius / 4, eyeRadius, RGB(0, 0, 255));

                // Draw the mouth
                int mouthRadius = radius / 5;
                drawCircle(hdc, center.x, center.y + radius / 4, mouthRadius, RGB(0, 0, 255));

                ReleaseDC(hwnd, hdc);

                isFirstClick = true;
            }
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
    HWND hwnd = CreateWindow("MyClass", "Smiley Face", WS_OVERLAPPEDWINDOW, 0, 0, 600, 400, nullptr, nullptr, hi, nullptr);
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
