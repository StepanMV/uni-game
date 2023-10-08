#include "timer.h"
#include "raylib.h"

Timer::Timer(double duration, std::function<void()> callback) {
    this->duration = duration;
    this->callback = callback;
    reset();
}

void Timer::update() {
    if (done) return;

    if (GetTime() - startTime > duration) {
        callback();
        done = true;
    }
}

bool Timer::isDone() const {
    return done;
}

void Timer::reset() {
    done = false;
    startTime = GetTime();
}
