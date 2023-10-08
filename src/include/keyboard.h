#pragma once

#include <bitset>
#include <array>
#include <memory>
#include "timer.h"

class Keyboard {
public:
    static void init();
    static void update();

    static bool isKeyPressed(int key);
    static bool isKeyDown(int key);
    static bool isKeyUp(int key);
    static bool isDoublePressed(int key);
    static double getTimePressed(int key);
private:
    static void resetDelayed(int key);

    static std::bitset<348> keyStatesDouble;
    static std::bitset<348> keyStatesDelayed;
    static std::array<std::shared_ptr<Timer>, 348> keyTimers;
    static std::array<double, 348> keyTimes;
};