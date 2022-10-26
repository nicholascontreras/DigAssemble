#include "FPSCounter.h"

#include <glfw/glfw3.h>

#include <thread>

double FPSCounter::MAX_HISTORY_AGE = 1;
std::list<double> FPSCounter::timestamps;

void FPSCounter::recordFrame() {
    double time = glfwGetTime();
    timestamps.push_back(time);

    while(timestamps.front() < time - MAX_HISTORY_AGE) {
        timestamps.pop_front();
    }
};

double FPSCounter::getAvgFrameTime() {
    if(timestamps.size() < 2) {
        return 0;
    } else {
        return (timestamps.back() - timestamps.front()) / timestamps.size();
    }
};

double FPSCounter::getPrevFrameTime() {
    if(timestamps.size() < 2) {
        return 0;
    } else {
        return timestamps.back() - *std::prev(std::prev(timestamps.end()));
    }
};

int FPSCounter::getFPS() {
    if(timestamps.size() < 2) {
        return 0;
    } else {
        return (int)(1 / getAvgFrameTime());
    }
};

void FPSCounter::delayForFPS(int fps) {
    double targetFrameTime = 1.0 / fps;
    double actualFrameTime = getPrevFrameTime();

    if (actualFrameTime < targetFrameTime) {
        double delay = targetFrameTime - actualFrameTime;
        int nanoseconds = (int)(delay * 1000000000);
        std::this_thread::sleep_for(std::chrono::nanoseconds(nanoseconds));
    }
};
