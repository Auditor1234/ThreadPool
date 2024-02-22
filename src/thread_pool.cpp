#include <sys/prctl.h>
#include <iostream>

#include "../include/thread_pool.h"
namespace thread_pool {

ThreadPool::ThreadPool(int max_threads, int max_tasks) 
        : thread_size_(max_threads), max_queue_size_(max_tasks), condition_(mutex_), started_(0), shutdown_(0) {
    threads_.resize(max_queue_size_);

    for(int i = 0; i < thread_size_; i++) {
        if(pthread_create(&threads_[i], nullptr, worker, this) != 0) {
            std::cout << "thread create error." << std::endl;
            exit(1);
        }
        started_++;
    }
}


ThreadPool::~ThreadPool() {}

void ThreadPool::Shutdown(int grace) {
    std::cout << "shutdown." << std::endl;
}

void* ThreadPool::worker(void *arg) {
    ThreadPool *pool = static_cast<ThreadPool*>(arg);
    if(pool == nullptr) {
        return nullptr;
    }
    // std::cout << "in worker" << std::endl;
    prctl(PR_SET_NAME, "EventLoopThread");
    pool->run();
    return nullptr;
}


void ThreadPool::run() {
    while(true) {
        ThreadTask task;
        {
            MutexLockGuard guard(this->mutex_);
            while(request_queue_.empty() && !shutdown_) {
                std::cout << "run wait." << std::endl;
                condition_.Wait();
            }
            if(shutdown_) {
                break;
            }
            task = request_queue_.front();
            request_queue_.pop_front();
        }
        task.process(task.arg);
    }
}

bool ThreadPool::Append(std::shared_ptr<void> arg, std::function<void(std::shared_ptr<void>)> fun) {
    if(shutdown_) {
        std::cout << "Thread pool has shutdown." << std::endl;
        return false;
    }
    if(request_queue_.size() > max_queue_size_) {
        std::cout << "task.size() = " << request_queue_.size() << std::endl;
        std::cout << "Too many tasks." << std::endl;
        return false;
    }

    MutexLockGuard guard(this->mutex_);
    ThreadTask task;
    task.process = fun;
    task.arg = arg;

    request_queue_.push_back(task);
    condition_.Notify();
    return true;
}


} // namespace thread_pool
