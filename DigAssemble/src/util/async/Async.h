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
    static void stopAllThreads();
    static void joinAllThreads();
    static bool runThread();
private:

    struct CrossThreadMutex {
        std::mutex mutex;
        std::condition_variable conditionVariable;
        bool locked = false;
    };

    static std::mutex mapMutex;
    static std::map<void*, CrossThreadMutex> locks;
    static bool threadsShouldStop;

    static std::vector<std::thread> threads;

    static std::vector<std::string> namedLockObjects;

    static void lock(void* target);
    static void unlock(void* target);
};

#define _SYNC_VAR_CONCAT(a, b) a ## b
#define SYNC_VAR_CONCAT(a, b) _SYNC_VAR_CONCAT(a, b)
#define SYNC_VAR SYNC_VAR_CONCAT(__sync_bool__, __LINE__)

#define synchronized(m) bool SYNC_VAR = true; for(Async::lock(m); SYNC_VAR; Async::unlock(m), SYNC_VAR = false)