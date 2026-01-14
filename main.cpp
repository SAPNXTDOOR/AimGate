#include <windows.h>
#include <shellapi.h>
#include "InputState.h"
#include "resource.h"

// =========================
// SHARED INPUT STATE (DEFINITIONS)
// =========================

// Physical state
bool pW = false;
bool pA = false;
bool pS = false;
bool pD = false;
bool pFire = false;

// Logical (emitted) state
bool lW = false;
bool lA = false;
bool lS = false;
bool lD = false;

// Previous logical state
bool prevW = false;
bool prevA = false;
bool prevS = false;
bool prevD = false;


// =========================
// GLOBAL STATE
// =========================
bool g_enabled = true;

// =========================
// TRAY DEFINITIONS
// =========================
#define WM_TRAYICON (WM_USER + 1)

#define ID_TOGGLE        1001
#define ID_UPDATE        1002
#define ID_HELP          1003
#define ID_ABOUT         1004
#define ID_EXIT          1005

HINSTANCE g_hInst;
NOTIFYICONDATA nid = {};
HMENU hTrayMenu;

// =========================
// UPDATE TRAY ICON + TEXT
// =========================
void UpdateTrayIcon(HWND hwnd) {
    nid.hIcon = LoadIcon(
        g_hInst,
        MAKEINTRESOURCE(g_enabled ? IDI_ICON_ON : IDI_ICON_OFF)
    );

    lstrcpy(
        nid.szTip,
        g_enabled ? TEXT("AimGate (Enabled)") : TEXT("AimGate (Disabled)")
    );

    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

// =========================
// WINDOW PROCEDURE
// =========================
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    switch (msg) {

    case WM_TRAYICON:
        if (lParam == WM_RBUTTONUP) {
            POINT pt;
            GetCursorPos(&pt);
            SetForegroundWindow(hwnd);

            // Update toggle text dynamically
            ModifyMenu(
                hTrayMenu,
                ID_TOGGLE,
                MF_BYCOMMAND | MF_STRING,
                ID_TOGGLE,
                g_enabled ? TEXT("Disable AimGate") : TEXT("Enable AimGate")
            );

            TrackPopupMenu(
                hTrayMenu,
                TPM_RIGHTBUTTON,
                pt.x,
                pt.y,
                0,
                hwnd,
                NULL
            );
        }
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {

        case ID_TOGGLE:
            g_enabled = !g_enabled;
            UpdateTrayIcon(hwnd);
            break;

        case ID_UPDATE:
            ShellExecute(
                NULL,
                TEXT("open"),
                TEXT("https://github.com/SAPNXTDOOR/AimGate"),
                NULL,
                NULL,
                SW_SHOWNORMAL
            );
            break;

        case ID_HELP:
            MessageBox(
                hwnd,
                TEXT("Email: soma27245@gmail.com"),
                TEXT("Get Help"),
                MB_OK | MB_ICONINFORMATION
            );
            break;

        case ID_ABOUT:
            MessageBox(
                hwnd,
                TEXT(
                    "AimGate v1.0\n"
                    "Release Date: 15/1/26\n"
                    "Repository: github.com/SAPNXTDOOR/AimGate\n\n"
                    "This app is made by Saptarshi Mallick"
                ),
                TEXT("About AimGate"),
                MB_OK | MB_ICONINFORMATION
            );
            break;

        case ID_EXIT:
            Shell_NotifyIcon(NIM_DELETE, &nid);
            PostQuitMessage(0);
            break;
        }
        break;

    case WM_DESTROY:
        Shell_NotifyIcon(NIM_DELETE, &nid);
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// =========================
// WINMAIN (ENTRY POINT)
// =========================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    g_hInst = hInstance;

    // Register hidden window
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("AimGateTrayWindow");

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        wc.lpszClassName,
        TEXT("AimGate"),
        WS_OVERLAPPEDWINDOW,
        0, 0, 0, 0,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    // Create tray menu
    hTrayMenu = CreatePopupMenu();

    AppendMenu(hTrayMenu, MF_STRING, ID_TOGGLE, TEXT("Disable AimGate"));
    AppendMenu(hTrayMenu, MF_SEPARATOR, 0, NULL);

    AppendMenu(hTrayMenu, MF_STRING, ID_UPDATE, TEXT("Check for updates"));
    AppendMenu(hTrayMenu, MF_STRING, ID_HELP,   TEXT("Get Help"));
    AppendMenu(hTrayMenu, MF_STRING, ID_ABOUT,  TEXT("About"));
    AppendMenu(hTrayMenu, MF_SEPARATOR, 0, NULL);

    AppendMenu(hTrayMenu, MF_STRING, ID_EXIT, TEXT("Exit"));

    // Setup tray icon
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON_ON));

    lstrcpy(nid.szTip, TEXT("AimGate (Enabled)"));

    Shell_NotifyIcon(NIM_ADD, &nid);

    // Install hooks
    InstallKeyboardHook();
    InstallMouseHook();

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
