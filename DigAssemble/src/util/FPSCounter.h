#pragma once

#include <list>

class FPSCounter {
public:
    static void recordFrameStart();
    static double getAvgBusyTime();
    static double getAvgFrameTime();
    static int getFPS();
    static void delayForFPS(int fps);
private:
    struct TimeRecord {
        double time;
        double busyTime;
    };

    static double MAX_HISTORY_AGE;
    static double frameStartTime;
    static std::list<TimeRecord> history;
};