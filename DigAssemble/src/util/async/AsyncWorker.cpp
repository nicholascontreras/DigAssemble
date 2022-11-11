#include "AsyncWorker.h"

#include <stdexcept>
#include <glfw/glfw3.h>

std::queue<AsyncWorker::QueuedWork> AsyncWorker::workQueue;
std::queue<AsyncWorker::QueuedCallback> AsyncWorker::callbackQueue;

void AsyncWorker::start() {
    Async::startThread([] {
        while(Async::runThread() || !workQueue.empty()) {

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
        }
    });
}

void AsyncWorker::runOnMain(std::function<void()> func) {
    std::function<void(void*)> wrappedFunc = [func = std::move(func)](void*) { func(); };
    synchronized(callbackQueue) {
        callbackQueue.push({ wrappedFunc, nullptr });
    }
}

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

bool AsyncWorker::isGLThread() {
    return glfwGetCurrentContext() != nullptr;
}

void AsyncWorker::ensureGLThread() {
    if(!isGLThread()) {
        throw std::runtime_error("The current thread is not the GL thread!");
    }
}

void AsyncWorker::finalizeAll() {
    if(Async::runThread()) {
        throw std::runtime_error("Cannot finalize when thread should still be running!");
    }

    while(!callbackQueue.empty()) {
        runCallback();
    }
}
