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
}
double FPSCounter::getFrameTime() {
    if(timestamps.size() < 2) {
        return 0;
    } else {
        return (timestamps.back() - timestamps.front()) / timestamps.size();
    }
};

int FPSCounter::getFPS() {
    if(timestamps.size() < 2) {
        return 0;
    } else {
        return (int)(1 / getFrameTime());
    }
}
void FPSCounter::delayForFPS(int fps) {
    if(getFPS() > fps) {
        double delay = (1.0 / fps) - getFrameTime();
        int nanoseconds = (int)(delay * 1000000000);
        std::this_thread::sleep_for(std::chrono::nanoseconds(nanoseconds));
    }
};
