#include "InputState.h"

extern bool g_enabled;

void EmitIfChanged() {
    if (lW != prevW) SendKey('W', lW);
    if (lA != prevA) SendKey('A', lA);
    if (lS != prevS) SendKey('S', lS);
    if (lD != prevD) SendKey('D', lD);

    prevW = lW;
    prevA = lA;
    prevS = lS;
    prevD = lD;
}

void Update() {

    // 🔒 DISABLED STATE
    if (!g_enabled) {
        lW = lA = lS = lD = false;
        EmitIfChanged();
        return;
    }

    // 🔥 Mouse Override logic
    if (pFire) {
        lW = lA = lS = lD = false;
    } else {
        lW = pW;
        lA = pA;
        lS = pS;
        lD = pD;
    }

    EmitIfChanged();
}
