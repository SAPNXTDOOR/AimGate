#include "InputState.h"

// Emit only on change
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
    if (pFire) {
        // 🔥 Fire held → disable movement
        lW = lA = lS = lD = false;
    } else {
        // 🔄 Fire released → restore physical movement
        lW = pW;
        lA = pA;
        lS = pS;
        lD = pD;
    }

    EmitIfChanged();
}
