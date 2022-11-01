#pragma once

#include <thread>
#include <queue>
#include <functional>
#include <mutex>

class GLThread {
public:
    static void call(const std::function<void()>& func);
    static void join();

private:

    struct QueuedCall {
        std::function<void()> func;
        bool done = false;
    };

    static std::thread glThread;
    static std::mutex queueMutex;
    static bool stop;
    static std::queue<QueuedCall*> callQueue;

    static void run();
};
