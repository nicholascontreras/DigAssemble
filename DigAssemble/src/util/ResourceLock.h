#pragma once
#include <mutex>

class ResourceLock {
public:
    ResourceLock();

    void lock();
    void unlock();
private:
    std::mutex m;
    std::condition_variable cv;
    bool b;
};