#include "ResourceLock.h"

ResourceLock::ResourceLock() : b(false) {
}

void ResourceLock::lock() {
    std::unique_lock lock(m);
    cv.wait(lock, [&] { return !b; });
    b = true;
}

void ResourceLock::unlock() {
    b = false;
    cv.notify_one();
}
