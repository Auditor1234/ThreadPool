#include <iostream>
#include <functional>
#include <unistd.h>

#include "../include/thread_pool.h"

using namespace thread_pool;

void call(std::shared_ptr<void> arg) {
    std::cout << "in call" << std::endl;
    std::shared_ptr<char> name = std::static_pointer_cast<char>(arg);
    for(int i = 0; i < 1000; i++)
        std::cout << name << " call " << i << "times." << std::endl;
}

/**
 * 用三个打印任务测试线程池
*/
int main() {
    ThreadPool threadPool(4, 8);
    char name1[] = "thread 1";
    char name2[] = "thread 2";
    char name3[] = "thread 3";
    std::shared_ptr<char> n1(name1);
    std::shared_ptr<char> n2(name2);
    std::shared_ptr<char> n3(name3);
    threadPool.Append(n1, std::bind(call, std::placeholders::_1));
    threadPool.Append(n2, std::bind(call, std::placeholders::_1));
    threadPool.Append(n3, std::bind(call, std::placeholders::_1));
    while(true);
}