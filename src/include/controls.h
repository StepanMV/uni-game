#pragma once

#include <bitset>
#include <array>
#include <memory>
#include "timer.h"

class Controls {
public:
    static void init();
    static void update();

    static bool isKeyPressed(int key);
    static bool isKeyDown(int key);
    static bool isKeyUp(int key);
    static bool isKeyDoublePressed(int key);
    static double getKeyTimePressed(int key);

    static bool isMousePressed(int key);
    static bool isMouseDown(int key);
    static bool isMouseUp(int key);
    static bool isMouseDoublePressed(int key);
    static double getMouseTimePressed(int key);
private:
    static void resetDelayed(int key);
    static std::bitset<348 + 7> keyStatesDouble;
    static std::bitset<348 + 7> keyStatesDelayed;
    static std::array<std::shared_ptr<Timer>, 348 + 7> keyTimers;
    static std::array<double, 348 + 7> keyTimes;
};