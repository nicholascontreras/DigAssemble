#include "Async.h"

#include <algorithm>

std::mutex Async::mapMutex;
std::map<void*, Async::CrossThreadMutex> Async::locks;
bool Async::kill = false;

std::vector<std::thread> Async::threads;

std::vector<std::string> Async::namedLockObjects;

void Async::lock(const char name[]) {
    mapMutex.lock();
    if(std::find(namedLockObjects.begin(), namedLockObjects.end(), name) == namedLockObjects.end()) {
        namedLockObjects.push_back(name);
    }
    mapMutex.unlock();
    size_t namedLockObjectIndex = std::find(namedLockObjects.begin(), namedLockObjects.end(), name) - namedLockObjects.begin();
    void* namedLockObject = (void*)(&namedLockObjects.at(namedLockObjectIndex));
    lock(namedLockObject);
}

void Async::unlock(const char name[]) {
    size_t namedLockObjectIndex = std::find(namedLockObjects.begin(), namedLockObjects.end(), name) - namedLockObjects.begin();
    void* namedLockObject = (void*)(&namedLockObjects.at(namedLockObjectIndex));
    unlock(namedLockObject);
}

void Async::startThread(std::function<void()> func) {
    threads.push_back(std::thread([func] {
        while(!kill) {
            func();
        }
    }));
}

void Async::killAll() {
    mapMutex.lock();
    kill = true;
    for(std::pair<void* const, CrossThreadMutex>& cur : locks) {
        cur.second.locked = false;
        cur.second.conditionVariable.notify_one();
    }
    mapMutex.unlock();

    for(std::thread& cur : threads) {
        cur.join();
    }
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
}

void Async::unlock(void* target) {
    CrossThreadMutex& ctm = locks.at(target);
    if(!ctm.locked) {
        throw std::invalid_argument("Target is not locked!");
    }
    std::unique_lock ul(ctm.mutex);
    ctm.locked = false;
    ctm.conditionVariable.notify_all();
}
