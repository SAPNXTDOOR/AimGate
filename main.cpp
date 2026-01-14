#include <windows.h>
#include <iostream>
#include "InputState.h"

// =========================
// GLOBAL STATE DEFINITIONS
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

// Previous logical state (for diff-based emission)
bool prevW = false;
bool prevA = false;
bool prevS = false;
bool prevD = false;

int main() {
    std::cout << "Mouse Override standalone running..." << std::endl;

    InstallKeyboardHook();
    InstallMouseHook();

    // Standard Windows message loop (required for hooks)
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
