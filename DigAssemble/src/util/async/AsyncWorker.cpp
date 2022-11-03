#include "AsyncWorker.h"

#include <stdexcept>
#include <glfw/glfw3.h>

std::thread AsyncWorker::workThread(run);

std::queue<AsyncWorker::QueuedWork> AsyncWorker::workQueue;
std::queue<AsyncWorker::QueuedCallback> AsyncWorker::callbackQueue;

void AsyncWorker::runCallback() {
    Async::lock(callbackQueue);
    if(callbackQueue.empty()) {
        Async::unlock(callbackQueue);
    } else {
        QueuedCallback qc = callbackQueue.front();
        callbackQueue.pop();
        Async::unlock(callbackQueue);

        qc.callback(qc.result);
    }
}

void AsyncWorker::threadJoin() {
    workThread.join();
}

bool AsyncWorker::isGLThread() {
    return glfwGetCurrentContext() != nullptr;
}

void AsyncWorker::ensureGLThread() {
    if(!isGLThread()) {
        throw std::runtime_error("The current thread is not the GL thread!");
    }
}

void AsyncWorker::run() {
    while(Async::shouldRun()) {
        Async::lock(workQueue);
        if(workQueue.empty()) {
            Async::unlock(workQueue);
        } else {
            QueuedWork qw = workQueue.front();
            workQueue.pop();
            Async::unlock(workQueue);

            void* result = qw.work();

            synchronized(callbackQueue) {
                callbackQueue.push({ qw.callback, result });
            }
        }

        std::this_thread::yield();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}