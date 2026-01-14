#pragma once
#include <windows.h>

// Physical state
extern bool pW, pA, pS, pD;
extern bool pFire;

// Logical state (what we emit)
extern bool lW, lA, lS, lD;

// Previous logical state
extern bool prevW, prevA, prevS, prevD;

// Core update
void Update();

// Hooks
void InstallKeyboardHook();
void InstallMouseHook();

// Emission
void SendKey(WORD vk, bool down);
