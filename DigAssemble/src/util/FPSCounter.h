#pragma once

#include <list>

class FPSCounter {
public:
    static void recordFrame();
    static double getAvgFrameTime();
    static double getPrevFrameTime();
    static int getFPS();
    static void delayForFPS(int fps);
private:
    static double MAX_HISTORY_AGE;
    static std::list<double> timestamps;
};