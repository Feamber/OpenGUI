#include <stdio.h>
#include <string>
#include <thread>
#include <iostream>

using ThreadId = void*;
struct ThreadTask
{
    virtual void Run() = 0;
};
struct ThreadInterface
{
    virtual ThreadId CreateAndRun(ThreadTask&) = 0;
    virtual void WaitUntilComplete(const ThreadId thread) = 0;
    virtual void Destroy(ThreadId) = 0;
};

template<typename T>
struct AsyncHandle
{
    bool Okay() const
    {
        return initialized;
    }
    void Initialize(T&& v)
    {
        value = v;
        initialized = true;
    }
    T& Get() const
    {
        return value;
    }
    T& Get()
    {
        return value;
    }
protected:
    T value;
    bool initialized = false;
};

struct ThreadImplementationDefault : public ThreadInterface
{
    virtual ThreadId CreateAndRun(ThreadTask& task)
    {
        auto thread = new std::thread(&ThreadTask::Run, &task);
        return thread;
    }    
    virtual void Destroy(ThreadId thread)
    {
        std::thread* t = (std::thread*)thread;
        delete t;
    }
    virtual void WaitUntilComplete(const ThreadId thread)
    {
        std::thread* t = (std::thread*)thread;
        t->join();
    }
};

struct FileLoadTask : public ThreadTask
{
    virtual void Run() override
    {
        file.Initialize("123456");
    }
    std::string filename;
    AsyncHandle<std::string> file;
};

int main(void)
{
    ThreadImplementationDefault impl;
    FileLoadTask tsk;
    impl.CreateAndRun(tsk);
    while(!tsk.file.Okay())
    {
        
    }
    std::cout << tsk.file.Get() << std::endl;
    return 0;
}