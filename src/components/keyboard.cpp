#include "keyboard.h"
#include "raylib.h"


std::bitset<348> Keyboard::keyStatesDouble;
std::bitset<348> Keyboard::keyStatesDelayed;
std::array<std::shared_ptr<Timer>, 348> Keyboard::keyTimers;
std::array<double, 348> Keyboard::keyTimes;

void Keyboard::init() {
    for (int i = 0; i < 348; i++) {
        keyStatesDouble[i] = false;
        keyStatesDelayed[i] = false;
        keyTimers[i] = Timer::getInstance(0.2, std::bind(&Keyboard::resetDelayed, i));
        keyTimes[i] = 0;
    }
}

void Keyboard::update()
{
    for (int i = 0; i < 348; i++) {
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

bool Keyboard::isKeyPressed(int key) {
    return IsKeyPressed(key);
}

bool Keyboard::isKeyDown(int key) {
    return IsKeyDown(key);
}

bool Keyboard::isKeyUp(int key) {
    return IsKeyUp(key);
}

bool Keyboard::isDoublePressed(int key) {
    return keyStatesDouble[key];
}

double Keyboard::getTimePressed(int key) {
    return keyTimes[key];
}

void Keyboard::resetDelayed(int key) {
    keyStatesDelayed[key] = false;
}
