#pragma once

#include <list>

class FPSCounter {
public:
    static void recordFrame();
    static double getFrameTime();
    static int getFPS();
    static void delayForFPS(int fps);
private:
    static double MAX_HISTORY_AGE;
    static std::list<double> timestamps;
};