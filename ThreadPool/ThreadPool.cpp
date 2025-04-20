#include <sys/prctl.h>
#include <iostream>
#include "ThreadPool.h"

ThreadPool::ThreadPool(int max_threads, int max_tasks) {
    thread_nums_ = max_threads;
    max_tasks_ = max_tasks;
    shutdown_ = false;
    for(int i = 0; i < max_threads; i++) {
        threads_.emplace_back(&ThreadPool::worker, this);
    }
}

ThreadPool::~ThreadPool() {
    shutdown();
}
void ThreadPool::shutdown() {
    shutdown_ = true;
    std::cout << "shutdown." << std::endl;
}

bool ThreadPool::append(std::function<void(std::shared_ptr<void>)> func, std::shared_ptr<void> arg) {
    if(shutdown_) {
        std::cout << "Thread pool has shutdown." << std::endl;
        return false;
    }

    if(tasks_queue_.size() >= max_tasks_) {
        std::cout << "Thread pool has full, current task number is " << max_tasks_ << std::endl;
        return false;
    }

    ThreadTask task;
    task.process = func;
    task.arg = arg;
    {
        std::lock_guard<std::mutex> guard(mutex_);
        tasks_queue_.push(task);
    }
    cv_.notify_one();
    return true;
}

void* ThreadPool::worker(void* arg) {
    ThreadPool *pool = static_cast<ThreadPool*>(arg);
    if(pool == nullptr) {
        return nullptr;
    }
    pool->run();
    return nullptr;
}

void ThreadPool::run() {
    while(true) {
        ThreadTask task;
        {
            std::unique_lock<std::mutex> guard(mutex_);
            while(tasks_queue_.empty() && !shutdown_) {
                std::cout << "run wait." << std::endl;
                cv_.wait(guard);
            }
            if(shutdown_) {
                break;
            }
            task = tasks_queue_.front();
            tasks_queue_.pop();
        }
        task.process(task.arg);
    }
}