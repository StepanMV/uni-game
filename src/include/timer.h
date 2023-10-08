#pragma once

#include <functional>
#include <vector>
#include <memory>

class Timer {

public:
    Timer() = default;

    static std::shared_ptr<Timer> getInstance(double duration, std::function<void()> callback = []() {});

    static void updateAll();

    bool isDone() const;
    double getTimeLeft() const;

    void reset();
    void stop();

private:
    Timer(double duration, std::function<void()> callback = []() {});

    void update();

    bool done = false;
    double duration = 0, startTime = 0, currentTime = 0;
    std::function<void()> callback;
    static std::vector<std::weak_ptr<Timer>> globalTimers;
};