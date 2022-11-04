#pragma once
#include <map>
#include <set>
#include <mutex>
#include <functional>

class Async {
public:
    template <class T>
    static inline void lock(const T& target) { lock((void*)(&target)); }

    template <class T>
    static inline void unlock(const T& target) { unlock((void*)(&target)); }

    static void lock(const char name[]);
    static void unlock(const char name[]);

    static void startThread(std::function<void()> func);
    static void killAll();
private:

    struct CrossThreadMutex {
        std::mutex mutex;
        std::condition_variable conditionVariable;
        bool locked = false;
    };

    static std::mutex mapMutex;
    static std::map<void*, CrossThreadMutex> locks;
    static bool kill;

    static std::vector<std::thread> threads;

    static std::vector<std::string> namedLockObjects;

    static void lock(void* target);
    static void unlock(void* target);
};

#define synchronized(m) bool __b = true; for(Async::lock(m); __b; Async::unlock(m), __b = false)