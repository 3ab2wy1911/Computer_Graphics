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

void DrawCircle(HDC hdc, int xc, int yc, int R, COLORREF color){
    int x =0, y = R;
    draw8Points(hdc, xc, yc, x, y, color);
    while(x<y){
        x++;
        y = (int) round(sqrt(R*R - x*x));
        draw8Points(hdc, xc, yc, x, y, color);
    }
}

void polarAlgorithm(HDC hdc, int xc, int yc, int R, COLORREF color){
    int x = R, y = 0;
    draw8Points(hdc, xc, yc, x, y, color);
    double theta = 0, dTheta = 1.0/R;
    while(x>y){
        theta += dTheta;
        x = (int) round (R * cos(theta));
        y = (int) round (R * sin(theta));
        draw8Points(hdc, xc, yc, x, y, color);
    }
}

void iterativePolarAlgorithm(HDC hdc, int xc, int yc, int R, COLORREF color){
    double x = R, y = 0;
    draw8Points(hdc, xc, yc, (int) x, (int) y, color);
    double dTheta = 1.0 / R;
    double c = cos(dTheta);
    double s = sin(dTheta);
    while (x > y){
        double tmp = x * c - y * s;
        y = x * s + y * c;
        x = tmp;
        draw8Points(hdc, xc, yc, (int) x, (int) y, color);
    }
}

void midPointAlgorithm(){

}

LRESULT WINAPI WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp)
{
    HDC hdc;
    int x, y;
    static POINT center; // Static variable to store center coordinates
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
                // Second click, calculate the radius and draw the circle.
                hdc = GetDC(hwnd);
                x = LOWORD(lp);
                y = HIWORD(lp);

                // Calculate the radius using the distance formula
                int radius = static_cast<int>(sqrt(pow(x - center.x, 2) + pow(y - center.y, 2)));

                // Draw the circle using one of the drawing algorithms
                iterativePolarAlgorithm(hdc, center.x, center.y, radius, RGB(0, 0, 255)); // Example color: Blue

                ReleaseDC(hwnd, hdc);

                // Reset for next circle drawing
                isFirstClick = true;
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