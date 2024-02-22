# C++线程池的实现

### 问题一：报错segmentation fault

可能原因：

    1. 内存访问越界
    2. 多线程程序使用了线程不安全的函数
    3. 多线程读写的数据未加锁保护
    4. 非法指针
    5. 堆栈溢出

调试分析：

    调试过程发现线程池指针作为参数传递时对象被销毁了
    经过逐步替换每个函数排查问题，发现在run函数中，等待的条件由request_queue_.empty() && !shutdown_写成了thread_.empty() && !shutdown_.导致一直没有进入等待过程。

### 问题二：报错free():invalid pointer.

可能原因：

    1. 重复释放
    2. 非动态分配的内存
    3. 内存越界
    4. 指针未初始化

调试分析：

    调试过程发现有时候是可以正确执行的，说明可能是多线程的原因，经过排查发现是访问了已经释放的内存。因为在main程序中直接return了，没有暂停，所以导致线程使用的资源已经被销毁了。