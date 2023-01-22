#pragma once

#include "Functions.h"

int random(int a, int b) {
    return rand() % (b - a + 1) + a;
}

float random(float a, float b) {
    return (rand() % (int)((b - a + 1) * 100.0f)) / 100.0f + a;
}

bool chance(int c) {
    return random(0, 100) < c ? true : false;
}
