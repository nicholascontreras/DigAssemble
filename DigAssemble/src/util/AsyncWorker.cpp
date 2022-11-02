#include "AsyncWorker.h"

#include <stdexcept>
#include <glfw/glfw3.h>

bool AsyncWorker::stop = false;
std::thread AsyncWorker::workThread(run);

std::mutex AsyncWorker::workQueueMutex;
std::queue<AsyncWorker::QueuedWork> AsyncWorker::workQueue;

std::mutex AsyncWorker::callbackQueueMutex;
std::queue<AsyncWorker::QueuedCallback> AsyncWorker::callbackQueue;

void AsyncWorker::runCallback() {
    callbackQueueMutex.lock();
    if(callbackQueue.empty()) {
        callbackQueueMutex.unlock();
    } else {
        QueuedCallback qc = callbackQueue.front();
        callbackQueue.pop();
        callbackQueueMutex.unlock();

        qc.callback(qc.result);
    }
}

void AsyncWorker::shutdown() {
    stop = true;
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
    while(!stop) {
        workQueueMutex.lock();
        if(workQueue.empty()) {
            workQueueMutex.unlock();
        } else {
            QueuedWork qw = workQueue.front();
            workQueue.pop();
            workQueueMutex.unlock();

            void* result = qw.work();

            callbackQueueMutex.lock();
            callbackQueue.push({ qw.callback, result });
            callbackQueueMutex.unlock();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}