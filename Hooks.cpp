#include <windows.h>
#include "InputState.h"

// Hooks
HHOOK keyboardHook = NULL;
HHOOK mouseHook = NULL;

extern bool g_enabled;


/* =========================
   KEYBOARD HOOK
   ========================= */
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* k = (KBDLLHOOKSTRUCT*)lParam;

        // 🔥 CRITICAL: ignore injected input from SendInput
        if (k->flags & LLKHF_INJECTED) {
            return CallNextHookEx(NULL, nCode, wParam, lParam);
        }

        bool isKeyDown = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN);
        bool isKeyUp   = (wParam == WM_KEYUP   || wParam == WM_SYSKEYUP);

        if (isKeyDown || isKeyUp) {

            switch (k->vkCode) {

            case 'W':
                if (isKeyDown && !pW) pW = true;
                else if (isKeyUp)     pW = false;
                break;

            case 'A':
                if (isKeyDown && !pA) pA = true;
                else if (isKeyUp)     pA = false;
                break;

            case 'S':
                if (isKeyDown && !pS) pS = true;
                else if (isKeyUp)     pS = false;
                break;

            case 'D':
                if (isKeyDown && !pD) pD = true;
                else if (isKeyUp)     pD = false;
                break;

            default:
                break;
            }

            // Update logical state (Mouse Override only)
            Update();

            // Block original WASD input
            if (g_enabled &&
                (k->vkCode == 'W' || k->vkCode == 'A' ||
                k->vkCode == 'S' || k->vkCode == 'D')) {
                return 1;
            }
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

/* =========================
   MOUSE HOOK
   ========================= */
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {

        if (wParam == WM_LBUTTONDOWN) {
            pFire = true;
            Update();
        }
        else if (wParam == WM_LBUTTONUP) {
            pFire = false;
            Update();
        }
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

/* =========================
   INSTALL HOOKS
   ========================= */
void InstallKeyboardHook() {
    keyboardHook = SetWindowsHookEx(
        WH_KEYBOARD_LL,
        KeyboardProc,
        NULL,
        0
    );
}

void InstallMouseHook() {
    mouseHook = SetWindowsHookEx(
        WH_MOUSE_LL,
        MouseProc,
        NULL,
        0
    );
}
