#include <memory>
#include <functional>
#include <list>
#include <pthread.h>

#include "condition.h"
#include "mutex_lock.h"
#include "noncopyable.h"

namespace thread_pool {

const int MAX_THREAD_SIZE = 1024;
const int MAX_QUEUE_SIZE = 10000;

typedef enum { immediate_mode = 1, graceful_mode = 2 } ShutdownMode;

struct ThreadTask {
    std::function<void(std::shared_ptr<void>)> process;
    std::shared_ptr<void> arg;
};

class ThreadPool {
public:
    ThreadPool(int max_threads, int max_tasks);
    ~ThreadPool();

public:
    bool Append(std::shared_ptr<void> arg, std::function<void(std::shared_ptr<void>)> fun);
    void Shutdown(int graceful);

private:
    static void* worker(void *args);
    void run();

private:
    MutexLock mutex_;
    Condition condition_;

    int thread_size_;
    int max_queue_size_;
    int started_;
    int shutdown_;
    std::vector<pthread_t> threads_;
    std::list<ThreadTask> request_queue_;
};

} // namespace thread_pool
