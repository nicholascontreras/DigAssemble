#include "GLThread.h"

std::thread GLThread::glThread(run);
std::mutex GLThread::queueMutex;
bool GLThread::stop = false;
std::queue<GLThread::QueuedCall*> GLThread::callQueue;

void GLThread::call(const std::function<void()>& func) {
    GLThread::QueuedCall* q = new GLThread::QueuedCall();
    q->func = func;

    queueMutex.lock();
    callQueue.push(q);
    queueMutex.unlock();

    while(!q->done);
    delete q;
}

void GLThread::join() {
    stop = true;
    glThread.join();
}

void GLThread::run() {
    while(!stop) {

        queueMutex.lock();
        if(!callQueue.empty()) {
            QueuedCall* q = callQueue.front();
            callQueue.pop();
            q->func();
            q->done = true;
        }
        queueMutex.unlock();

        std::this_thread::yield();
    }
}
