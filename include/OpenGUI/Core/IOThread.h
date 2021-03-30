#pragma once
#include "OpenGUI/Core/AsyncFile.h"
#include <atomic>
#include <mutex>
#include <vector>
#include <thread>
#include <chrono>
#include <memory>
#include <functional>

namespace OGUI
{
class OGUI_API IOThread
{
public:
    template<typename T = AsyncFile>
    std::shared_ptr<T> Load(const char* path,
        const std::function<void(std::shared_ptr<AsyncFile>)>& completeCallback = [](std::shared_ptr<AsyncFile> tex) {})
    {
        FileLoaderTask task;
        task.path = path;
        task.file = std::shared_ptr<T>(new T(), [](T* file){
            file->Finalize();
        });
        task.complete_callback = completeCallback;
        load_queue_mutex.lock();
        load_queue.push_back(task);
        load_queue_mutex.unlock();
        return std::static_pointer_cast<T>(task.file);
    }
    IOThread();
    ~IOThread();
protected:
    inline void loaderThreadFunction()
    {
        while(is_running)
        {
            load_queue_mutex.lock();
            std::vector<FileLoaderTask> tasks = load_queue;
            load_queue.clear();
            load_queue_mutex.unlock();

            for (FileLoaderTask& task : tasks) {
                task.file->__initialize(task.path);
                
                task.complete_callback(task.file);
            }
            {
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(1ms);
            }
        }
    }
    struct FileLoaderTask 
    {
		const char* path;
		std::shared_ptr<AsyncFile> file;
		std::function<void(std::shared_ptr<AsyncFile>)> complete_callback;
	};
    std::atomic_bool is_running;
    std::thread loader_thread;
	std::vector<FileLoaderTask> load_queue;
	std::mutex load_queue_mutex;
};
}