#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <condition_variable>

struct ThreadTask {
    std::shared_ptr<void> arg;
    std::function<void(std::shared_ptr<void>)> process;
};

class ThreadPool {
public:
    ThreadPool(int max_threads, int max_tasks);
    ~ThreadPool();

    bool append(std::function<void(std::shared_ptr<void>)> func, std::shared_ptr<void> arg);
    void shutdown();

private:
    static void* worker(void* pool);
    void run();

private:
    int thread_nums_;
    int max_tasks_;
    bool shutdown_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::queue<ThreadTask> tasks_queue_;
    std::vector<std::thread> threads_;
};