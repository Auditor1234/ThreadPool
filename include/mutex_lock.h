#pragma once
#include <pthread.h>

#include "noncopyable.h"

namespace thread_pool {

class MutexLock : public Noncopyable{
public:
    MutexLock() { pthread_mutex_init(&mutex_, nullptr); }
    ~MutexLock() { pthread_mutex_destroy(&mutex_); }

public:
    pthread_mutex_t* GetMutex() { return &mutex_; }

    void inline Lock() { pthread_mutex_lock(&mutex_); }
    void inline Unlock() { pthread_mutex_unlock(&mutex_); }

private:
    pthread_mutex_t mutex_;
};

class MutexLockGuard : public Noncopyable{
public:
    MutexLockGuard(MutexLock &mutex) : mutex_(mutex) { mutex_.Lock(); }
    ~MutexLockGuard() { mutex_.Unlock(); }

private:
    MutexLock &mutex_;
};
}
