// ListenForLockAndUnlock.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "ListenForLockAndUnlock.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LISTENFORLOCKANDUNLOCK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LISTENFORLOCKANDUNLOCK));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LISTENFORLOCKANDUNLOCK));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LISTENFORLOCKANDUNLOCK);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   if (!WTSRegisterSessionNotification(hWnd, NOTIFY_FOR_THIS_SESSION))
   {
       MessageBox(NULL, L"Failed to register for notifications", L"Error Registering", MB_OK);
       return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND msgwnd = NULL;
    switch (message)
    {
    case WM_CREATE:
        msgwnd = CreateWindow(L"LISTBOX", NULL, WS_CHILD | WS_VSCROLL | WS_TABSTOP | WS_VISIBLE | LBS_HASSTRINGS| LBS_NOSEL, 20, 20, 300, 400, hWnd, NULL, hInst, NULL);
        ShowWindow(msgwnd, SW_SHOW);

        SendMessage(msgwnd, LB_ADDSTRING, NULL, (LPARAM)L"Starting...");

        break;

    case WM_WTSSESSION_CHANGE:
    {
        switch (wParam)
        {
        case WTS_CONSOLE_CONNECT:
            SendMessage(msgwnd, LB_ADDSTRING, NULL, (LPARAM)L"Console connect");
            break;
        case WTS_CONSOLE_DISCONNECT:
            SendMessage(msgwnd, LB_ADDSTRING, NULL, (LPARAM)L"Console disconnect");
            break;
        case WTS_REMOTE_CONNECT:
            SendMessage(msgwnd, LB_ADDSTRING, NULL, (LPARAM)L"Remote connect");
            break;
        case WTS_REMOTE_DISCONNECT:
            SendMessage(msgwnd, LB_ADDSTRING, NULL, (LPARAM)L"Remote disconnect");
            break;
        case WTS_SESSION_LOGON:
            SendMessage(msgwnd, LB_ADDSTRING, NULL, (LPARAM)L"Session logon");
            break;
        case WTS_SESSION_LOGOFF:
            SendMessage(msgwnd, LB_ADDSTRING, NULL, (LPARAM)L"Session logoff");
            break;
        case WTS_SESSION_LOCK:
            SendMessage(msgwnd, LB_ADDSTRING, NULL, (LPARAM)L"Session lock");
            break;
        case WTS_SESSION_UNLOCK:
            SendMessage(msgwnd, LB_ADDSTRING, NULL, (LPARAM)L"Session Unlock");
            break;
        case WTS_SESSION_REMOTE_CONTROL:
            SendMessage(msgwnd, LB_ADDSTRING, NULL, (LPARAM)L"Session remote control");
            break;
        case WTS_SESSION_CREATE:
            SendMessage(msgwnd, LB_ADDSTRING, NULL, (LPARAM)L"Session create");
            break;
        default:
            SendMessage(msgwnd, LB_ADDSTRING, NULL, (LPARAM)L"UNKNOWN");
            break;
        }
    }
    break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
