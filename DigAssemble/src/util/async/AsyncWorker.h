#pragma once

#include <queue>
#include <functional>
#include <thread>
#include <mutex>

#include "Async.h"


class AsyncWorker {
public:

    static void start();

    template <class T>
    static inline void queue(std::function<T()> func, std::function<void(T)> callback) {
        std::function<void* ()> wrappedFunc = [func = std::move(func)]() { return (void*)(new T(func())); };
        std::function<void(void*)> wrappedCallback = [callback = std::move(callback)](void* result) { callback(*(T*)result); delete ((T*)result); };

        Async::lock(workQueue);
        workQueue.push({wrappedFunc, wrappedCallback});
        Async::unlock(workQueue);
    }

    static void runCallback();

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

    static std::queue<QueuedWork> workQueue;
    static std::queue<QueuedCallback> callbackQueue;
};
