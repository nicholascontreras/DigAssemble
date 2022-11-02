#pragma once

#include <queue>
#include <functional>
#include <thread>
#include <mutex>


class AsyncWorker {
public:
    template <class T>
    static inline void queue(std::function<T()> func, std::function<void(T)> callback) {
        std::function<void* ()> wrappedFunc = [func = std::move(func)]() { return (void*)(new T(func())); };
        std::function<void(void*)> wrappedCallback = [callback = std::move(callback)](void* result) { callback(*(T*)result); delete ((T*)result); };

        workQueueMutex.lock();
        workQueue.push({wrappedFunc, wrappedCallback});
        workQueueMutex.unlock();
    }

    static void runCallback();
    static void shutdown();

    static bool isGLThread();
    static void ensureGLThread();

private:
    struct QueuedWork {
        std::function<void*()> work;
        std::function<void(void*)> callback;
    };

    struct QueuedCallback {
        std::function<void(void*)> callback;
        void* result;
    };

    static bool stop;
    static std::thread workThread;

    static std::mutex workQueueMutex;
    static std::queue<QueuedWork> workQueue;

    static std::mutex callbackQueueMutex;
    static std::queue<QueuedCallback> callbackQueue;

    static void run();
};
