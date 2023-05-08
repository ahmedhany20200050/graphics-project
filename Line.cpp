#include <Windows.h>
#include <iostream>

// This function swaps two points
void mySwap(int &a, int &b) {
    int tmp = a;
    a = b;
    b = tmp;
}

// This function round double number to integer number
int myRound(double num) {
    return (int) (num + 0.5);
}

/// Direct method to draw line

void directMethod1(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color) {
    int dx = xe - xs;
    int dy = ye - ys;
    double slope = (double) dy / dx;
    if (xs > xe) {
        mySwap(xs, xe);
        mySwap(ys, ye);
    }

    int x = xs;
    double y = ys;

    // Draw first point
    SetPixel(hdc, x, myRound(y), color);

    while (x <= xe) {
        y = ys + slope * (x - xs);
        SetPixel(hdc, x, myRound(y), color);
        x++;
    }
}

// Modified direct method that checks on the slope to determine independent variable

void directMethod2(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color) {
    int dx = xe - xs;
    int dy = ye - ys;

    // if abs(dy) <= abs(dx) that means slope is <= 1 as slope = dy / dx
    if (abs(dy) <= abs(dx)) {
        // x is independent
        // y is dependent

        double slope = (double) dy / dx;
        if (xs > xe) {
            mySwap(xs, xe);
            mySwap(ys, ye);
        }

        int x = xs;
        double y = ys;

        // Draw first point
        SetPixel(hdc, x, myRound(y), color);

        while (x <= xe) {
            y = ys + slope * (x - xs);
            SetPixel(hdc, x, myRound(y), color);
            x++;
        }
    } else if (abs(dy) > abs(dx)) {
        // y is independent
        // x is dependent
        double slope = (double) dx / dy;
        if (ys > ye) {
            mySwap(xs, xe);
            mySwap(ys, ye);
        }

        double x = xs;
        int y = ys;

        // Draw first point
        SetPixel(hdc, myRound(x), y, color);

        while (y <= ye) {
            x = xs + slope * (y - ys);

            SetPixel(hdc, myRound(x), y, color);
            y++;
        }
    }
}


/// Simple DDA
void simpleDDA(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color) {

    int dx = xe - xs;
    int dy = ye - ys;

    // Draw first point
    SetPixel(hdc, xs, ys, color);


    // if slope < 1
    if (abs(dy) <= abs(dx)) {
        // x is independent
        // y is dependent

        if (xs > xe) {
            // if xs > xe then swap the two points
            mySwap(xs, xe);
            mySwap(ys, ye);
        }

        int x = xs;
        double y = ys;

        // calculate slope
        double m = (double) dy / dx;

        //increment x by 1 and increment y by change(m)
        while (x < xe) {
            x++;
            y += m;
            SetPixel(hdc, x, myRound(y), color);
        }
    } else if (abs(dy) > abs(dx)) {
        // if slope > 1
        // y is independent
        // x is dependent

        if (ys > ye) {
            // if ys > ye then swap the two points
            mySwap(xs, xe);
            mySwap(ys, ye);
        }

        int y = ys;
        double x = xs;

        // calculate perpendicular slope (m inverse)
        // mi = 1/m = dx/dy
        double mi = (double) dx / dy;

        // increment y by 1 and increment x by change(mi)

        while (y < ye) {
            y++;
            x += mi;
            SetPixel(hdc, myRound(x), y, color);
        }
    }
}

/// Bresenham cases
//  0 <= slope <= 1
void case1(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color) {
    // Ensure that dx is positive
    if (xs > xe) {
        mySwap(xs, xe);
        mySwap(ys, ye);
    }
    int dx = xe - xs;
    int dy = ye - ys;
    int x = xs;
    int y = ys;
    int d_initial = dx - 2 * dy;
    int d = d_initial;
    int d1 = 2 * (dx - dy);
    int d2 = -2 * dy;
    SetPixel(hdc, x, y, color);
    while (x < xe) {
        if (d < 0) {
            y++;
            d += d1;
        } else
            d += d2;
        x++;
        SetPixel(hdc, x, y, color);
    }
}

// 1 < slope
void case2(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color) {
    // Ensure that dy is positive
    if (ys > ye) {
        mySwap(xs, xe);
        mySwap(ys, ye);
    }
    int dx = xe - xs;
    int dy = ye - ys;
    int x = xs;
    int y = ys;
    int d_initial = 2 * dx - dy;
    int d = d_initial;
    int d1 = 2 * dx;
    int d2 = 2 * (dx - dy);
    SetPixel(hdc, x, y, color);
    while (y < ye) {
        if (d < 0) {
            d += d1;
        } else {
            x++;
            d += d2;
        }
        y++;
        SetPixel(hdc, x, y, color);
    }
}

// -1 <= slope <= 0
void case3(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color) {
    // Ensure that dx is negative
    if (xs < xe) {
        mySwap(xs, xe);
        mySwap(ys, ye);
    }
    int dx = xe - xs;
    int dy = ye - ys;
    int x = xs;
    int y = ys;
    int d_initial = dx + 2 * dy;
    int d = d_initial;
    int d1 = 2 * (dx + dy);
    int d2 = 2 * dy;
    SetPixel(hdc, x, y, color);
    while (x > xe) {
        if (d < 0) {
            y++;
            d += d1;
        } else
            d += d2;
        x--;
        SetPixel(hdc, x, y, color);
    }
}

// slope < -1
void case4(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color) {
    // Ensure that dy is positive
    if (ys > ye) {
        mySwap(xs, xe);
        mySwap(ys, ye);
    }
    int dx = xe - xs;
    int dy = ye - ys;
    int x = xs;
    int y = ys;
    int d_initial = 2 * dx + dy;
    int d = d_initial;
    int d1 = 2 * (dx + dy);
    int d2 = 2 * dx;
    SetPixel(hdc, x, y, color);
    while (y < ye) {
        if (d < 0) {
            x--;
            d += d1;
        } else
            d += d2;
        y++;
        SetPixel(hdc, x, y, color);
    }
}

// Bresenham algorithm (midpoint algorithm) (integer DDA)
void Bresenham(HDC hdc, int xs, int ys, int xe, int ye, COLORREF color) {
    // Compute slope
    double slope = (double) (ye - ys) / (xe - xs);
    // case 1
    if (0 <= slope && slope <= 1) case1(hdc, xs, ys, xe, ye, color);

        // case 2
    else if (slope > 1) case2(hdc, xs, ys, xe, ye, color);

        // case 3
    else if (-1 <= slope && slope < 0) case3(hdc, xs, ys, xe, ye, color);

        // case 4
    else if (slope < -1) case4(hdc, xs, ys, xe, ye, color);
}

// lp is 4 bytes, 2 bytes for x and 2 bytes for y and in new devices is 8 bytes 4 bytes for each x and y
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    HDC hdc;
    COLORREF color = RGB(50, 100, 100);

    static int x1, y1;

    // when you make a left click
    if (msg == WM_LBUTTONDOWN) {
        // to get x1 and y1 of the point where mouse was clicked
        x1 = LOWORD(lp);
        y1 = HIWORD(lp);
    }

        // when you release left click
    else if (msg == WM_LBUTTONUP) {
        // get the object which is acting like an intermediate between your code and the target window you want to draw on
        hdc = GetDC(hWnd);

        // (x2, y2) is the end point of my line
        int x2, y2;

        x2 = LOWORD(lp);
        y2 = HIWORD(lp);

        // draw line with direct method
        Bresenham(hdc, x1, y1, x2, y2, color);

        ReleaseDC(hWnd, hdc);
    } else if (msg == WM_CLOSE) {
        DestroyWindow(hWnd);
    } else if (msg == WM_DESTROY) {
        PostQuitMessage(0);
    } else
        return DefWindowProc(hWnd, msg, wp, lp);
    return 0;
}

int APIENTRY WinMain(HINSTANCE h, HINSTANCE p, LPSTR cmd, int csh) {
    WNDCLASS wc;
    wc.lpszClassName = "MyClass";
    wc.lpszMenuName = nullptr;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hbrBackground = (HBRUSH) GetStockObject(LTGRAY_BRUSH);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = h;
    RegisterClass(&wc);
    HWND hWnd = CreateWindow("MyClass", "Hello", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, nullptr, nullptr, h, nullptr);
    ShowWindow(hWnd, csh);
    UpdateWindow(hWnd);
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
