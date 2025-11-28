#include "FPSCounter.h"

#include <glfw/glfw3.h>

#include <thread>

#include "debug.h"

double FPSCounter::MAX_HISTORY_AGE = 1;
double FPSCounter::frameStartTime;
std::list<FPSCounter::TimeRecord> FPSCounter::history;

void FPSCounter::recordFrameStart() {
    frameStartTime = glfwGetTime();
}

double FPSCounter::getAvgBusyTime() {
    if (history.empty()) {
        return 0;
    } else {
        double sumOfBusyTimes = 0;
        for (TimeRecord tr : history) {
            sumOfBusyTimes += tr.busyTime;
        }
        return sumOfBusyTimes / history.size();
    }
};

double FPSCounter::getAvgFrameTime() {
    if(history.size() < 2) {
        return 0;
    } else {
        return (history.back().time - history.front().time) / (history.size() - 1);
    }
};

int FPSCounter::getFPS() {
    if(history.size() < 2) {
        return 0;
    } else {
        return (int) round(1 / getAvgFrameTime());
    }
};

void FPSCounter::delayForFPS(int fps) {
    double delayStartTime = glfwGetTime();
    double targetDelayEndTime = frameStartTime + (1.0 / fps);

    while (true) {
        double curTime = glfwGetTime();
        if (curTime >= targetDelayEndTime) {
;           history.push_back({curTime, delayStartTime - frameStartTime});
            return;
        }
    }
};
