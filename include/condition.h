#pragma once

#include "mutex_lock.h"
#include "noncopyable.h"

namespace thread_pool {
class Condition : Noncopyable {
public:
    Condition(MutexLock &mutex) : _mutexLock(mutex) { pthread_cond_init(&_cond, nullptr); }
    ~Condition() { pthread_cond_destroy(&_cond); }

public:

    void inline Wait() { pthread_cond_wait(&_cond, _mutexLock.GetMutex()); }
    void inline Notify() { pthread_cond_signal(&_cond); }
    void inline NotifyAll() { pthread_cond_broadcast(&_cond); }

private:
    MutexLock &_mutexLock;
    pthread_cond_t _cond;
};
}
