#pragma once
#include <map>
#include <mutex>

class Async {
public:
    template <class T>
    static inline void lock(const T& target) { lock((void*)(&target)); }

    template <class T>
    static inline void unlock(const T& target) { unlock((void*)(&target)); }

    static void killAll();
    static bool shouldRun();
private:

    struct CrossThreadMutex {
        std::mutex mutex;
        std::condition_variable conditionVariable;
        bool locked = false;
    };

    static std::mutex mapMutex;
    static std::map<void*, CrossThreadMutex> locks;
    static bool kill;

    static void lock(void* target);
    static void unlock(void* target);
};

#define synchronized(m) bool __b = true; for(Async::lock(m); __b; Async::unlock(m), __b = false)