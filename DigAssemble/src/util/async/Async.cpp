#include "Async.h"

std::mutex Async::mapMutex;
std::map<void*, Async::CrossThreadMutex> Async::locks;
bool Async::kill = false;

void Async::killAll() {
    mapMutex.lock();
    kill = true;
    for(std::pair<void* const, CrossThreadMutex>& cur : locks) {
        cur.second.locked = false;
        cur.second.conditionVariable.notify_one();
    }
    mapMutex.unlock();
}

bool Async::shouldRun() {
    return !kill;
}

void Async::lock(void* target) {
    mapMutex.lock();
    if(!locks.count(target)) {
        locks[target];
    }
    mapMutex.unlock();
    CrossThreadMutex& ctm = locks.at(target);
    std::unique_lock ul(ctm.mutex);
    ctm.conditionVariable.wait(ul, [&ctm] {return !ctm.locked; });
    ctm.locked = true;
    ul.unlock();

    if(kill) {
        unlock(target);
    }
}

void Async::unlock(void* target) {
    if(kill) {
        return;
    }

    CrossThreadMutex& ctm = locks.at(target);
    if(!ctm.locked) {
        throw std::invalid_argument("Target is not locked!");
    }
    ctm.locked = false;
    ctm.conditionVariable.notify_one();
}
