#include "controls.h"
#include "raylib.h"


std::bitset<348 + 7> Controls::keyStatesDouble;
std::bitset<348 + 7> Controls::keyStatesDelayed;
std::array<std::shared_ptr<Timer>, 348 + 7> Controls::keyTimers;
std::array<double, 348 + 7> Controls::keyTimes;

void Controls::init() {
    for (int i = 0; i < 348 + 7; i++) {
        keyStatesDouble[i] = false;
        keyStatesDelayed[i] = false;
        keyTimers[i] = Timer::getInstance(0.2, std::bind(&Controls::resetDelayed, i));
        keyTimes[i] = 0;
    }
}

void Controls::update()
{
    for (int i = 0; i < 348 + 7; i++) {
        if (keyStatesDouble[i]) {
            keyStatesDouble[i] = false;
            keyStatesDelayed[i] = false;
        }
        if (IsKeyPressed(i)) {
            if (keyStatesDelayed[i]) keyStatesDouble[i] = true;
            keyStatesDelayed[i] = true;
            keyTimers[i].reset();
        }
        if (IsKeyReleased(i)) {
            keyTimes[i] = 0;
        }
        if (IsKeyDown(i)) {
            keyTimes[i] += GetFrameTime();
        }
    }
}

bool Controls::isKeyPressed(int key) {
    return IsKeyPressed(key);
}

bool Controls::isKeyDown(int key) {
    return IsKeyDown(key);
}

bool Controls::isKeyUp(int key) {
    return IsKeyUp(key);
}

bool Controls::isKeyDoublePressed(int key) {
    return keyStatesDouble[key];
}

double Controls::getKeyTimePressed(int key) {
    return keyTimes[key];
}

bool Controls::isMousePressed(int key) {
    return IsMouseButtonPressed(key);
}

bool Controls::isMouseDown(int key) {
    return IsMouseButtonDown(key);
}

bool Controls::isMouseUp(int key) {
    return IsMouseButtonUp(key);
}

bool Controls::isMouseDoublePressed(int key) {
    return keyStatesDouble[key + 348];
}

double Controls::getMouseTimePressed(int key) {
    return keyTimes[key + 348];
}

void Controls::resetDelayed(int key) {
    keyStatesDelayed[key] = false;
}
