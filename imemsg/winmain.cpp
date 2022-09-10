#include <Windows.h>
#include <atlstr.h>

constexpr wchar_t* kClassName = L"ImeMsg";

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void AddString(HWND, LPCWSTR);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pszCmdLine, _In_ int nCmdShow)
{
    WNDCLASSW WndClass = { 0 };
    WndClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    WndClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
    WndClass.hIcon = LoadIconW(NULL, IDI_APPLICATION);
    WndClass.hInstance = hInstance;
    WndClass.lpfnWndProc = WndProc;
    WndClass.lpszClassName = kClassName;
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
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

void AddString(HWND hList, LPCWSTR pszStr)
{
    SendMessageW(hList, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(pszStr));
    SendMessageW(hList, LB_SETCURSEL, static_cast<WPARAM>(SendMessageW(hList, LB_GETCOUNT, 0, 0)) - 1, 0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    constexpr POINT kGuideOutPoint = { 5, 5 };
    constexpr int kGuideHeight = 30;
    static const CStringW kGuide(L"입력하기 전에 왼쪽 버튼을 클릭하여 포커스를 주고 목록을 지울 때는 오른쪽 버튼 클릭");
    static HWND hList = NULL;
    static HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandleW(nullptr));

    PAINTSTRUCT ps = { 0 };
    CStringW strTmp;

    static int chidx = 0;
    static wchar_t ch[3] = { 0 };

    switch (uMsg) {
    case WM_CREATE:
        hList = CreateWindowExW(
            0,
            L"listbox",
            nullptr,
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | WS_HSCROLL | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT,
            0,
            0,
            0,
            0,
            hWnd,
            NULL,
            hInstance,
            nullptr);
        return 0;
    case WM_SIZE:
        MoveWindow(hList, 0, kGuideHeight, LOWORD(lParam), HIWORD(lParam) - kGuideHeight, TRUE);
        return 0;
    case WM_PAINT:
        BeginPaint(hWnd, &ps);
        TextOutW(ps.hdc, kGuideOutPoint.x, kGuideOutPoint.y, kGuide.GetString(), kGuide.GetLength());
        EndPaint(hWnd, &ps);
        return 0;
    case WM_LBUTTONDOWN:
        SetFocus(hWnd);
        return 0;
    case WM_RBUTTONDOWN:
        SendMessageW(hList, LB_RESETCONTENT, 0, 0);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_IME_SETCONTEXT:
        strTmp.Format(L"WM_IME_SETCONTEXT %s lParam = 0x%X", wParam ? L"활성" : L"비활성", lParam);
        AddString(hList, strTmp);
        break;
    case WM_IME_NOTIFY:
        strTmp = L"WM_IME_NOTIFY 명령 = ";
        switch (wParam) {
        case IMN_CHANGECANDIDATE:
            strTmp.Append(L"IMN_CHANGECANDIDATE");
            break;
        case IMN_CLOSECANDIDATE:
            strTmp.Append(L"IMN_CLOSECANDIDATE");
            break;
        case IMN_GUIDELINE:
            strTmp.Append(L"IMN_GUIDELINE");
            break;
        case IMN_OPENCANDIDATE:
            strTmp.Append(L"IMN_OPENCANDIDATE");
            break;
        case IMN_OPENSTATUSWINDOW:
            strTmp.Append(L"IMN_OPENSTATUSWINDOW");
            break;
        case IMN_SETCANDIDATEPOS:
            strTmp.Append(L"IMN_SETCANDIDATEPOS");
            break;
        case IMN_SETCOMPOSITIONFONT:
            strTmp.Append(L"IMN_SETCOMPOSITIONFONT");
            break;
        case IMN_SETCOMPOSITIONWINDOW:
            strTmp.Append(L"IMN_SETCOMPOSITIONWINDOW");
            break;
        case IMN_SETCONVERSIONMODE:
            strTmp.Append(L"IMN_SETCONVERSIONMODE");
            break;
        case IMN_SETOPENSTATUS:
            strTmp.Append(L"IMN_SETOPENSTATUS");
            break;
        case IMN_SETSENTENCEMODE:
            strTmp.Append(L"IMN_SETSENTENCEMODE");
            break;
        case IMN_SETSTATUSWINDOWPOS:
            strTmp.Append(L"IMN_SETSTATUSWINDOWPOS");
            break;
        default:
            strTmp.AppendFormat(L"Unknown (wParam = 0x%X)", wParam);
            break;
        }
        strTmp.AppendFormat(L", lParam = 0x%X", lParam);
        AddString(hList, strTmp);
        break;
    case WM_IME_STARTCOMPOSITION:
        AddString(hList, L"WM_IME_STARTCOMPOSITION");
        break;
    case WM_IME_ENDCOMPOSITION:
        AddString(hList, L"WM_IME_ENDCOMPOSITION");
        break;
    case WM_IME_COMPOSITIONFULL:
        AddString(hList, L"WM_IME_COMPOSITIONFULL");
        break;
    case WM_IME_CONTROL:
        AddString(hList, L"WM_IME_CONTROL");
        break;
    case WM_IME_KEYDOWN:
        AddString(hList, L"WM_IME_KEYDOWN");
        break;
    case WM_IME_KEYUP:
        AddString(hList, L"WM_IME_KEYUP");
        break;
    case WM_IME_SELECT:
        AddString(hList, L"WM_IME_SELECT");
        break;
    case WM_CHAR:
        if (IS_HIGH_SURROGATE(wParam)) {
            ch[0] = wParam;
            chidx = 1;
        } else {
            ch[chidx] = wParam;
            ch[chidx + 1] = 0;
            chidx = 0;
        }

        strTmp.AppendFormat(L"WM_CHAR 문자 = %s, lParam=0x%X", ch, lParam);
        AddString(hList, strTmp);
        return 0;
    case WM_IME_CHAR:
        if (IS_HIGH_SURROGATE(wParam)) {
            ch[0] = wParam;
            chidx = 1;
        } else {
            ch[chidx] = wParam;
            ch[chidx + 1] = 0;
            chidx = 0;
        }

        strTmp.AppendFormat(L"WM_IME_CHAR 문자 = %s, lParam = 0x%X", ch, lParam);
        AddString(hList, strTmp);
        //return 0;
        break;
    case WM_IME_COMPOSITION:
        ch[0] = wParam;
        ch[1] = 0;
        strTmp.Format(L"WM_IME_COMPOSITION 문자 = %s,lParam = ", ch);

        if (lParam & GCS_COMPATTR) {
            strTmp.Append(L"GCS_COMPATTR | ");
        }
        if (lParam & GCS_COMPCLAUSE) {
            strTmp.Append(L"GCS_COMPCLAUSE | ");
        }
        if (lParam & GCS_COMPREADSTR) {
            strTmp.Append(L"GCS_COMPREADSTR | ");
        }
        if (lParam & GCS_COMPREADATTR) {
            strTmp.Append(L"GCS_COMPREADATTR | ");
        }
        if (lParam & GCS_COMPREADCLAUSE) {
            strTmp.Append(L"GCS_COMPREADCLAUSE | ");
        }
        if (lParam & GCS_COMPSTR) {
            strTmp.Append(L"GCS_COMPSTR | ");
        }
        if (lParam & GCS_CURSORPOS) {
            strTmp.Append(L"GCS_CURSORPOS | ");
        }
        if (lParam & GCS_DELTASTART) {
            strTmp.Append(L"GCS_DELTASTART | ");
        }
        if (lParam & GCS_RESULTCLAUSE) {
            strTmp.Append(L"GCS_RESULTCLAUSE | ");
        }
        if (lParam & GCS_RESULTREADCLAUSE) {
            strTmp.Append(L"GCS_RESULTREADCLAUSE | ");
        }
        if (lParam & GCS_RESULTREADSTR) {
            strTmp.Append(L"GCS_RESULTREADSTR | ");
        }
        if (lParam & GCS_RESULTSTR) {
            strTmp.Append(L"GCS_RESULTSTR | ");
        }
        if (lParam & CS_INSERTCHAR) {
            strTmp.Append(L"CS_INSERTCHAR | ");
        }
        if (lParam & CS_NOMOVECARET) {
            strTmp.Append(L"CS_NOMOVECARET | ");
        }

        if (strTmp.Right(3).Compare(L" | ") == 0) {
            strTmp = strTmp.Left(strTmp.GetLength() - 3);
        } else {
            strTmp.AppendFormat(L"Unknown (0x%X)", lParam);
        }

        AddString(hList, strTmp);
        //return 0;
        break;
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}
