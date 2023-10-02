#pragma once

#include <functional>

class Timer {

public:
    Timer() = default;

    Timer(double duration, std::function<void()> callback);

    void update();

    bool isDone() const;

    void reset();

private:
    bool done = false;
    double duration = 0, startTime = 0;
    std::function<void()> callback;
};