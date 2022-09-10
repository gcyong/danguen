#include <Windows.h>

constexpr wchar_t* kClassName = L"DefIme";
constexpr int kBufferMaxLen = 65536;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pszCmdLine, _In_ int nCmdShow)
{
    WNDCLASSW WndClass = { 0 };
    WndClass.hbrBackground  = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    WndClass.hCursor        = LoadCursorW(NULL, IDC_ARROW);
    WndClass.hIcon          = LoadIconW(NULL, IDI_APPLICATION);
    WndClass.hInstance      = hInstance;
    WndClass.lpfnWndProc    = WndProc;
    WndClass.lpszClassName  = kClassName;
    WndClass.style          = CS_HREDRAW | CS_VREDRAW;
    RegisterClassW(&WndClass);
    
    HWND hWnd = NULL;
    hWnd = CreateWindowExW(
        0,
        kClassName,
        kClassName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        nullptr);

    if (IsWindow(hWnd)) {
        ShowWindow(hWnd, nCmdShow);

        MSG msg = { 0 };
        while (GetMessageW(&msg, 0, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        return static_cast<int>(msg.wParam);
    }

    return -1;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps = { 0 };
    static struct _Buffer {
        wchar_t* ptr = nullptr;
        int len = 0;
    } buffer;

    switch (uMsg) {
    case WM_CREATE:
        buffer.ptr = new wchar_t[kBufferMaxLen] {0};
        return 0;
    case WM_DESTROY:
        delete[] buffer.ptr;
        PostQuitMessage(0);
        return 0;
    case WM_CHAR:
        if (buffer.len + 1 < kBufferMaxLen) {
            buffer.ptr[buffer.len++] = static_cast<wchar_t>(wParam);
            if (!IS_HIGH_SURROGATE(wParam)) {
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        return 0;
    case WM_PAINT:
        BeginPaint(hWnd, &ps);
        if (ps.hdc) {
            TextOutW(ps.hdc, 0, 0, buffer.ptr, buffer.len);
        }
        EndPaint(hWnd, &ps);

        return 0;

    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);

}