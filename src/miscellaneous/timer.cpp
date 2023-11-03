#include "timer.h"
#include "raylib.h"
#include <memory>

std::vector<std::weak_ptr<Timer>> Timer::globalTimers;

Timer::Timer(double duration, bool autoReset, std::function<void()> callback) {
    this->duration = duration;
    this->autoReset = autoReset;
    this->callback = callback;
    this->startTime = GetTime();
}

void Timer::update() {
    if (done) return;
    currentTime = GetTime();
    if (currentTime - startTime > duration) {
        done = true;
        callback();
        if (autoReset) reset();
    }
}

std::shared_ptr<Timer> Timer::getInstance(double duration, bool autoReset, std::function<void()> callback) {
    auto timer = std::shared_ptr<Timer>(new Timer(duration, autoReset, callback));
    globalTimers.push_back(timer);
    return timer;
}

void Timer::updateAll() {
    auto expireTest = [](std::weak_ptr<Timer> timer) { return timer.expired(); };
    globalTimers.erase(std::remove_if(globalTimers.begin(), globalTimers.end(), expireTest), globalTimers.end());

    for (auto &timer : globalTimers) {
        if (auto timerPtr = timer.lock()) timerPtr->update();
    }
}



bool Timer::isDone() const {
    return done;
}

double Timer::getTimeLeft() const {
    return duration - (currentTime - startTime);
}

void Timer::reset() {
    done = false;
    startTime = GetTime();
}

void Timer::stop() {
    done = true;
}
